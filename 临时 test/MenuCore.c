#include "MenuCore.h"
#include <stdio.h>

Node_t *NodeArr;
static int16_t NodeAllocHead = 0, NodeArr_Num;

uint8_t NodeAlloc_init(Node_t _NodeArr[], int16_t _NodeArr_Num)
{
	NodeArr_Num = _NodeArr_Num;
	printf("NodeAlloc_init: %d\n", NodeArr_Num);
	NodeArr = _NodeArr;
	NodeAllocHead = 0;
}

Node_t *NodeAlloc()
{
	if (NodeAllocHead < NodeArr_Num)
	{
		NodeAllocHead++;
		return &(NodeArr[NodeAllocHead - 1]);
		// return (NodeArr + NodeAllocHead - 1);
	}
	else
	{
		printf("NodeAlloc Error: NodeArr_Num is out of range\n");
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

void menu_show_path(menu_handle_t *menu)
{
	if (!menu || !menu->stack || menu->stack_index < 0)
		return;

	uint8_t cnt = 0, len = 15;//menu->screen_x / menu->font_x;
	uint8_t i;
	char str[100], str_len;
	for (i = menu->stack_index + 1; i > 0; i--)
	{
		str_len = strlen(menu->stack[i - 1]->name);
		cnt +=  str_len + 1;
		if (cnt > len)
		{
			// i -= 1;
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
	printf("long %u\n",strlen(str));
	printf("%s\n", str);
	// screen_show_str(0, 0, str, strlen(str), 1);
}

void menu_show(menu_handle_t *menu)
{
	for (int i = 0; i < menu->page_num; i++)
	{
		if (i == menu->focus_line)
			printf(">%s\n", menu->page_buff[i]->name);
		else
			printf(" %s\n", menu->page_buff[i]->name);
	}
}

void menu_init(menu_handle_t *menu, Node_t *root)
{
	memset(menu, 0, sizeof(menu_handle_t));
	menu->screen_x = 128;
	menu->screen_y = 64;
	menu->font_x = 5;
	menu->font_y = 8;
	menu->stack_index = 0;
	menu->page_line = 5;
	menu->rootNode = root;
	menu->stack[0] = root;
	menu->stack_index = 0;
}

void menu_opt(menu_handle_t *menu, char input)
{
	switch (input)
	{
	case 's': // 下
		menu->focus_line++;
		if (menu->focus_line >= menu->page_num)
			menu->focus_line = 0;
		break;
	case 'w': // 上
		if (menu->focus_line <= 0)
			menu->focus_line = menu->page_num - 1;
		else
			menu->focus_line--;
		break;
	case 'a': // 左：退出
		if (menu->stack_index > 0)
		{
			menu->stack_index--;
			menu_scan_page(menu);
			// menu->focus_line = menu->stack[menu->stack_index];
		}
		break;
	case 'd': // 右：进入
		if (menu->page_buff[menu->focus_line]->pointer != NULL &&
			menu->page_buff[menu->focus_line]->type == NODETYPE_DIR)
		{
			menu->stack[++menu->stack_index] = menu->page_buff[menu->focus_line];
			menu_scan_page(menu);
		}
		break;
	}
}

// Menu Statistics
#define MENU_TOTAL_NODES 11
#define MENU_MAX_DEPTH 2

// Compact Menu Node Array in Flash
Node_t menu_nodes[MENU_TOTAL_NODES] = {
	{"root", NODETYPE_DIR, &menu_nodes[1], NULL},
	{"sets", NODETYPE_DIR, &menu_nodes[2], &menu_nodes[5]},
	{"bright", NODETYPE_VAL, (void *)NULL, &menu_nodes[3]},
	{"openled", NODETYPE_FUN, (void *)NULL, &menu_nodes[4]},
	{"closeled", NODETYPE_FUN, (void *)NULL, NULL},
	{"info", NODETYPE_DIR, &menu_nodes[6], &menu_nodes[9]},
	{"version", NODETYPE_VAL, (void *)NULL, &menu_nodes[7]},
	{"author", NODETYPE_VAL, (void *)NULL, &menu_nodes[8]},
	{"email", NODETYPE_VAL, (void *)NULL, NULL},
	{"H1", NODETYPE_DIR, &menu_nodes[10], NULL},
	{"H1_1", NODETYPE_DIR, NULL, NULL},
};

Node_t *root1 = &menu_nodes[0];

int main(void)
{
	menu_handle_t menu;
	char input;
	printf("Hello World\n");
	// clang-format off
	
	static Node_t N[20];
	NodeAlloc_init(N,20);
	Node_t *root =
		SetNode(NODETYPE_DIR, "root", SetBranch(
			SetNode(NODETYPE_DIR, "HH1", SetBranch(
				SetNode(NODETYPE_DIR, "HH1-1", SetBranch(
					SetNode(NODETYPE_DIR, "HH1-1-1", SetBranch(
						SetNode(NODETYPE_DIR, "HH1-1-1-1", SetBranch(
							SetNode(NODETYPE_FUN, "LED_Turn", NULL)
						))
					))
				))
			)), 
			SetNode(NODETYPE_FUN, "LED_Turn", NULL), 
			SetNode(NODETYPE_FUN, "RESET", NULL), 
			SetNode(NODETYPE_DIR, "Speed", SetBranch(
				SetNode(NODETYPE_FUN, "SpeedUP", NULL)
			)), 
			SetNode(NODETYPE_FUN, "ha", NULL), 
			SetNode(NODETYPE_FUN, "ji", NULL)
		));
	// clang-format on
	// root_scan(root);
	menu_init(&menu, root);
	menu_scan_page(&menu);
	menu_show_path(&menu);
	menu_show(&menu);

	while (1)
	{
		input = getchar();
		if (input == 'q')
			break;
		if (input == '\n')
			continue;
		menu_opt(&menu, input);
		menu_show_path(&menu);
		menu_show(&menu);
	}

	return 0;
}
