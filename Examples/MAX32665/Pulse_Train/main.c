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

/**
 * @file main.c
 * @brief Configures and starts four different pulse trains on GPIO LEDs.
 * @details LED0 (P1.14) - PT14 setup as 2Hz continuous signal that outputs 10110b
 *          LED1 (P1.15) - PT15 setup as 10Hz continuous square wave
 *          SW1 - push button setup to stop/start pulse trains
 *
 * @note Interrupts for pulse trains are enabled but the interrupt handler only clears the flags.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_device.h"
#include "board.h"
#include "pb.h"
#include "pt.h"

/***** Definitions *****/
#define ALL_PT 0xFFFF
/***** Globals *****/
#define PTG MXC_PTG_BUS0

/***** Functions *****/

// *****************************************************************************
static void PB_Start_Stop_handler(void)
{
    uint32_t enablePTMask;

    //Check if any pulse trains are running
    if (MXC_PT_IsActive(PTG, ALL_PT)) {
        //stop all pulse trains
        MXC_PT_Stop(PTG, ALL_PT);
    } else {
        //start PT14 and PT15
        enablePTMask = MXC_F_PTG_ENABLE_PT14 | MXC_F_PTG_ENABLE_PT15;
        MXC_PT_Start(PTG, enablePTMask);
        MXC_PT_Start(PTG, MXC_F_PTG_ENABLE_PT14);
        MXC_PT_Start(PTG, MXC_F_PTG_ENABLE_PT15);
    }
}

// *****************************************************************************
void PT_IRQHandler(void)
{
    printf("flags = 0x%08x\n", MXC_PT_GetFlags(PTG));

    MXC_PT_ClearFlags(PTG, ALL_PT);
}

// *****************************************************************************
void ContinuousPulseTrain(void)
{
    //Setup GPIO to PT output function
    //GPIO P1.14 uses PT14

    //setup PT configuration
    mxc_pt_cfg_t ptConfig;
    ptConfig.channel = 14; //PT14
    ptConfig.bps = 2; //bit rate
    ptConfig.ptLength = 5; //bits
    ptConfig.pattern = 0x16;
    ptConfig.loop = 0; //continuous loop
    ptConfig.loopDelay = 0;

    MXC_PT_Config(PTG, &ptConfig);

    //start PT14
    MXC_PT_Start(PTG, MXC_F_PTG_ENABLE_PT14);
}

// *****************************************************************************
void SquareWave(void)
{
    //Setup GPIO to PT output function
    //GPIO P1.15 uses PT15

    uint32_t freq = 10; //Hz
    MXC_PT_SqrWaveConfig(PTG, 15, freq); //PT15

    //start PT15
    MXC_PT_Start(PTG, MXC_F_PTG_ENABLE_PT15);
}

// *****************************************************************************
int main(void)
{
    printf("\n*************** Pulse Train Demo ****************\n");

#if defined(BOARD_FTHR2)
    printf("P1.14 = Outputs continuous pattern of 10110b at 2bps\n");
    printf("P1.15 = Outputs 10Hz continuous square wave\n");
    printf("Push button 0 = Stop/Start all pulse trains\n");
#else
    printf("LED0 = Outputs continuous pattern of 10110b at 2bps\n");
    printf("LED1 = Outputs 10Hz continuous square wave\n");
    printf("Push button 0 = Stop/Start all pulse trains\n");
#endif
    //Setup push button to start/stop All pulse train
    PB_RegisterCallback(0, (pb_callback)PB_Start_Stop_handler);

    NVIC_EnableIRQ(PT_IRQn); //enabled default interrupt handler
    MXC_PT_EnableInt(PTG, ALL_PT); //enabled interrupts for all PT
    MXC_PT_Init(PTG, MXC_PT_CLK_DIV1); //initialize pulse trains

    //configure and start pulse trains
    ContinuousPulseTrain();
    SquareWave();

    while (1) {}
}
