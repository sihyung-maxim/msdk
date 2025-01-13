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
 * @file    main.c
 * @brief   SPI Usecase!
 * @details This example demonstrate how to use SPI master slave feature.
 */

/*******************************      INCLUDES    ****************************/
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "spi_config.h"

/*******************************      DEFINES     ****************************/

/******************************* Type Definitions ****************************/

/*******************************     Variables    ****************************/

/******************************* Static Functions ****************************/
static void hex_dump(const char *title, unsigned char *buf, unsigned int len)
{
    unsigned int i;

    if (title) {
        printf("%s", title);
    }

    for (i = 0; i < len; i++) {
        if (!(i % 16)) {
            printf("\n");
        }
        printf("%02X ", buf[i]);
    }
    printf("\n");
}

int main(void)
{
    int ret = 0;
    unsigned int i;
    unsigned char tx_buff[TEST_BUFF_SIZE] = { 0 };
    unsigned char rx_buff[TEST_BUFF_SIZE] = { 0 };
    unsigned char expect_packet[TEST_BUFF_SIZE] = { 0 };
    unsigned int len;

    printf("---------------------------------------------------------------------------------------"
           "--------\n");
    printf("SPI%d is configured as master\n", MXC_SPI_GET_IDX(SPIx_MASTER));
    printf("SPI%d is configured as slave\n", MXC_SPI_GET_IDX(SPIx_SLAVE));
    printf("Please use jumper to connect these two SPI ports:\n");
    printf("\tMISO: P0.2 <--> P0.14\n");
    printf("\tMOSI: P0.3 <--> P0.15\n");
    printf("\tCLK : P0.4 <--> P0.16\n");
    printf("\tSS  : P0.5 <--> P0.17\n");
    printf("\n");
    printf(
        "This example will send some test byte from master to slave then from slave to master\n");
    printf("---------------------------------------------------------------------------------------"
           "--------\n\n");

    for (i = 0; i < TEST_BUFF_SIZE; i++) {
        tx_buff[i] = i + 1;
        expect_packet[i] = 0xFF - i;
    }

    ret = spi_slave_init();
    if (ret) {
        return ret;
    }

    ret = spi_master_init();
    if (ret) {
        return ret;
    }

    hex_dump("Master Send Packet", tx_buff, TEST_BUFF_SIZE);
    hex_dump("\nMaster Expect Packet", expect_packet, TEST_BUFF_SIZE);
    printf("-------------------------------------------------------\n");

    spi_slave_send(expect_packet, TEST_BUFF_SIZE);

    ret = spi_master_send_rcv(tx_buff, TEST_BUFF_SIZE, rx_buff);
    if (ret) {
        printf("\nSPI Master Send Failed! (0x%X)\n", ret);
        return ret;
    }
    hex_dump("\nMaster Receive:", rx_buff, TEST_BUFF_SIZE);

    if (memcmp(rx_buff, expect_packet, TEST_BUFF_SIZE) == 0) {
        printf("\nMaster Send Receive Succeeded\n");
    } else {
        printf("\nMaster Read Failed!\n");
    }

    memset(rx_buff, 0, sizeof(rx_buff));

    spi_slave_rcv(rx_buff, TEST_BUFF_SIZE, &len);
    hex_dump("\nSlave Receive:", rx_buff, len);

    if (memcmp(rx_buff, tx_buff, TEST_BUFF_SIZE) == 0) {
        printf("\nSlave Send Receive Succeeded\n");
    } else {
        printf("\nSlave Read Failed!\n");
        ret = -1;
    }

    printf("\nEnd of Example!\n");

    return ret;
}
