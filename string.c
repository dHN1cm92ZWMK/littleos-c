#include "string.h"
#include "mem.h"

const char* itoa(unsigned long number, unsigned char base) {
	int len = 0;
	for (int x = number; x; len++, x/=base); 

	char *str = kmalloc(len + 1);
	memset(str, len + 1, 0);
	for (int digit=0, rem = number; digit < len; digit++, rem /= base) {
		int val = rem % base;
		str[len - digit - 1] = (val < 10) ? '0' + val : 'a' + val - 10;
	}

	return str;
}


int strlen(const char *str) {
	int len = 0;
	for (; *str; len++, str++);
	return len;
}

const char* sprintf(const char *format, void **objects) {
	char *target = kmalloc(100);
	memset(target, 100, 0);


	for(int param=0, tidx=0;*format; format++) {
		if (*format == '{') {
			format++;
			const char *c = 0;
			int len = 0;
			switch (*format) {
				case 'x':
					c = itoa(*(unsigned int*)objects[param++], 16);
					len = strlen(c);
					memcpy(target + tidx, c, len);
					tidx+=len;
					// todo free
					break;
				case 'd':
					c = itoa(*(unsigned int*)objects[param++], 10);
					len = strlen(c);
					memcpy(target + tidx, c, len);
					tidx+=len;
					// todo free
					break;
				default: 
					break;
			}
			format++; // assert }
		} else
			target[tidx++] = *format;
	}

	return target;

	/*

	int discard = format[0];
	void *first = objects[0];
	void *second = objects[1];

	discard--;

	itoa(*(int*)first, 10);

	return itoa(*(int*)second, 10);
	*/
}

