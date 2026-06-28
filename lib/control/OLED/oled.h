#ifndef LCD_H
#define LCD_H

void oledInit();
void oledPrint(const char* msg, int size=1, int x=0, int y=0, bool clear=true, bool clear_self=false);
void oledClear();
void oledPrintTip(const char* msg,bool clear_self=false);

#endif