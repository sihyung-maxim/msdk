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

/*******************************      INCLUDES    ****************************/
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#include "uart.h"
#include "terminal.h"

/*******************************      DEFINES     ****************************/
#define PC_COM_PORT MXC_UART1

/******************************* Type Definitions ****************************/

/*******************************    Variables   ****************************/

/******************************* Static Functions ****************************/

/******************************* Public Functions ****************************/
int terminal_init(void)
{
    int ret = 0;

    ret = MXC_UART_Init(PC_COM_PORT, 115200, MAP_A);

    return ret;
}

int terminal_read_num(unsigned int timeout)
{
    (void)timeout;
    int key;
    int num = 0;

    while (1) {
        key = MXC_UART_ReadCharacter(PC_COM_PORT);

        if (key > 0) {
            //echo
            MXC_UART_WriteCharacter(PC_COM_PORT, (unsigned char)key);

            if ((key >= '0') && (key <= '9')) {
                num = num * 10 + (key - '0');
            }

            if ((key == '\n') || (key == '\r')) {
                break;
            } else if (key == 0x1B) { // Escape char 0x1B = 27
                num = -1;
                break;
            }
        }
    }

    MXC_UART_ClearRXFIFO(PC_COM_PORT);

    return num;
}

int terminal_select_from_list(const char *title, const list_t *items, int nb_items, int nb_col)
{
    int i, k;
    int key = 0;
    int nb_row;
    char buf[512];
    char item_data[64];
    int index;

    if (title) {
        terminal_printf("\r\n\r\n%s\r\n", title);
    }
    terminal_printf("---------------------------------------------------------\r\n");

    if (nb_col == 0) {
        nb_col = 1;
    }

    nb_row = nb_items / nb_col;

    if (nb_items % 2) {
        nb_row++;
    }

    if (nb_items > 1) {
        for (i = 0; i < nb_row; i++) {
            buf[0] = '\0';

            for (k = 0; k < nb_col; k++) {
                index = i + (k * nb_row);

                if (index < nb_items) {
                    snprintf(item_data, sizeof(item_data), "%-3d- %-32s ", index + 1,
                             items[index].name);
                    strncat(buf, item_data, sizeof(buf) - 1);
                }
            }
            //
            terminal_printf("%s\r\n", buf);
        }

        terminal_printf("\r\nPlease select: ");
        key = terminal_read_num(0);
        terminal_printf("\r\n");
    } else if (nb_items == 1) {
        key = 1;
    }

    if ((key > 0) && (key <= nb_items)) {
        if (items[key - 1].callback) {
            int ret;
            ret = items[key - 1].callback(items[key - 1].name); // index start from 0
            if (ret == 0) {
                terminal_printf("\r\n%s: SUCCESS\r\n\r\n", items[key - 1].name);
            } else {
                terminal_printf("\r\n%s: FAILURE\r\n\r\n", items[key - 1].name);
            }
        }
    } else if (key == -1) {
        key = KEY_CANCEL;
    } else {
        terminal_printf("ERR:Invalid input\r\n\r\n");
    }

    return key;
}

int terminal_printf(const char *format, ...)
{
    char buffer[512];
    int len;

    __gnuc_va_list args;
    va_start(args, format);
    len = vsnprintf(buffer, sizeof(buffer), format, args);
    if (len > 0) {
        MXC_UART_Write(PC_COM_PORT, (uint8_t *)buffer, &len);
    }
    va_end(args);

    return len;
}

void terminal_hexdump(const char *title, char *buf, unsigned int len)
{
    unsigned int i;

    if (title) {
        terminal_printf("%s", title);
    }

    /* Print buffer bytes */
    for (i = 0; i < len; i++) {
        if (!(i % 16)) {
            terminal_printf("\r\n");
        }
        terminal_printf("%02X ", buf[i]);
    }

    terminal_printf("\r\n");
}
