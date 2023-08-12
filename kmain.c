#include "display.h"
#include "mem.h"
#include "string.h"

unsigned long kmain() {
	clear_screen();

	char* s = kmalloc(10);
	memset(s, 10, 0);
	s[0] = 'h';
	s[1] = 'E';
	s[2] = 'L';
	s[3] = 'L';
	s[4] = 'o';
	putstr(0, 0, s);

	/*
	putstr(0, 22, "former line 22");
	putstr(0, 23, "former line 23");
	putstr(0, 24, "former line 24");
	memcpy((void*)(0xb8000 + 21 * 80 * 2), (void*)(0xb8000 + 22 * 80 * 2), 80 * 2);
	memcpy((void*)(0xb8000 + 22 * 80 * 2), (void*)(0xb8000 + 23 * 80 * 2), 80 * 2);
	memcpy((void*)(0xb8000 + 23 * 80 * 2), (void*)(0xb8000 + 24 * 80 * 2), 80 * 2);
	memset((void*)(0xb8000 + 24 * 80 * 2), 80 * 2, 0);
	*/
	//roll_put_str(":PPPP");
	roll_putstr("11111");
	roll_putstr("222222");
	roll_putstr("3.3.3.3.3.");


	int a = 72, b = 928347;
	void* objects[] = {&a, &b};

	putstr(0, 10, sprintf("n10={d}, n16=0x{x}", objects));

	putch(0, 24, 'x');
	putch(79, 24, 'x');
	putch(0, 0, 'x');
	putch(79, 0, 'x');

	
	return 0x12345678;
}
