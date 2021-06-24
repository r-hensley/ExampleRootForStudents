Things done to convert from root 5 files to root 6:


// These define a bunch of variables to use in file paths. I don't know if they're necessary but they're one of the things I did + they're good ideas. You need to add them to the hidden file in your home directory called .bashrc in order to make them permanent.  
`export ROOTSYS=$HOME/root`  
`export PATH=$PATH:$ROOTSYS/bin`  
`export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib`  
`export PROJECT=$HOME/Documents/mu2e/ExampleRootForStudents`  // use your directory path


Add `R__LOAD_LIBRARY($PROJECT/libData.so)` to the first line of your macro file  


Run the following in your main directory:  
`make clean` // In case there are any files compiled on a different system  
`make` // It's good to compile everything from the same system  

One of the files that should come out in `/dataClasses` is `DataCint_rdict.pcm`

Root 6 uses Cling, an interpreter built as an alternative to the original ROOT interpreter Cint. One of the new features of Root 6 is the compliation of files of some kind into these .pcm files (when there is old Cint files?). Anyway, we need to put that next to the .so file, but it'll probably be generated in /dataClasses, so we need to move it.  
In particular, I think DataCint.h is being replaced by DataCint_rdict.pcm. When I did make clean, DataCint.h was removed along with a couple other files, and when I did make, everything else got remade execpt DataCint.h, and I got the new DataCint_rdict.pcm.  
On the other hand, running `make clean` and `make` with ROOT 5 produces DataCint.h and you don't get any DataCint_rdict.pcm files.

CONCLUSION: ROOT 5 makes a DataCint.h file that can be kept in /dataClasses, ROOT 6 makes a DataCint_rdict.pcm file in its place that must be placed next to the libData.so file.
