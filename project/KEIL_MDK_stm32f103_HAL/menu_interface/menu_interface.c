#include "menu_interface.h"


// 显示字符串
void menu_interface_screen_show_str(uint16_t x, uint16_t y,
									char *str, uint8_t color)
{
	ssd1306_basic_string(x, y, str, strlen(str));
}
void menu_interface_screen_clear(void)
{
	ssd1306_basic_clear();
}





