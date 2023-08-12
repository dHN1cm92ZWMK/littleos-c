#ifndef __DISPLAY_H__
#define __DISPLAY_H__

void putch(unsigned char x, unsigned char y, char c);
void putstr(unsigned char x, unsigned char y, const char *str);
void clear_screen(); 
void roll_putstr(const char *str); 

#endif
