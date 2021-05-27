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

RM := rm -rf

CC := g++

#global paths
ROOTINCS = $(shell root-config --incdir)
ROOTLIBS = $(shell root-config --libdir)
ROOTCFLAGS = $(shell root-config --cflags)
PWD = $(shell pwd)

DATA = $(PWD)/dataClasses

-include $(DATA)/Data.mk

all: libData.so \

#ROOT html documentation, it will be done as a program which will be alsa compiled by this makefile, program will be as a last condition after all of the libraries
#htmldoc: libAculData.so
#	-$(RM) htmldoc
#	root -l -q html.cxx	

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

#*.so files
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
