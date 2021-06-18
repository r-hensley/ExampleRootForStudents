//
// These files for students analysis raw data  
// Must use Root 5  
// Depends on having root-config installed
// I don't know if it's necessary but I got the main ones here: https://root.cern/install/dependencies
// (Optional download didn't work for me)
// I follow https://root.cern/install/build_from_source/
// I started with `git clone --branch v5-34-38 https://github.com/root-project/root.git root_src`
// This downloads it to a folder called root_src
// First, use the makefile to install necessary files
// If you get error 2: This does not look like a tar archive, do: `sudo apt install curl`

$ make

//
// Example untar file 

$ tar xvjf file.tbz

// x = extract  
// v = verbose, verbosely list files processed  
// j = use bzip2  
// f = file, take file input

//  
// For us

/data$ tar xvjf 1000evn.tbz

// Example analysis raw data

$ cd macros  
/macros$ root -l  
root[0] .L testShowProf_rawevent.cxx  
root[1] testShowProf_rawevent("path/to/rootfile",100)         //100 is the number of events  
// as of initial commit, the path to the rootfile from the macros folder is "../data/1000evn_v3.root"  


