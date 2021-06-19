//  
// These files for students analysis raw data  
// There are files for root 5 or root 6 installed, along with instructions for how to convert from root 5 to root 6.


// I don't know if it's necessary but I got the main dependencies listed here: https://root.cern/install/dependencies  
// (Optional download didn't work for me)  
// I follow https://root.cern/install/build_from_source/  
// I started with `git clone --branch v5-34-38 https://github.com/root-project/root.git root_src` for root 5.

// This downloads it to a folder called root_src  
// First, use the makefile to install necessary files  
// If you get error 2: This does not look like a tar archive, do: `sudo apt install curl`

// For root 6, you're probably able to just download one of the prebuilt distributions found by googling `root cern releases`.

// To add ROOT to your environment variables (allows you to open it by just typing `root`), you need to follow these instructions:  
// https://root.cern.ch/root/htmldoc/guides/users-guide/GettingStarted.html  
// These will only work for the session you're in. To set them permanently, add them to your .bashrc file, a hidden file in your home directory. The ones I added for Ubuntu 20 were:  

export ROOTSYS=$HOME/root  
export PATH=$PATH:$ROOTSYS/bin  
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib  
export PROJECT=$HOME/Documents/mu2e/ExampleRootForStudents  // use your directory path  

// 
// Next, go into the main directory of your ForStudents files, open terminal, type `make clean`, then `make`. This compiles a bunch of necessary files for you.

// 
// You'll need to untar the data .root file.  
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

