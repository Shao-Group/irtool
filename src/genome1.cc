#include "genome1.h"
#include "config.h"
#include <cassert>
#include <fstream>
#include <cmath>

genome1::genome1()
{}

genome1::genome1(const string &file)
{
	build(file);
}

int genome1::build4(const string &file)
{
        clear();
        genome gm(file);
        int tcount = 0;
        int icount = 0;
        bool add;
        vector< pair<int32_t, int32_t> > ilist;
        vector<double> ilist_cov;
        set<int32_t> blist;
        set<string> iclist;

        string gid;
        string seqname;

        // add all transcripts
        for(int i = 0; i < gm.genes.size(); i++)
        {
                const gene &g = gm.genes[i];
                for(int k = 0; k < g.transcripts.size(); k++)
                {
                        transcript t = g.transcripts[k];
                        //if(t.exons.size() <= 1) continue;
                        if(t.exons.size() < 1) continue;
                        if(merge_coverage_as_counts == true) t.coverage = 1.0;
                        else if(merge_coverage_log == true) t.coverage = log(1 + t.coverage);
                        add_transcript(t);
                }
        }

	int total_b = 0;
        int total_i = 0;
        int total_ic = 0;

        // add boundary list: blist, add intron list: ilist, add intron-chain list: iclist
        for(int i = 0; i < gm.genes.size(); i++)
        {
                const gene &g = gm.genes[i];
                tcount += g.transcripts.size();
                ilist.clear();
                ilist_cov.clear();
                blist.clear();
                iclist.clear();
                add = false;
                for(int k = 0; k < g.transcripts.size(); k++)
                {
                        transcript t = g.transcripts[k];
                        if(t.exons.size() > 1) {
                                if(add == false) {
                                        add = true;
                                        gid = t.gene_id;
                                        seqname = t.seqname;
                                }
                                for(int j = 0; j < t.exons.size() - 1; j++)
                                {
                                        ilist.push_back({t.exons[j].second, t.exons[j+1].first});
                                        ilist_cov.push_back(t.coverage);
                                        blist.insert(t.exons[j].second);
       	                                blist.insert(t.exons[j+1].first);
                                        icount++;
                                }
                                iclist.insert(compute_intron_hashing(t));
                        }
                }
                if(add == true)
                {
                        vector<string> mg;
                        mg.push_back(gid);
                        mg.push_back(seqname);
                        mgenes.push_back(mg);
                        mgilist.push_back(ilist);
                        mgilist_cov.push_back(ilist_cov);
                        mgblist.push_back(blist);
                        mgiclist.push_back(iclist);
                        mgi[gid] = mgenes.size() - 1;

                        total_b += blist.size();
                        total_i += ilist.size();
                        total_ic += iclist.size();
                }
        }

        //printf("total # genes = %d, # multi-exon genes = %d, # total boundaries = %d, # total junctions = %d, # total intron-chains = %d \n", gm.genes.size(), mgenes.size(), total_b, total_i, total_ic);
        return 0;
}

int genome1::build(const string &file)
{
	clear();
	genome gm(file);
	for(int i = 0; i < gm.genes.size(); i++)
	{
		const gene &g = gm.genes[i];
		for(int k = 0; k < g.transcripts.size(); k++)
		{
			transcript t = g.transcripts[k];
			if(t.exons.size() <= 1) continue;
			if(merge_coverage_as_counts == true) t.coverage = 1.0;
			else if(merge_coverage_log == true) t.coverage = log(1 + t.coverage);
			add_transcript(t);
		}
	}
	return 0;
}

int genome1::build(const vector<transcript> &v)
{
	clear();
	for(int i = 0; i < v.size(); i++)
	{
		add_transcript(v[i]);
	}
	return 0;
}

int genome1::clear()
{
	transcripts.clear();
	intron_hashing.clear();
	gt.clear();
	stranscripts.clear();
	mgenes.clear();
	mgblist.clear();
	return 0;
}

int genome1::add_transcript(const transcript &t)
{
	string s = compute_intron_hashing(t);
	if(intron_hashing.find(s) == intron_hashing.end())
	{
		intron_hashing.insert(PSI(s, transcripts.size()));
		transcripts.push_back(t);
	}
	else
	{
		int k = intron_hashing[s];
		assert(k >= 0 && k < transcripts.size());
		transcript tt = transcripts[k];
		if(transcripts[k].length() > t.length()) 
		{
			transcripts[k].coverage += t.coverage;
		}
		else
		{
			double c = transcripts[k].coverage + t.coverage;
			transcripts[k] = t;
			transcripts[k].coverage = c;
		}
	}
	return 0;
}

int genome1::build_intersection(const genome1 &gm, genome1 &out)
{
	out.clear();
	for(MSI::iterator it = intron_hashing.begin(); it != intron_hashing.end(); it++)
	{
		string s = it->first;
		int k1 = it->second;
		transcript t = transcripts[k1];
		MSI::const_iterator x = gm.intron_hashing.find(s);
		if(x == gm.intron_hashing.end()) continue;
		int k2 = x->second;
		t.coverage += gm.transcripts[k2].coverage;
		out.add_transcript(t);
	}
	return 0;
}

int genome1::build_union(const genome1 &gm)
{
	for(int k = 0; k < gm.transcripts.size(); k++)
	{
		const transcript &t = gm.transcripts[k];
		add_transcript(t);
	}
	return 0;
}

int genome1::add_suffix(const string &p)
{
	for(int i = 0; i < transcripts.size(); i++)
	{
		transcript &t = transcripts[i];
		t.transcript_id.append("-").append(p);
		t.gene_id.append("-").append(p);
	}
	return 0;
}

int genome1::print(int index)
{
	printf("genome %d: %lu transcripts, %lu distinct first intron\n", index, transcripts.size(), intron_hashing.size());
	return 0;
}

int genome1::print_hashing()
{
	for(int i = 0; i < transcripts.size(); i++)
	{
		string s = compute_intron_hashing(transcripts[i]);
		printf("hash = %s\n", s.c_str());
	}
	return 0;
}

int genome1::write(const string &file)
{
	ofstream fout(file.c_str());
	if(fout.fail()) return 0;
	for(int i = 0; i < transcripts.size(); i++)
	{
		transcript &t = transcripts[i];
		if(t.coverage < min_transcript_coverage) continue;
		t.write(fout);
	}
	fout.close();
	return 0;
}

string tostring(int p)
{
	char buf[10240];
	sprintf(buf, "%d", p);
	return string(buf);
}

string genome1::compute_intron_hashing(const transcript &t)
{
	string h = t.seqname;
	
	// do not consider strand
	/*
	if(t.strand == '.') h.append("0");
	if(t.strand == '+') h.append("1");
	if(t.strand == '-') h.append("2");
	*/

	// single exon, seqname + start + (end - start)
	if(t.exons.size() <= 1) 
	{
		h.append(tostring(t.start));
		h.append(tostring(t.end - t.start));
		return h;
	}

	// multi exon
	int32_t p = t.exons[0].second;
	h.append(tostring(p));

	for(int k = 1; k < t.exons.size(); k++)
	{
		int32_t q1 = t.exons[k].first;
		int32_t q2 = t.exons[k].second;
		h.append(tostring(q1 - p));
		if(k == t.exons.size() - 1) break;
		h.append(tostring(q2 - q1));
		p = q2;
	}
	return h;
}
