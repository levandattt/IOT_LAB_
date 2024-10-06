################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/board/ra6m5_ck/board_init.c \
../ra/board/ra6m5_ck/board_leds.c 

C_DEPS += \
./ra/board/ra6m5_ck/board_init.d \
./ra/board/ra6m5_ck/board_leds.d 

OBJS += \
./ra/board/ra6m5_ck/board_init.o \
./ra/board/ra6m5_ck/board_leds.o 

SREC += \
Daty3_Humidity_N_Temperature.srec 

MAP += \
Daty3_Humidity_N_Temperature.map 


# Each subdirectory must supply rules for building sources it contributes
ra/board/ra6m5_ck/%.o: ../ra/board/ra6m5_ck/%.c
	$(file > $@.in,-mcpu=cortex-m33 -mthumb -mfloat-abi=hard -mfpu=fpv5-sp-d16 -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-strict-aliasing -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal -g -D_RENESAS_RA_ -D_RA_CORE=CM33 -D_RA_ORDINAL=1 -I"/home/xenwithu/e2_studio/workspace/Daty3_Humidity_N_Temperature/src" -I"." -I"/home/xenwithu/e2_studio/workspace/Daty3_Humidity_N_Temperature/ra/fsp/inc" -I"/home/xenwithu/e2_studio/workspace/Daty3_Humidity_N_Temperature/ra/fsp/inc/api" -I"/home/xenwithu/e2_studio/workspace/Daty3_Humidity_N_Temperature/ra/fsp/inc/instances" -I"/home/xenwithu/e2_studio/workspace/Daty3_Humidity_N_Temperature/ra/arm/CMSIS_6/CMSIS/Core/Include" -I"/home/xenwithu/e2_studio/workspace/Daty3_Humidity_N_Temperature/ra_gen" -I"/home/xenwithu/e2_studio/workspace/Daty3_Humidity_N_Temperature/ra_cfg/fsp_cfg/bsp" -I"/home/xenwithu/e2_studio/workspace/Daty3_Humidity_N_Temperature/ra_cfg/fsp_cfg" -std=c99 -Wno-stringop-overflow -Wno-format-truncation --param=min-pagesize=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

