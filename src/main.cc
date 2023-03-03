#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <iostream>
#include <cassert>

#include "irtool.h"
#include "config.h"

using namespace std;

int main(int argc, const char **argv)
{
	if(argc <= 3)
	{
		print_help();
		return 0;
	}

	parse_parameters(argc, argv);

	assert(argc >= 4);
	irtool gm;
        gm.intron_retention_filter(argv[1], argv[2], argv[3]);	

    return 0;
}
