#ifndef _MENU_INTERFACE_H
#define _MENU_INTERFACE_H

#include "menu_core.h"

// 显示字符串
void menu_interface_screen_show_str(uint16_t x, uint16_t y,
									char *str, uint8_t color);
void menu_interface_screen_clear(void); // 清屏

#endif
