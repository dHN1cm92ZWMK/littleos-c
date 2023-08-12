#include "display.h"
#include "mem.h"

unsigned short *framebuffer_base = (unsigned short*)0xb8000;

void putch(unsigned char x, unsigned char y, char c) {
	framebuffer_base[x + y * 80] = 0x0200 | c;
}

void putstr(unsigned char x, unsigned char y, const char *str) {
	for (int i = 0; *str; i++, str++) {
		putch(x + i, y, *str);
	}
}

void clear_screen() {
	memset((void*)framebuffer_base, 80 * 24 * 2, 0);
}

void roll_putstr(const char *str) {
	for (int line=1; line <= 24; line++) {
		// todo improve, move whole block
		void *prev_line = (void*)(framebuffer_base + 80 * (line - 1));
		void *curr_line = (void*)(framebuffer_base + 80 * line);
		memcpy(prev_line, curr_line, 80);
	}
	putstr(0, 24, str);
}
