#include "menu_core.h"

/* ===============分配函数================ */

Node_t *NodeArr;
uint16_t NodeAllocHead, NodeArr_Num;

uint8_t NodeAlloc_init(Node_t *_NodeArr, uint16_t _NodeArr_Num)
{
	NodeArr_Num = _NodeArr_Num;
	NodeArr = _NodeArr;
	NodeAllocHead = 0;
	return 0;
}

Node_t *NodeAlloc(void)
{
	if (NodeAllocHead < NodeArr_Num)
	{
		NodeAllocHead++;
		return &(NodeArr[NodeAllocHead - 1]);
	}
	else
	{
		return NULL;
	}
}

Node_t *set_node(Node_t *node, NodeType type, const char *name, void *pointer)
{
	if (node == NULL)
	{
		return NULL;
	}
	node->type = type;
	node->name = name;
	node->pointer = pointer;

	node->nextNode = NULL;

	return node;
}

Node_t *set_branch(Node_t *first_node, ...)
{
	if (first_node == NULL)
	{
		return NULL;
	}

	va_list args;
	va_start(args, first_node);
	Node_t *currentNode = NULL;
	Node_t *LastNode = first_node;
	while (1)
	{
		currentNode = va_arg(args, Node_t *);
		LastNode->nextNode = currentNode;
		if (currentNode == NULL)
			break;
		LastNode = currentNode;
	}
	va_end(args);
	return first_node;
}

/* ===============分配函数================ */

/*================测试输出=====================*/
void print_level(int level)
{
	for (int i = 0; i < level; i++)
	{
		printf("\t");
	}
	printf("[%d]", level);
}
void print_type(NodeType type)
{
	switch (type)
	{
	case NODETYPE_DIR:
		printf("[DIR] ");
		break;
	case NODETYPE_FUN:
		printf("[FUN] ");
		break;
	case NODETYPE_VAL:
		printf("[VAL] ");
		break;
	default:
		printf("[UNN] ");
	}
}

void root_scan(Node_t *root)
{
	Node_t *stack[100] = {0};
	Node_t *now_node = root;
	int stack_level = 0;

	int i = 10;
	while (i-- && now_node != NULL)
	{
		print_level(stack_level);
		print_type(now_node->type);
		printf("%s\n", now_node->name);

		// 优先进入子节点
		if (now_node->type == NODETYPE_DIR && now_node->pointer != NULL)
		{
			stack[stack_level++] = now_node;
			now_node = (Node_t *)(now_node->pointer);
			continue;
		}

		// 尝试同级节点
		if (now_node->nextNode != NULL)
		{
			now_node = now_node->nextNode;
			continue;
		}

		// 回溯到有下一个兄弟节点的父节点
		while (stack_level > 0)
		{
			now_node = stack[--stack_level];
			if (now_node->nextNode != NULL)
			{
				now_node = now_node->nextNode;
				break;
			}
		}

		// 如果回溯到栈底且没有下一个节点，遍历结束
		if (stack_level == 0 && (now_node->nextNode == NULL || stack_level == 0))
		{
			break;
		}
	}
}

void menu_scan_page(menu_handle_t *menu)
{
	Node_t *node = menu->stack[menu->stack_index];
	int i = 0;
	if (node != NULL && node->type == NODETYPE_DIR)
	{
		if (node->pointer != NULL)
		{
			node = (Node_t *)(node->pointer);
			menu->page_buff[0] = node;
			while (node->nextNode != NULL)
			{
				i++;
				node = node->nextNode;
				menu->page_buff[i] = node;
			}
			menu->page_num = i + 1;
		}
		else
		{
			menu->page_num = 0;
		}
	}
}

/*================测试输出=====================*/

/*================菜单操作=====================*/

void menu_init(menu_handle_t *menu, Node_t *root)
{

	menu->screen_x = 128;
	menu->screen_y = 64;
	menu->font_x = 6;
	menu->font_y = 12;
	menu->stack_index = 0;
	menu->page_max_line = 4;
	menu->page_start_line = 0;
	menu->rootNode = root;
	menu->stack[0] = root;
	menu->stack_index = 0;
}

void menu_show_path(menu_handle_t *menu)
{
	if (!menu || !menu->stack)
	{
		printf("menu_show_path: menu or menu->stack is null\n");
		return;
	}

	uint8_t cnt = 0, len = menu->screen_x / menu->font_x;
	uint8_t i;
	char str[100] = "", str_len;
	for (i = menu->stack_index + 1; i > 0; i--)
	{
		str_len = strlen(menu->stack[i - 1]->name);
		cnt += str_len + 1;
		if (cnt > len)
		{
			if (cnt - str_len > len - 3)
			{
				i++;
			}
			break;
		}
	}
	if (i != 0)
	{
		strcat(str, ".../");
	}
	for (; i <= menu->stack_index; i++)
	{
		strcat(str, menu->stack[i]->name);
		strcat(str, "/");
	}
	menu->screen_show_str(0, 0, str, 1);
	// printf("%s\n", str);
}

void menu_show_page(menu_handle_t *menu)
{
	uint8_t line_num = menu->page_num - menu->page_start_line;
	if (line_num > menu->page_max_line)
		line_num = menu->page_max_line;
	for (int i = 0; i < line_num; i++)
	{
		if (menu->page_start_line + i == menu->focus_line)
		{
			// printf(">%s\n", menu->page_buff[menu->page_start_line + i]->name);
			menu->screen_show_str(0, menu->font_y * (i + 1), ">", 1);
		}
		else
		{
			// printf(" %s\n", menu->page_buff[menu->page_start_line + i]->name);
		}
		menu->screen_show_str(menu->font_x, menu->font_y * (i + 1),
							  (char *)(menu->page_buff[menu->page_start_line + i]->name), 1);
	}
}

void menu_show_line(menu_handle_t *menu)
{
}

void menu_opt(menu_handle_t *menu, char input)
{
	switch (input)
	{
	case 'd': // 下
		menu->focus_line++;
		if (menu->focus_line >= menu->page_num)
		{
			menu->focus_line = 0;
			menu->page_start_line = 0;
		}
		else if (menu->focus_line >= menu->page_start_line + menu->page_max_line)
		{
			menu->page_start_line++;
		}
		printf("focus_line:%d\n", menu->focus_line);
		printf("page_start_line:%d\n", menu->page_start_line);

		break;
	case 'u': // 上
		if (menu->focus_line == 0)
		{
			menu->focus_line = menu->page_num - 1;
			menu->page_start_line = menu->focus_line - menu->page_max_line + 1;
		}
		else
		{
			menu->focus_line--;
			if (menu->focus_line < menu->page_start_line)
				menu->page_start_line--;
		}

		printf("focus_line:%d\n", menu->focus_line);
		printf("page_start_line:%d\n", menu->page_start_line);
		break;
	case 'l': // 左：退出
		if (menu->stack_index > 0)
		{
			menu->focus_line = menu->stack_line[menu->stack_index];
			menu->stack_index--;
			menu_scan_page(menu);
		}
		break;
	case 'r': // 右：进入
		if (menu->page_buff[menu->focus_line]->pointer != NULL &&
			menu->page_buff[menu->focus_line]->type == NODETYPE_DIR)
		{
			menu->stack_index++;
			menu->stack[menu->stack_index] = menu->page_buff[menu->focus_line];
			menu->stack_line[menu->stack_index] = menu->focus_line;
			menu->focus_line = 0;
			menu_scan_page(menu);
		}
		break;
	}
}
