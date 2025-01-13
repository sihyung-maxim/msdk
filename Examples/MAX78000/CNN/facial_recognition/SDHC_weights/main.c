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
 * @brief   read and write sdhc
 * @details This example uses the sdhc and ffat to read/write the file system on
 *          an SD card. The Fat library used supports long filenames (see ffconf.h)
 *          the max length is 256 characters.
 *
 *          You must connect an sd card to the sd card slot.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "mxc_delay.h"
#include "mxc_device.h"
#include "gpio.h"
#include "uart.h"
#include "weights_2.h"
#include "ff.h"

#ifdef BOARD_EVKIT_V1
#warning This example is not supported by the MAX78000EVKIT.
#endif

/***** Definitions *****/

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define MAXLEN 256

/***** Globals *****/
FATFS *fs; //FFat Filesystem Object
FATFS fs_obj;
FIL file; //FFat File Object
FRESULT err; //FFat Result (Struct)
FILINFO fno; //FFat File Information Object
DIR dir; //FFat Directory Object
TCHAR message[MAXLEN], directory[MAXLEN], cwd[MAXLEN], filename[MAXLEN], volume_label[24],
    volume = '0';
TCHAR *FF_ERRORS[20];
DWORD clusters_free = 0, sectors_free = 0, sectors_total = 0, volume_sn = 0;
UINT bytes_written = 0, bytes_read = 0, mounted = 0;
uint32_t total_bytes = 0;
BYTE work[4096];
static char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789,.-#'?!";
mxc_gpio_cfg_t SDPowerEnablePin = { MXC_GPIO1, MXC_GPIO_PIN_12, MXC_GPIO_FUNC_OUT,
                                    MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIO };

// Kernels:
static const uint32_t kernels[] = KERNELS;

/***** FUNCTIONS *****/
void generateMessage(unsigned length)
{
    for (int i = 0; i < length; i++) {
        /*Generate some random data to put in file*/
        message[i] = charset[rand() % (sizeof(charset) - 1)];
    }
}

