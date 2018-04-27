/*
 * 
 * This file is part of pyA20Lime.
 * gpio_lib.c is python GPIO extension.
 * 
 * Copyright (c) 2014 Stefan Mavrodiev @ OLIMEX LTD, <support@olimex.com> 
 * 
 * pyA20Lime is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */


#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <pthread.h>
#include <unistd.h>
#include <sched.h>

static void *SUNXI_PIO_BASE = 0;
static void *SUNXI_PIO_BASE_LM = 0;

#include "gpio_lib.h"

int sunxi_gpio_init(void) {
	int fd;
	uint32_t PageSize, PageMask;
	uint32_t addr_start, addr_offset;

	if (SUNXI_PIO_BASE != 0)
		return 0;

	if ((fd = open("/dev/mem", O_RDWR | O_SYNC)) < 0) // O_RDWR only
		return -1;

	PageSize = sysconf(_SC_PAGESIZE);
	PageMask = (~(PageSize - 1));

	addr_start = SW_PORTC_IO_BASE & PageMask;
	addr_offset = SW_PORTC_IO_BASE & ~PageMask;
	SUNXI_PIO_BASE = mmap(0, PageSize * 2, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr_start);
	if (SUNXI_PIO_BASE != MAP_FAILED)
		SUNXI_PIO_BASE += addr_offset;

	addr_start = SW_PORTC_IO_BASE_LM & PageMask;
	addr_offset = SW_PORTC_IO_BASE_LM & ~PageMask;
	SUNXI_PIO_BASE_LM = mmap(0, PageSize * 2, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr_start);
	if (SUNXI_PIO_BASE_LM != MAP_FAILED)
		SUNXI_PIO_BASE_LM += addr_offset;
	else
		SUNXI_PIO_BASE_LM = 0;

	close(fd);
    return 0;
}

int sunxi_gpio_set_cfgpin(uint32_t pin, uint8_t val)
{
	uint32_t cfg;
	uint32_t bank = GPIO_BANK(pin);
	uint32_t index = GPIO_CFG_INDEX(pin);
	uint32_t offset = GPIO_CFG_OFFSET(pin);
	struct sunxi_gpio *pio;

	if (SUNXI_PIO_BASE == 0)
		return -1;

	if (bank == 11) {
		if (SUNXI_PIO_BASE_LM == 0) return -1;
		pio = &((struct sunxi_gpio_reg *)SUNXI_PIO_BASE_LM)->gpio_bank[0];
	} else pio = &((struct sunxi_gpio_reg *)SUNXI_PIO_BASE)->gpio_bank[bank];

	cfg = *(&pio->cfg[0] + index);
	cfg &= ~(0xf << offset);
	cfg |= val << offset;

	*(&pio->cfg[0] + index) = cfg;

	return 0;
}

int sunxi_gpio_get_cfgpin(uint32_t pin)
{
	uint32_t cfg;
	uint32_t bank = GPIO_BANK(pin);
	uint32_t index = GPIO_CFG_INDEX(pin);
	uint32_t offset = GPIO_CFG_OFFSET(pin);
	struct sunxi_gpio *pio;

	if (SUNXI_PIO_BASE == 0)
		return -1;

	if (bank == 11) {
		if (SUNXI_PIO_BASE_LM == 0) return -1;
		pio = &((struct sunxi_gpio_reg *)SUNXI_PIO_BASE_LM)->gpio_bank[0];
	} else pio = &((struct sunxi_gpio_reg *)SUNXI_PIO_BASE)->gpio_bank[bank];

	cfg = *(&pio->cfg[0] + index);
	cfg >>= offset;

	return (cfg & 0xf);
}

int sunxi_gpio_output(uint32_t pin, uint8_t val)
{
	uint32_t bank = GPIO_BANK(pin);
	uint32_t num = GPIO_NUM(pin);
	struct sunxi_gpio *pio;

	if (SUNXI_PIO_BASE == 0)
		return -1;
		
	if (bank == 11) {
		if (SUNXI_PIO_BASE_LM == 0) return -1;
		pio = &((struct sunxi_gpio_reg *)SUNXI_PIO_BASE_LM)->gpio_bank[0];
	} else pio = &((struct sunxi_gpio_reg *)SUNXI_PIO_BASE)->gpio_bank[bank];

	if (val)
		*(&pio->dat) |= 1 << num;
	else
		*(&pio->dat) &= ~(1 << num);

	return 0;
}

int sunxi_gpio_pullup(uint32_t pin, uint8_t pull)
{
	uint32_t cfg;
	uint32_t bank = GPIO_BANK(pin);
	uint32_t index = GPIO_PUL_INDEX(pin);
	uint32_t offset = GPIO_PUL_OFFSET(pin);
	struct sunxi_gpio *pio;

	if (SUNXI_PIO_BASE == 0)
		return -1;

	if (bank == 11) {
		if (SUNXI_PIO_BASE_LM == 0) return -1;
		pio = &((struct sunxi_gpio_reg *)SUNXI_PIO_BASE_LM)->gpio_bank[0];
	} else pio = &((struct sunxi_gpio_reg *)SUNXI_PIO_BASE)->gpio_bank[bank];

	cfg = *(&pio->pull[0] + index);
	cfg &= ~(0x3 << offset);
	cfg |= pull << offset;

	*(&pio->pull[0] + index) = cfg;

	return 0;
}

int sunxi_gpio_input(uint32_t pin)
{
	uint32_t dat;
	uint32_t bank = GPIO_BANK(pin);
	uint32_t num = GPIO_NUM(pin);
	struct sunxi_gpio *pio;

	if (SUNXI_PIO_BASE == 0)
		return -1;

	if (bank == 11) {
		if (SUNXI_PIO_BASE_LM == 0) return -1;
		pio = &((struct sunxi_gpio_reg *)SUNXI_PIO_BASE_LM)->gpio_bank[0];
	} else pio = &((struct sunxi_gpio_reg *)SUNXI_PIO_BASE)->gpio_bank[bank];

	dat = *(&pio->dat);
	dat >>= num;

	return (dat & 0x1);
}

void sunxi_gpio_close(void)
{
	uint32_t PageSize = sysconf(_SC_PAGESIZE);

	if (SUNXI_PIO_BASE != 0)
		munmap(SUNXI_PIO_BASE, PageSize * 2);

	if (SUNXI_PIO_BASE_LM != 0)
		munmap(SUNXI_PIO_BASE_LM, PageSize * 2);

	SUNXI_PIO_BASE = 0;
	SUNXI_PIO_BASE_LM = 0;
}