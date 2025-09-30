#include "menu_basic.h"

menu_handle_t menu_handle;
Node_t *root;
Node_t N[20];
void node_init(void)
{
	// clang-format off
	NodeAlloc_init(N,20);
	root =
		SetNode(NODETYPE_DIR, "root", SetBranch(
			SetNode(NODETYPE_FUN, "LED_Turn", NULL), 
			SetNode(NODETYPE_FUN, "RESET", NULL), 
			SetNode(NODETYPE_DIR, "Speed", SetBranch(
				SetNode(NODETYPE_FUN, "SpeedUP", NULL),
				SetNode(NODETYPE_FUN, "SpeedUP", NULL)
			)), 
			SetNode(NODETYPE_FUN, "ha", NULL), 
			SetNode(NODETYPE_FUN, "ji", NULL)
		));
	// clang-format on
}

void menu_basic_init(void)
{
	memset(&menu_handle, 0, sizeof(menu_handle_t));
	menu_handle.screen_show_str = menu_interface_screen_show_str;
	menu_handle.screen_clear = menu_interface_screen_clear;
	node_init();
	menu_init(&menu_handle, root);
	menu_scan_page(&menu_handle);
	menu_handle.screen_clear();
	menu_show_path(&menu_handle);
	menu_show_page(&menu_handle);
	ssd1306_basic_update();
	// printf("menu page num %d\n", menu_handle.page_num);
	// root_scan(root);
}

void menu_basic_run(void)
{
	if (menu_handle.focus_line_state == 1)
	{
		menu_handle.screen_clear();
		menu_show_path(&menu_handle);
		menu_show_page(&menu_handle);
		ssd1306_basic_update();
		menu_handle.focus_line_state = 0;
	}
}
