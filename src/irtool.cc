#include "genome1.h"
#include "irtool.h"
#include "config.h"

#include <string>
#include <thread>

int load_genome3(const string &s, genome1 *gm)
{
        gm->clear();
        gm->build4(s);
        return 0;
}

int irtool::intron_retention_filter(const string &file, const string &fo, const string &fo1)
{
        load_genome3(file.c_str(), &gm1);
	bool if_ir = false;
	int mcount = 0;
	
	printf("# total transcripts = %lu, ", gm1.transcripts.size());
	
	for(int i = 0; i < gm1.transcripts.size(); i++)
        {
		if_ir = false;
		transcript t = gm1.transcripts[i];
		if(t.exons.size() <= 1) 
		{
			gm1.irftranscripts.push_back(t);
			continue;
		}
		mcount++;

		double t_cov = t.coverage;

		int mgindex = gm1.mgi[t.gene_id];
		vector< pair<int32_t, int32_t> > ilist = gm1.mgilist[mgindex];
		vector<double> ilist_cov = gm1.mgilist_cov[mgindex];

		double cov_threshold = 1.0 / coverage_ratio;

		for(int j = 0; j < t.exons.size(); j++)
		{
			int32_t lpos = t.exons[j].first;
			int32_t rpos = t.exons[j].second;
			
			if(!partial_off)
			{
				// check if first/last exon included in an intron
				if(j == 0)
				{
					for(int k = 0; k < ilist.size() - 1; k++)
					{
						if(t_cov > cov_threshold * ilist_cov[k]) continue;
						if(lpos > ilist[k].first && lpos < ilist[k].second && rpos == ilist[k+1].first)
						{
							// check length ratio, skip if overlap ratio < threshold
							double overlap_length = ilist[k].second - lpos;
							double intron_length = ilist[k].second - ilist[k].first;

							if(overlap_length < length_ratio * intron_length) continue;

							gm1.irtranscripts.push_back(t);
							if_ir = true;
							break;
						}
					}
				}
				else if(j == t.exons.size() - 1)
				{
					for(int k = 1; k < ilist.size(); k++)
					{
						if(t_cov > cov_threshold * ilist_cov[k]) continue;
						if(rpos > ilist[k].first && rpos < ilist[k].second && lpos == ilist[k-1].second)
						{
							// check length ratio, skip if overlap ratio < threshold
							double overlap_length = rpos - ilist[k].first;
                                                        double intron_length = ilist[k].second - ilist[k].first;

                                                        if(overlap_length < length_ratio * intron_length) continue;

							gm1.irtranscripts.push_back(t);
							if_ir = true;
							break;
						}
					}
				}
				if(if_ir == true) break;
			}

			if(!whole_off)
			{
				// exon overlap whole intron
				for(int k = 0; k < ilist.size(); k++)
				{
					if(t_cov > cov_threshold * ilist_cov[k]) continue;
					if(ilist[k].first > lpos && ilist[k].second < rpos)
					{
						if((rpos - lpos) / (ilist[k].second - ilist[k].first) > 4.0) continue;
						gm1.irtranscripts.push_back(t);
						if_ir = true;
						break;
					}
				}
				if(if_ir == true) break;
			}
		}
		if(if_ir == false) gm1.irftranscripts.push_back(t);
	}
	printf("# multi-exon transcripts = %d, # intron-retention transcripts = %lu, # transcripts after intron-retention filtering = %lu\n", mcount, gm1.irtranscripts.size(), gm1.irftranscripts.size());

	ofstream fout(fo.c_str());
	for(int i = 0; i < gm1.irtranscripts.size(); i++)
	{
		transcript t = gm1.irtranscripts[i];
		t.write(fout);
	}

	ofstream fout1(fo1.c_str());
	for(int i = 0; i < gm1.irftranscripts.size(); i++)
	{
		transcript t = gm1.irftranscripts[i];
		t.write(fout1);
	}

	return 0;
}

