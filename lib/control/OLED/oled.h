#ifndef LCD_H
#define LCD_H

void oledPrint(const char* msg, int size = 1, int x = 0, int y = 0);
void oledClear();

#endif