//
// These files for students analysis raw data
// Must use Root 5
// Depends on having root-config installed

// 
// First, use the makefile to install necessary files

ForStudents$ make

//
// Example untar file 

$ tar xvjf file.tbz

// x = extract
// v = verbose, verbosely list files processed
// j = use bzip2
// f = file, take file input

// 
// For us

ForStudents/data$ tar xvjf 1000evn.tbz

// Example analysis raw data

ForStudents$ cd macros
ForStudents/macros$ root -l
root[0] .L testShowProf_rawevent.cxx
root[1] testShowProf_rawevent("path/to/rootfile",100)         //100 is the number of events
// as of initial commit, the path to the rootfile from the macros folder is "../data/1000evn_v3.root"


