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

#include <stddef.h>
#include <stdlib.h>
#include "board.h"
#include "mxc_device.h"
#include "mxc_assert.h"
#include "pb.h"

// State array for holding PB polarity settings
static pb_polarity_t *g_pb_polarity = NULL;

/******************************************************************************/
int PB_Init(void)
{
    int retval = E_NO_ERROR;
    unsigned int i;

    // Enable pushbutton inputs
    for (i = 0; i < num_pbs; i++) {
        if (MXC_GPIO_Config(&pb_pin[i]) != E_NO_ERROR) {
            retval = E_UNKNOWN;
        }
    }

    if (g_pb_polarity == NULL) {
        g_pb_polarity = malloc(num_pbs * sizeof(pb_polarity_t));
        // Note we have to use malloc here because we decided to make "num_pbs" a
        // const variable instead of a compiler definition...
    }

    for (unsigned int i = 0; i < num_pbs; i++) {
        g_pb_polarity[i] = PB_POLARITY_LOW;
    }

    return retval;
}

/******************************************************************************/
int PB_Set_Polarity(unsigned int pb, pb_polarity_t polarity)
{
    if (pb >= num_pbs) {
        return E_BAD_PARAM;
    }

    g_pb_polarity[pb] = polarity;
    return E_NO_ERROR;
}

/******************************************************************************/
int PB_RegisterCallback(unsigned int pb, pb_callback callback)
{
    if (pb >= num_pbs) {
        return E_BAD_PARAM;
    }

    if (callback) {
        // Register callback
        MXC_GPIO_RegisterCallback(&pb_pin[pb], callback, (void *)pb);

        // Configure and enable interrupt
        mxc_gpio_int_pol_t interrupt_polarity = MXC_GPIO_INT_FALLING;
        if (g_pb_polarity[pb] == PB_POLARITY_HIGH) {
            interrupt_polarity = MXC_GPIO_INT_RISING;
        }
        MXC_GPIO_IntConfig(&pb_pin[pb], interrupt_polarity);
        MXC_GPIO_EnableInt(pb_pin[pb].port, pb_pin[pb].mask);
        NVIC_EnableIRQ((IRQn_Type)MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(pb_pin[pb].port)));
    } else {
        // Disable interrupt and clear callback
        MXC_GPIO_DisableInt(pb_pin[pb].port, pb_pin[pb].mask);
        MXC_GPIO_RegisterCallback(&pb_pin[pb], NULL, NULL);
    }

    return E_NO_ERROR;
}

/******************************************************************************/
int PB_RegisterCallbackRiseFall(unsigned int pb, pb_callback callback)
{
    if (pb >= num_pbs) {
        return E_BAD_PARAM;
    }

    if (callback) {
        // Register callback
        MXC_GPIO_RegisterCallback(&pb_pin[pb], callback, (void *)&pb_pin[pb]);

        // Configure and enable interrupt
        MXC_GPIO_IntConfig(&pb_pin[pb], MXC_GPIO_INT_BOTH);
        MXC_GPIO_EnableInt(pb_pin[pb].port, pb_pin[pb].mask);
        NVIC_EnableIRQ((IRQn_Type)MXC_GPIO_GET_IRQ(MXC_GPIO_GET_IDX(pb_pin[pb].port)));
    } else {
        // Disable interrupt and clear callback
        MXC_GPIO_DisableInt(pb_pin[pb].port, pb_pin[pb].mask);
        MXC_GPIO_RegisterCallback(&pb_pin[pb], NULL, NULL);
    }

    return E_NO_ERROR;
}

//******************************************************************************
void PB_IntEnable(unsigned int pb)
{
    MXC_ASSERT(pb < num_pbs);
    MXC_GPIO_EnableInt(pb_pin[pb].port, pb_pin[pb].mask);
}

//******************************************************************************
void PB_IntDisable(unsigned int pb)
{
    MXC_ASSERT(pb < num_pbs);
    MXC_GPIO_DisableInt(pb_pin[pb].port, pb_pin[pb].mask);
}

//******************************************************************************
void PB_IntClear(unsigned int pb)
{
    MXC_ASSERT(pb < num_pbs);
    MXC_GPIO_ClearFlags(pb_pin[pb].port, pb_pin[pb].mask);
}

//******************************************************************************
int PB_Get(unsigned int pb)
{
    MXC_ASSERT(pb < num_pbs);
    uint32_t state = MXC_GPIO_InGet(pb_pin[pb].port, pb_pin[pb].mask);
    if (g_pb_polarity[pb] == PB_POLARITY_LOW) {
        state = !state;
    }
    return state;
}

//******************************************************************************
int PB_IsPressedAny(void)
{
    int i = 0;

    for (i = 0; i < num_pbs; i++) {
        if (PB_Get(i)) {
            return 1;
        }
    }

    return 0;
}
