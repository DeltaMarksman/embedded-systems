################################################################################
# Automatically-generated file. Do not edit!
################################################################################

CG_TOOL_ROOT := /Applications/ti/ccs2041/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./GUIComm_JSON.obj" \
"./callbacks_JSON.obj" \
"./main.obj" \
"./src/MSP430_GUI/GUI_JSON.obj" \
"./src/MSP430_GUI/jsmn/jsmn.obj" \
"../lnk_msp430fr6989.cmd" \
"../src/IQMathLib/CCS/MPY32/5xx_6xx/IQmathLib_CCS_MPY32_5xx_6xx_CPUX_large_code_large_data.lib" \
"../src/IQMathLib/CCS/MPY32/5xx_6xx/IQmathLib_CCS_MPY32_5xx_6xx_CPUX_large_code_restricted_data.lib" \
"../src/IQMathLib/CCS/MPY32/5xx_6xx/IQmathLib_CCS_MPY32_5xx_6xx_CPUX_large_code_small_data.lib" \
"../src/IQMathLib/CCS/MPY32/5xx_6xx/IQmathLib_CCS_MPY32_5xx_6xx_CPUX_small_code_small_data.lib" \
"../src/IQMathLib/CCS/MPY32/5xx_6xx/QmathLib_CCS_MPY32_5xx_6xx_CPUX_large_code_large_data.lib" \
"../src/IQMathLib/CCS/MPY32/5xx_6xx/QmathLib_CCS_MPY32_5xx_6xx_CPUX_large_code_restricted_data.lib" \
"../src/IQMathLib/CCS/MPY32/5xx_6xx/QmathLib_CCS_MPY32_5xx_6xx_CPUX_large_code_small_data.lib" \
"../src/IQMathLib/CCS/MPY32/5xx_6xx/QmathLib_CCS_MPY32_5xx_6xx_CPUX_small_code_small_data.lib" \
$(GEN_CMDS__FLAG) \
-llibmpu_init.a \
-llibmath.a \
-llibc.a \

-include ../makefile.init

RM := rm -rf
RMDIR := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include src/IQMathLib/CCS/MPY32/5xx_6xx/subdir_vars.mk
-include src/MSP430_GUI/subdir_vars.mk
-include src/MSP430_GUI/jsmn/subdir_vars.mk
-include subdir_rules.mk
-include src/IQMathLib/CCS/MPY32/5xx_6xx/subdir_rules.mk
-include src/MSP430_GUI/subdir_rules.mk
-include src/MSP430_GUI/jsmn/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
lab3.out 

EXE_OUTPUTS__QUOTED += \
"lab3.out" 


# All Target
all: $(OBJS) $(CMD_SRCS) $(A_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "lab3.out"

# Tool invocations
lab3.out: $(OBJS) $(CMD_SRCS) $(A_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: MSP430 Linker'
	"/Applications/ti/ccs2041/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/bin/cl430" -vmspx --data_model=restricted -O3 --use_hw_mpy=F5 --advice:power="none" --advice:hw_config=all --define=__MSP430FR6989__ --define=_MPU_ENABLE --define=__ENABLE_GUI_COMM=1 --define=GLOBAL_Q=8 -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 -z -m"lab3.map" --heap_size=160 --stack_size=160 --cinit_hold_wdt=on -i"/Applications/ti/ccs2041/ccs/ccs_base/msp430/include" -i"/Applications/ti/ccs2041/ccs/ccs_base/msp430/lib/5xx_6xx_FRxx" -i"/Applications/ti/ccs2041/ccs/ccs_base/msp430/lib/FR59xx" -i"/Applications/ti/ccs2041/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/lib" -i"/Applications/ti/ccs2041/ccs/tools/compiler/ti-cgt-msp430_21.6.1.LTS/include" --priority --reread_libs --define=_MPU_ENABLE --diag_wrap=off --display_error_number --warn_sections --xml_link_info="lab3_linkInfo.xml" --use_hw_mpy=F5 --rom_model -o "lab3.out" $(ORDERED_OBJS) $(A_SRCS)
	@echo 'Finished building target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)
	-$(RM) "GUIComm_JSON.obj" "callbacks_JSON.obj" "main.obj" "src/MSP430_GUI/GUI_JSON.obj" "src/MSP430_GUI/jsmn/jsmn.obj" 
	-$(RM) "GUIComm_JSON.d" "callbacks_JSON.d" "main.d" "src/MSP430_GUI/GUI_JSON.d" "src/MSP430_GUI/jsmn/jsmn.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

