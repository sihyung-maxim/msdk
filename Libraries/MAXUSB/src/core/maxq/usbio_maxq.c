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

#include <stdio.h>
#include <stdint.h>
#include "MAXQ1050.h"
#include "usbio_maxq.h"

/* Number of while() loop iterations before we time out on a read/write */
unsigned int param_tmo = 32;

/* Writes a value into the UADDR/UDATA indirect register interface */
/* Returns -1 on UADDR.BUSY=1 timeout, 0 otherwise */
int usbio_writereg(unsigned int reg, uint16_t data)
{
  unsigned int tmo = param_tmo;

  UADDR = reg;
  UDATA = data;

  while ((UADDR & 0x40) && --tmo) {
  }

  if (!tmo) {
    /* Timeout waiting for busy to clear */
    return -1;
  }

  return 0;
}

/** Writes a value into the UADDR/UDATA indirect register interface
 * Doesn't check UBUSY
 */
void usbio_blind_writereg(unsigned int reg, uint16_t data)
{
  UADDR = reg;
  UDATA = data;
}

/* Reads a value from the UADDR/UDATA indirect register interface */
/* Returns -1 on UADDR.BUSY=1 timeout, 0 otherwise */
int usbio_readreg(unsigned int reg, uint16_t *data)
{
  unsigned int tmo = param_tmo;

  UADDR = 0x80 | reg;
  while ((UADDR & 0x40) && --tmo) {
  }
  if (!tmo) {
    /* Timeout waiting for busy to clear */
    return -1;
  }

  *data = UDATA;

  return 0;
}

/* Reads num bytes from the UADDR/UDATA indirect register interface */
/* Returns -1 on UADDR.BUSY=1 timeout, 0 otherwise */
int usbio_readfifo(unsigned int reg, uint8_t *data, unsigned int num)
{
  int ret;
  unsigned int tmo = param_tmo;

  while ((UADDR & 0x40) && --tmo) {
  }
  if (!tmo) {
    /* Timeout waiting for busy to clear */
    return -1;
  }

  UADDR = 0x80 | reg;
  while (num--) {
    while ((UADDR & 0x40) && --tmo) {
    }
    if (!tmo) {
      /* Timeout waiting for busy to clear */
      return -1;
    }
    *data = UDATA;
    data++;
  }
  ret = 0;

  return ret;
}


/* Writes num bytes to the UADDR/UDATA indirect register interface */
/* Returns -1 on UADDR.BUSY=1 timeout, 0 otherwise */
int usbio_writefifo(unsigned int reg, uint8_t *data, unsigned int num)
{
  int ret;
  unsigned int tmo = param_tmo;

  while ((UADDR & 0x40) && --tmo) {
  }
  if (!tmo) {
    /* Timeout waiting for busy to clear */
    return -1;
  }

  UADDR = reg;
  while (num--) {
    UDATA = *data;
    data++;
    while ((UADDR & 0x40) && --tmo) {
    }

    if (!tmo) {
      /* Timeout waiting for busy to clear */
      return -1;
    }
  }
  ret = 0;

  return ret;
}

