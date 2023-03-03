#ifndef __GTFMERGE_H__
#define __GTFMERGE_H__

#include "genome1.h"

using namespace std;

int load_genome3(const string &v, genome1 *gm);

class irtool
{
public:
	genome1 gm1;
	
public:
        int intron_retention_filter(const string &file, const string &fo, const string &fo1);
};

#endif
