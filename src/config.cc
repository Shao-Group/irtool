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
double length_ratio = 0;
double coverage_ratio = 0.5;
bool partial_off = false;
bool whole_off = false;

int parse_parameters(int argc, const char ** argv)
{
	for(int i = 1; i < argc; i++)
	{
		if(string(argv[i]) == "-lr")
                {
                        length_ratio = atof(argv[i + 1]);
                        i++;
                }
		else if(string(argv[i]) == "-cr")
                {
                        coverage_ratio = atof(argv[i + 1]);
                        i++;
                }
		else if(string(argv[i]) == "-po")
                {
                        partial_off = true;
                }
		else if(string(argv[i]) == "-wo")
                {
                        whole_off = true;
                }
		else if(string(argv[i]) == "-c")
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
	
	printf("\n");
        printf("options:\n");
        printf(" %-14s  %s\n", "-lr <double>",  "min overlap length-ratio for partial intron retention, default 0");
	printf(" %-14s  %s\n", "-cr <double>",  "coverage-ratio, default 0.5");
        printf(" %-14s  %s\n", "-po",  "turn off partial intron retention detection");
        printf(" %-14s  %s\n", "-wo",  "turn off whole intron retention detection");

	return 0;
}
