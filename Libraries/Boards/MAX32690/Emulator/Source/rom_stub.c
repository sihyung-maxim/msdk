/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by 
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

#include "max32690.h"
#include "mxc_device.h"

/* The stack address is defined by the linker
 * It is typed as a function here to avoid compiler warnings
 */
extern void __StackTop(void);

void Reset_Handler(void);
void NMI_Handler_ROM(void);
void HardFault_Handler_ROM(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

/* Create a vector table to locate at zero in the ROM for handling reset and startup */
__attribute__((section(".rom_vector"))) void (*const rom_vector[])(void) = {
    __StackTop, /* Top of Stack */
    Reset_Handler, /* Reset Handler */
    NMI_Handler_ROM, /* NMI Handler */
    HardFault_Handler_ROM, /* Hard Fault Handler */
    MemManage_Handler, /* MPU Fault Handler */
    BusFault_Handler, /* Bus Fault Handler */
    UsageFault_Handler, /* Usage Fault Handler */
    0, /* Reserved */
    0, /* Reserved */
    0, /* Reserved */
    0, /* Reserved */
    SVC_Handler, /* SVCall Handler */
    DebugMon_Handler, /* Debug Monitor Handler */
    0, /* Reserved */
    PendSV_Handler, /* PendSV Handler */
    SysTick_Handler, /* SysTick Handler */
};

/* This is needed to handle the NMI at POR */
__attribute__((section(".rom_handlers"))) void NMI_Handler_ROM(void)
{
    __NOP();
}

/* This is needed to handle the fault after initial programming */
__attribute__((section(".rom_handlers"))) void HardFault_Handler_ROM(void)
{
    NVIC_SystemReset();
}
