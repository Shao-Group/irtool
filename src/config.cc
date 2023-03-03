#include "config.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace std;

double min_transcript_coverage = -1;
bool merge_coverage_as_counts = false;
bool merge_coverage_log = false;
int num_threads = 1;

int parse_parameters(int argc, const char ** argv)
{
	for(int i = 1; i < argc; i++)
	{
		if(string(argv[i]) == "-c")
		{
			min_transcript_coverage = atof(argv[i + 1]);
			i++;
		}
		else if(string(argv[i]) == "-t")
		{
			num_threads = atoi(argv[i + 1]);
			i++;
		}
		else if(string(argv[i]) == "-n")
		{
			merge_coverage_as_counts = true;
		}
		else if(string(argv[i]) == "-e")
		{
			merge_coverage_log = true;
		}
	}

	return 0;
}

int print_help()
{
	printf("\n");
	printf("usage: irtool <input-gtf-file> <intron-retention-gtf-file> <filtered-gtf-file>\n");
	return 0;
}