int mount(void)
{
    fs = &fs_obj;

    if ((err = f_mount(fs, "", 1)) != FR_OK) { //Mount the default drive to fs now
        printf("Error opening SD card: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
    } else {
        printf("SD card mounted.\n");
        mounted = 1;
    }

    f_getcwd(cwd, sizeof(cwd)); //Set the Current working directory

    return err;
}

int umount(void)
{
    if ((err = f_mount(NULL, "", 0)) != FR_OK) { //Unmount the default drive from its mount point
        printf("Error unmounting volume: %s\n", FF_ERRORS[err]);
    } else {
        printf("SD card unmounted.\n");
        mounted = 0;
    }

    return err;
}

int formatSDHC(void)
{
    printf("\n\n*****THE DRIVE WILL BE FORMATTED IN 5 SECONDS*****\n");
    printf("**************PRESS ANY KEY TO ABORT**************\n\n");
    MXC_UART_ClearRXFIFO(MXC_UART0);
    MXC_Delay(MSEC(5000));

    if (MXC_UART_GetRXFIFOAvailable(MXC_UART0) > 0) {
        return E_ABORT;
    }

    printf("FORMATTING DRIVE\n");

    MKFS_PARM format_options = { .fmt = FM_FAT32 };

    if ((err = f_mkfs("", &format_options, work, sizeof(work))) !=
        FR_OK) { //Format the default drive to FAT32
        printf("Error formatting SD card: %s\n", FF_ERRORS[err]);
    } else {
        printf("Drive formatted.\n");
    }

    mount();

    if ((err = f_setlabel("MAXIM")) != FR_OK) {
        printf("Error setting drive label: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
    }

    umount();

    return err;
}

int getSize(void)
{
    if (!mounted) {
        mount();
    }

    if ((err = f_getfree(&volume, &clusters_free, &fs)) != FR_OK) {
        printf("Error finding free size of card: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
    }

    sectors_total = (fs->n_fatent - 2) * fs->csize;
    sectors_free = clusters_free * fs->csize;

    printf("Disk Size: %u bytes\n", sectors_total / 2);
    printf("Available: %u bytes\n", sectors_free / 2);

    return err;
}

int ls(void)
{
    if (!mounted) {
        mount();
    }

    printf("Listing Contents of %s - \n", cwd);

    if ((err = f_opendir(&dir, cwd)) == FR_OK) {
        while (1) {
            err = f_readdir(&dir, &fno);

            if (err != FR_OK || fno.fname[0] == 0) {
                break;
            }

            printf("%s/%s", cwd, fno.fname);

            if (fno.fattrib & AM_DIR) {
                printf("/");
            }

            printf("\n");
        }

        f_closedir(&dir);
    } else {
        printf("Error opening directory!\n");
        return err;
    }

    printf("\nFinished listing contents\n");

    return err;
}

int createFile(void)
{
    unsigned int length = 128;

    if (!mounted) {
        mount();
    }

    printf("Enter the name of the text file: \n");
    scanf("%255s", filename);
    printf("Enter the length of the file: (%d max)\n", MAXLEN);
    scanf("%d", &length);

    if (length > MAXLEN) {
        printf("Error. File size limit for this example is %d bytes.\n", MAXLEN);
        return FR_INVALID_PARAMETER;
    }

    printf("Creating file %s with length %d\n", filename, length);

    if ((err = f_open(&file, (const TCHAR *)filename, FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK) {
        printf("Error opening file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    printf("File opened!\n");

    generateMessage(length);

    if ((err = f_write(&file, &message, length, &bytes_written)) != FR_OK) {
        printf("Error writing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    printf("%d bytes written to file!\n", bytes_written);

    if ((err = f_close(&file)) != FR_OK) {
        printf("Error closing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    printf("File Closed!\n");
    return err;
}

int appendFile(void)
{
    unsigned int length = 0;

    if (!mounted) {
        mount();
    }

    printf("Enter name of file to append: \n");
    scanf("%255s", filename);
    printf("Enter length of random data to append: (%d max)\n", MAXLEN);
    scanf("%d", &length);

    if ((err = f_stat((const TCHAR *)filename, &fno)) == FR_NO_FILE) {
        printf("File %s doesn't exist!\n", (const TCHAR *)filename);
        return err;
    }

    if (length > MAXLEN) {
        printf("Error. Size limit for this example is %d bytes.\n", MAXLEN);
        return FR_INVALID_PARAMETER;
    }

    if ((err = f_open(&file, (const TCHAR *)filename, FA_OPEN_APPEND | FA_WRITE)) != FR_OK) {
        printf("Error opening file %s\n", FF_ERRORS[err]);
        return err;
    }

    printf("File opened!\n");

    generateMessage(length);

    if ((err = f_write(&file, &message, length, &bytes_written)) != FR_OK) {
        printf("Error writing file: %s\n", FF_ERRORS[err]);
        return err;
    }

    printf("%d bytes written to file\n", bytes_written);

    if ((err = f_close(&file)) != FR_OK) {
        printf("Error closing file: %s\n", FF_ERRORS[err]);
        return err;
    }

    printf("File closed.\n");
    return err;
}

int mkdir(void)
{
    if (!mounted) {
        mount();
    }

    printf("Enter directory name: \n");
    scanf("%255s", directory);

    err = f_stat((const TCHAR *)directory, &fno);

    if (err == FR_NO_FILE) {
        printf("Creating directory...\n");

        if ((err = f_mkdir((const TCHAR *)directory)) != FR_OK) {
            printf("Error creating directory: %s\n", FF_ERRORS[err]);
            f_mount(NULL, "", 0);
            return err;
        } else {
            printf("Directory %s created.\n", directory);
        }

    } else {
        printf("Directory already exists.\n");
    }

    return err;
}

int cd(void)
{
    if (!mounted) {
        mount();
    }

    printf("Directory to change into: \n");
    scanf("%255s", directory);

    if ((err = f_stat((const TCHAR *)directory, &fno)) == FR_NO_FILE) {
        printf("Directory doesn't exist (Did you mean mkdir?)\n");
        return err;
    }

    if ((err = f_chdir((const TCHAR *)directory)) != FR_OK) {
        printf("Error in chdir: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    printf("Changed to %s\n", directory);
    f_getcwd(cwd, sizeof(cwd));

    return err;
}

int delete (void)
{
    if (!mounted) {
        mount();
    }

    printf("File or directory to delete (always recursive!)\n");
    scanf("%255s", filename);

    if ((err = f_stat((const TCHAR *)filename, &fno)) == FR_NO_FILE) {
        printf("File or directory doesn't exist\n");
        return err;
    }

    if ((err = f_unlink(filename)) != FR_OK) {
        printf("Error deleting file\n");
        return err;
    }

    printf("Deleted file %s\n", filename);
    return err;
}

int example(void)
{
    unsigned int length = 256;

    if ((err = formatSDHC()) != FR_OK) {
        printf("Error Formatting SD Card: %s\n", FF_ERRORS[err]);
        return err;
    }

    //open SD Card
    if ((err = mount()) != FR_OK) {
        printf("Error opening SD Card: %s\n", FF_ERRORS[err]);
        return err;
    }

    printf("SD Card Opened!\n");

    if ((err = f_setlabel("MAXIM")) != FR_OK) {
        printf("Error setting drive label: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    if ((err = f_getfree(&volume, &clusters_free, &fs)) != FR_OK) {
        printf("Error finding free size of card: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    if ((err = f_getlabel(&volume, volume_label, &volume_sn)) != FR_OK) {
        printf("Error reading drive label: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    if ((err = f_open(&file, "0:HelloWorld.txt", FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK) {
        printf("Error opening file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    printf("File opened!\n");

    generateMessage(length);

    if ((err = f_write(&file, &message, length, &bytes_written)) != FR_OK) {
        printf("Error writing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    printf("%d bytes written to file!\n", bytes_written);

    if ((err = f_close(&file)) != FR_OK) {
        printf("Error closing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    printf("File Closed!\n");

    if ((err = f_chmod("HelloWorld.txt", 0, AM_RDO | AM_ARC | AM_SYS | AM_HID)) != FR_OK) {
        printf("Error in chmod: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    err = f_stat("MaximSDHC", &fno);

    if (err == FR_NO_FILE) {
        printf("Creating Directory...\n");

        if ((err = f_mkdir("MaximSDHC")) != FR_OK) {
            printf("Error creating directory: %s\n", FF_ERRORS[err]);
            f_mount(NULL, "", 0);
            return err;
        }
    }

    printf("Renaming File...\n");

    if ((err = f_rename("0:HelloWorld.txt", "0:MaximSDHC/HelloMaxim.txt")) !=
        FR_OK) { //cr: clearify 0:file notation
        printf("Error moving file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    if ((err = f_chdir("/MaximSDHC")) != FR_OK) {
        printf("Error in chdir: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    printf("Attempting to read back file...\n");

    if ((err = f_open(&file, "HelloMaxim.txt", FA_READ)) != FR_OK) {
        printf("Error opening file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    if ((err = f_read(&file, &message, bytes_written, &bytes_read)) != FR_OK) {
        printf("Error reading file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    printf("Read Back %d bytes\n", bytes_read);
    printf("Message: ");
    printf("%s", message);
    printf("\n");

    if ((err = f_close(&file)) != FR_OK) {
        printf("Error closing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        return err;
    }

    printf("File Closed!\n");

    //unmount SD Card
    //f_mount(fs, "", 0);
    if ((err = f_mount(NULL, "", 0)) != FR_OK) {
        printf("Error unmounting volume: %s\n", FF_ERRORS[err]);
        return err;
    }

    return 0;
}

void waitCardInserted(void)
{
    // On the MAX78000FTHR board, P0.12 will be pulled low when a card is inserted.
    mxc_gpio_cfg_t cardDetect;
    cardDetect.port = MXC_GPIO0;
    cardDetect.mask = MXC_GPIO_PIN_12;
    cardDetect.func = MXC_GPIO_FUNC_IN;
    cardDetect.pad = MXC_GPIO_PAD_NONE;
    cardDetect.vssel = MXC_GPIO_VSSEL_VDDIOH;

    MXC_GPIO_Config(&cardDetect);

    // Exit function if card is already inserted
    if (MXC_GPIO_InGet(MXC_GPIO0, MXC_GPIO_PIN_12) == 0) {
        return;
    }

    printf("Insert SD card to continue.\n");

    while (MXC_GPIO_InGet(MXC_GPIO0, MXC_GPIO_PIN_12) != 0) {
        // Spin waiting for card to be inserted.
    }

    // Card has been detected, exit the function.
}

/******************************************************************************/
int main(void)
{
    int i;
    int write_size = 3000; // in bytes
    uint8_t *wr_ptr;

    FF_ERRORS[0] = "FR_OK";
    FF_ERRORS[1] = "FR_DISK_ERR";
    FF_ERRORS[2] = "FR_INT_ERR";
    FF_ERRORS[3] = "FR_NOT_READY";
    FF_ERRORS[4] = "FR_NO_FILE";
    FF_ERRORS[5] = "FR_NO_PATH";
    FF_ERRORS[6] = "FR_INVLAID_NAME";
    FF_ERRORS[7] = "FR_DENIED";
    FF_ERRORS[8] = "FR_EXIST";
    FF_ERRORS[9] = "FR_INVALID_OBJECT";
    FF_ERRORS[10] = "FR_WRITE_PROTECTED";
    FF_ERRORS[11] = "FR_INVALID_DRIVE";
    FF_ERRORS[12] = "FR_NOT_ENABLED";
    FF_ERRORS[13] = "FR_NO_FILESYSTEM";
    FF_ERRORS[14] = "FR_MKFS_ABORTED";
    FF_ERRORS[15] = "FR_TIMEOUT";
    FF_ERRORS[16] = "FR_LOCKED";
    FF_ERRORS[17] = "FR_NOT_ENOUGH_CORE";
    FF_ERRORS[18] = "FR_TOO_MANY_OPEN_FILES";
    FF_ERRORS[19] = "FR_INVALID_PARAMETER";

    printf("\n\n***** " TOSTRING(TARGET) " Writes CNN weights to SD card *****\n");

    waitCardInserted();

    printf("Card inserted\n");

    f_getcwd(cwd, sizeof(cwd));

    err = 0;

    if ((err = mount()) != FR_OK) {
        printf("Error opening SD Card: %s\n", FF_ERRORS[err]);
        return err;
    }

    printf("SD Card Opened\n");

    if ((err = f_open(&file, "weights_2.bin", FA_CREATE_ALWAYS | FA_WRITE)) != FR_OK) {
        printf("ERROR opening file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1) {}
    }

    printf("Opened file 'weights_2.bin'\n");

    wr_ptr = (uint8_t *)kernels;
    // Get size of weights
    i = sizeof(kernels);
    printf("%d total bytes to write\n", i);

    while (i > 0) {
        if ((err = f_write(&file, wr_ptr, write_size, &bytes_written)) != FR_OK) {
            printf("ERROR writing file %d: %s\n", i, FF_ERRORS[err]);
            f_mount(NULL, "", 0);
            while (1) {}
        }

        // Calculate residual bytes
        i -= write_size;

        if (i < write_size)
            write_size = i;

        wr_ptr += bytes_written;
        total_bytes += bytes_written;
        printf("%d bytes written to file\n", bytes_written);
    }

    printf("%d total bytes written to file\n", total_bytes);

    if ((err = f_close(&file)) != FR_OK) {
        printf("ERROR closing file: %s\n", FF_ERRORS[err]);
        f_mount(NULL, "", 0);
        while (1) {}
    }

    printf("File Closed\n");

    if ((err = f_mount(NULL, "", 0)) != FR_OK) {
        printf("ERROR unmounting volume: %s\n", FF_ERRORS[err]);
        while (1) {}
    }

    printf("SD Unmonted\n");

    return 0;
}
