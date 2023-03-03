# Introduction
irtool an intron-retention tool to identify transcripts (in `gtf` format) with intron-retentions.

# Installation
Install from source code: download the source code of irtool from 
[here](https://github.com/Shao-Group/irtool),
then use the following commands to compile:
```
./configure --prefix=/path/to/your/install/folder
make
make install
```

# Usage
Given the input transcript file (in `gtf` format), it will output two files (in `gtf` format):      
1. transcripts with intron-retentions,          
2. retained transcripts after filtering out transcripts with intron-retentions.

The usage of `irtool` is:
```
irtool <input-gtf-file> <intron-retention-gtf-file> <filtered-gtf-file>
```

The `input-gtf-file` is the input transcript file (in `gtf` format).
The `intron-retention-gtf-file` is the first output transcript file (in `gtf` format) contains all transcripts with intron-retentions.
The `filtered-gtf-file` is the second output transcript file (in `gtf` format) contains the remaining transcripts after filtering. 
