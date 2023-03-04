# Introduction
This is a tool to identify and filtering out transcripts with intron-retentions from an assembly.

# Installation
Download the source code of the latest release 
[here](https://github.com/Shao-Group/irtool/releases/download/v1.0.0/irtool-1.0.0.tar.gz),
then use the following commands to compile:
```
./configure --prefix=/path/to/your/install/folder
make
make install
```

# Usage
Given the input transcript file (in `gtf` format), irtool will produce two files (in `gtf` format):      
1. transcripts with intron-retentions,          
2. transcripts after filtering out transcripts with intron-retentions.

The usage of `irtool` is:
```
irtool <input-gtf-file> <intron-retention-gtf-file> <filtered-gtf-file>
```

The `input-gtf-file` is the input transcript file (in `gtf` format).
The `intron-retention-gtf-file` is an output file (in `gtf` format) contains all transcripts with intron-retentions.
The `filtered-gtf-file` is another output file (in `gtf` format) contains the remaining transcripts after filtering. 
