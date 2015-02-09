################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/VisualRhythm.cpp \
../src/Main.cpp \
../src/Video.cpp 

OBJS += \
./src/VisualRhythm.o \
./src/Main.o \
./src/Video.o 

CPP_DEPS += \
./src/VisualRhythm.d \
./src/Main.d \
./src/Video.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ $(OPENCVFLAGS) -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


