#ifndef __GENOME1_H__
#define __GENOME1_H__

#include "genome.h"

using namespace std;

typedef pair<string, int> PSI;
typedef map<string, int> MSI;

class genome1
{
public:
	genome1();
	genome1(const string &file);

public:
	vector<transcript> transcripts;
	MSI intron_hashing;

	map<string, vector<int> > gt; // gene_id and its transcripts list

	vector<transcript> stranscripts; // single-exon
	vector<vector<string>> mgenes; // <<gene_id, seqname>>, genes containing multi-exon transcripts
	vector< set<int32_t> > mgblist; // boundry list of genes containing multi-exon transcripts
	vector< vector< pair<int32_t, int32_t> > > mgilist; // intron list of genes containing multi-exon transcripts
	vector< vector<double> > mgilist_cov; // coverage of intron list 'mgilist'
	vector< set<string> > mgiclist; // intron-chain list of genes containing multi-exon transcripts
	map<string, int> mgi; // <gene_id, index> index in mgenes
	vector<transcript> irtranscripts; // intron-retention transcripts
	vector<transcript> irftranscripts; // intron-retention filtered transcripts

public:
	int add_transcript(const transcript &t);
	int build(const string &file);
	int build(const vector<transcript> &v);
	int build_union(const genome1 &gm);
	int build_intersection(const genome1 &gm, genome1 &out);
	int clear();
	int write(const string &file);
	int add_suffix(const string &p);
	int remove_redundancy();
	int print(int index);
	int print_hashing();
	string compute_intron_hashing(const transcript &t);
	int build4(const string &file);
	int write_individual(const vector<string> &ogtf);
private:
	int build_multiexon_transcripts(const string &file);
};

string tostring(int p);

#endif
