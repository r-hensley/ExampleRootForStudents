Things done to convert from root 5 files to root 6:

// These define a bunch of variables to use in file paths. I don't know if they're necessary but they're one of the things I did + they're good ideas.
export ROOTSYS=$HOME/root
export PATH=$PATH:$ROOTSYS/bin
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ROOTSYS/lib
export PROJECT=$HOME/Documents/mu2e/ExampleRootForStudents  // use your directory path

Add R__LOAD_LIBRARY($PROJECT/libData.so) to the first line of your macro file

make clean // some of the files in /dataClasses were made on Root 5, we need to delete them all...
make // ... and then remake them all here like this

One of the files that should come out in /dataClasses is DataCint_rdict.pcm

Root 6 uses Cling, an interpreter built as an alternative to the original ROOT interpreter Cint. One of the new features of Root 6 is the compliation of files of some kind into 
these .pcm files (when there is old Cint files?). Anyway, we need to put that next to the .so file, but it'll probably be generated in /dataClasses, so we need to move it.

If you run into the problem of Root closing with no errors, I'm guessing it's memory overload, so change all the 10,000 in the macro to 100 or 1000.
This post (https://root-forum.cern.ch/t/no-error-when-macro-crashes/14931) also implies a command to increase some memory limit but I think that didn't immediately fix it for me so it's better probably to reduce to 100
