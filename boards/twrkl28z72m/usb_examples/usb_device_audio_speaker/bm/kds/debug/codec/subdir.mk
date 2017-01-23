################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
F:/MCUXpresso\ SDK/SDK_2.1_TWR-KL28Z72M/boards/twrkl28z72m/usb_examples/usb_device_audio_speaker/bm/fsl_sgtl5000.c 

OBJS += \
./codec/fsl_sgtl5000.o 

C_DEPS += \
./codec/fsl_sgtl5000.d 


# Each subdirectory must supply rules for building sources it contributes
codec/fsl_sgtl5000.o: F:/MCUXpresso\ SDK/SDK_2.1_TWR-KL28Z72M/boards/twrkl28z72m/usb_examples/usb_device_audio_speaker/bm/fsl_sgtl5000.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0plus -mthumb -mfloat-abi=soft -O0 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -ffreestanding -fno-builtin -Wall  -g -D_DEBUG=1 -DCPU_MKL28Z512VLL7 -DUSB_STACK_USE_DEDICATED_RAM=1 -DUSB_STACK_BM -DUSING_SAI -DTWR_KL28Z72M -DTOWER -I../.. -I../../../../../../../CMSIS/Include -I../../../../../../../devices -I../../../../../../../middleware/usb_1.6.3 -I../../../../../../../middleware/usb_1.6.3/osa -I../../../../../../../middleware/usb_1.6.3/include -I../../../../../../../middleware/usb_1.6.3/device -I../../../../.. -I../../../../../../../devices/MKL28Z7/drivers -I../../../../../../../devices/MKL28Z7/utilities -I../../../../../../../devices/MKL28Z7 -std=gnu99 -mapcs -MMD -MP -MF"codec/fsl_sgtl5000.d" -MT"codec/fsl_sgtl5000.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


