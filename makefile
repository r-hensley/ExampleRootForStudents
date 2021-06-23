################################################################################
# Makefile building all dynamic libraries:
#
#
#	OBJS - files type *.o (subdir.mk)
#	LIBS - list of the loaded libraries (objects.mk)
#	RM - command "rm -rf"
#	CPP_DEPS - files type *.d (subdir.mk)
#	.PHONY - 
#	.SECONDARY - 
#	
# Vratislav Chudoba
################################################################################

# Defining macros for use
# RM is a shortcut for `rm -rf` which ReMoves a file, -r does it recursively, -f forces it
# CC is a shortcut for the C++ compiler g++ (similar is gcc which compiles C code)
# Can be ran later by typing $(RM) or $(CC)
# 
# Example: `$(CC) a_source_file.c$` means `g++ a_source_file.c`. 
# 
# These are useful because if we want to change our compiler from g++ to something else, 
#  we only need to change one line
RM := rm -rf
OS:=$(shell uname)
CC := g++

ifeq ($(OS),Darwin)
	CC := clang++
endif

# global paths
# These are variables we define that will make things work no matter where ROOT is installed on your computer
# 
# For example, my root is installed to /home/ryry013/root/
# ROOTINCS links to the include folder, /home/ryry013/root/include, which has a bunch of importable .h files
# ROOTLIBS links to the lib folder, /home/ryry013/root/lib, which has a bunch of .so and .rootmap files
# ROOTCFLAGS are configurable options for this makefile, like -pthread, -std=c++11, -Wno-deprecated-declarations, etc
# PWD = print working directory, it's the current directory you're in
CXXFLAGS=`root-config --cflags` -fPIC
LDFLAGS=`root-config --ldflags`
GLIBS=`root-config --glibs` -lRooFit -lRooFitCore -lHtml -lMinuit -lFumili
ROOTINCS = $(shell root-config --incdir)
ROOTLIBS = $(shell root-config --libdir)
ROOTCFLAGS = $(shell root-config --cflags)
PWD = $(shell pwd)

# IMPORTANT: if these are not set properly, I think the makefile will finish configuring itself without actually working properly 
# Try running just `root-config` in shell by itself and see if that's installed. If not, consider trying to install it (`sudo apt-get install root-config`)
#  or manually putting in the correct directories

# One more variable, the /dataClasses folder
DATA = $(PWD)/dataClasses

# Data.mk is another makefile over in the DATA folder defined in the above line

# It first defines a variable called $(DATALIBS) which is equal to: 
#  DATALIBS := -lHist #-lCore #-lCint -lRIO -lTree -lNet -lThread -lMatrix -lMathCore -lGpad -lGraf -lSpectrum #-lTELoss

# It then creates the variables DATA_HEADERS, DATACPP_SRCS, DATAOBJS, and DATACPP_DEPS which have files such as 
#  RawEvent.h, linkdef.h, RawEvent.cpp, DataCint.cpp, etc
-include $(DATA)/Data.mk

# This "all" is the scope (or "target") of the make command. If only `make` is called, it will default to "all". In this case, it will go down to the `libData.so` section
all: libData.so \

# ROOT html documentation, it will be done as a program which will be alsa compiled by this makefile, program will be as a last condition after all of the libraries
# htmldoc: libAculData.so
# 	-$(RM) htmldoc
# 	root -l -q html.cxx	

clean:
	-$(RM) $(DATAOBJS) $(DATACPP_DEPS) 
	-$(RM) $(DATA)/DataCint.* libData.so
	-@echo ' '
	-$(RM) htmldoc
	-@echo ' '

# Those *Cint* files below need special treating:
$(DATA)/DataCint.cpp:
	-@echo 'Pre-building DataCint.cpp and DataCint.h files'
	-rootcint -f $(DATA)/DataCint.cpp -c -p $(DATA_HEADERS)
	-@echo ' '

# *.so files
libData.so: $(DATAOBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	# This compiles into libData.so all the default libraries in root + all the ones in DATALIBS like lHist + RawEvent.o + DataCint.o
	$(CC) -L $(ROOTLIBS) -shared -o"libData.so" -std=c++11 -stdlib=libc++ $(LDFLAGS) $(CXXFLAGS) $(GLIBS) $(DATAOBJS) $(DATALIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# If you type `make .PHONY`, it'll construct libData.so then immediately run clean and delete it.
.PHONY: all clean

# Each subdirectory must supply rules for building sources it contributes
%.o: %.cpp
	@echo 'Building file: $@'
	@echo 'Invoking: $(CC) Compiler'
	$(CC) -I$(ROOTINCS) -O0 -g3 -Wall -c -fmessage-length=0 -fPIC $(ROOTCFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
#	$(CC) -I$(ROOTINCS) -O2 -Wall -mmmx -msse -msse2 -msse3 -mfpmath=sse,387 -march=nocona -mtune=nocona -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $@'
	@echo ' '
