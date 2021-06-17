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

CC := g++

# global paths
# These are variables we define that will make things work no matter where ROOT is installed on your computer
# 
# For example, my root is installed to /home/ryry013/root/
# ROOTINCS links to the include folder, /home/ryry013/root/include, which has a bunch of importable .h files
# ROOTLIBS links to the lib folder, /home/ryry013/root/lib, which has a bunch of .so and .rootmap files
# ROOTCFLAGS are configurable options for this makefile, like -pthread, -std=c++11, -Wno-deprecated-declarations, etc
# PWD = print working directory, it's the current directory you're in
ROOTINCS = $(shell root-config --incdir)
ROOTLIBS = $(shell root-config --libdir)
ROOTCFLAGS = $(shell root-config --cflags)
PWD = $(shell pwd)

# IMPORTANT: if these are not set properly, I think the makefile will finish configuring itself without actually working properly 
# Try running just `root-config` in shell by itself and see if that's installed. If not, consider trying to install it (`sudo apt-get install root-config`)
#  or manually putting in the correct directories

# One more variable, the /dataClasses folder
DATA = $(PWD)/dataClasses

-include $(DATA)/Data.mk

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
	$(CC) -L $(ROOTLIBS) -shared -o"libData.so" $(DATAOBJS) $(DATALIBS)
	@echo 'Finished building target: $@'
	@echo ' '

.PHONY: all clean

# Each subdirectory must supply rules for building sources it contributes
%.o: %.cpp
	@echo 'Building file: $@'
	@echo 'Invoking: $(CC) Compiler'
	$(CC) -I$(ROOTINCS) -O0 -g3 -Wall -c -fmessage-length=0 -fPIC $(ROOTCFLAGS) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
#	$(CC) -I$(ROOTINCS) -O2 -Wall -mmmx -msse -msse2 -msse3 -mfpmath=sse,387 -march=nocona -mtune=nocona -c -fmessage-length=0 -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $@'
	@echo ' '
