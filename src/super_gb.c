#include <gb/cgb.h>
#include <gb/gb.h>
#include <stdint.h>
#include <gb/sgb.h>
#include <string.h>

#include "super_gb.h"

void sgb_pal_delay(void) {
	vsync();
	vsync();
	vsync();
	vsync();
}

uint8_t cmd_buf[16];

void sgb_transfer_pal(palette_color_t* pal) {
	memset(cmd_buf, 0, sizeof(cmd_buf));

	cmd_buf[0] = (SGB_PAL_01 << 3) | 1;
	cmd_buf[1] = pal[0] & 0xFF;
	cmd_buf[2] = pal[0] >> 8;
	cmd_buf[3] = pal[1] & 0xFF;
	cmd_buf[4] = pal[1] >> 8;
	cmd_buf[5] = pal[2] & 0xFF;
	cmd_buf[6] = pal[2] >> 8;
	cmd_buf[7] = pal[3] & 0xFF;
	cmd_buf[8] = pal[3] >> 8;

	sgb_transfer(cmd_buf);
}