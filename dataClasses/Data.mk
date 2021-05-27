################################################################################
# AculData input with some variables
################################################################################

DATALIBS := -lHist #-lCore #-lCint -lRIO -lTree -lNet -lThread -lMatrix -lMathCore -lGpad -lGraf -lSpectrum #-lTELoss

# Add inputs and outputs from these tool invocations to the build variables 
DATA_HEADERS += \
$(DATA)/RawEvent.h \
$(DATA)/linkdef.h


DATACPP_SRCS += \
$(DATA)/RawEvent.cpp \
$(DATA)/DataCint.cpp

DATAOBJS += \
$(DATA)/RawEvent.o \
$(DATA)/DataCint.o

DATACPP_DEPS += \
$(DATA)/RawEvent.d \
$(DATA)/DataCint.d
