#ifndef _MENU_CORE_H
#define _MENU_CORE_H

#include "menu_config.h"
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

typedef enum
{
	NODETYPE_FUN = 0, // 执行函数
	NODETYPE_DIR = 1, // 目录
	NODETYPE_VAL = 2  // 值
} NodeType;

typedef enum
{
	Zero_Error = -1,
	Node_OutOfRange = 0,
	NodeBranch_OutOfRange = 1
} NodeAllocError;

typedef struct Node_s Node_t;
struct Node_s
{
	const char *name; // 名称
	NodeType type;	  // 类型
	void *pointer;	  // 数据 or 函数指针 or 子节点指针

	Node_t *nextNode; // 同级节点
};

typedef struct
{
	// 数据结构
	Node_t *rootNode; // 根节点
	// 屏幕显示接口
	void (*screen_show_str)(uint16_t x, uint16_t y, char *str, uint8_t color); // 显示字符串
	void (*screen_clear)(void);												   // 清屏
	void (*node_init)(void);
	// 屏幕参数
	uint16_t screen_x; // 宽
	uint16_t screen_y; // 高
	uint8_t font_x;
	uint8_t font_y;
	uint8_t page_max_line;	 // 页显示最大行数
	uint8_t page_start_line; // 页开始行数 从0开始
	// 状态
	Node_t *stack[20];		  // 栈
	uint8_t stack_line[20];	  // 历史行记录
	uint8_t stack_index;	  // 栈索引
	Node_t *page_buff[20];	  // 页缓存
	uint8_t page_num;		  // 本页节点总数量 从1开始
	uint8_t focus_line;		  // 焦点行 从0开始
	uint8_t focus_line_state; // 焦点行状态: 0:正常 1:选中 2:禁用
} menu_handle_t;

/* 节点分配 */
uint8_t NodeAlloc_init(Node_t *_NodeArr, uint16_t _NodeArr_Num);
Node_t *NodeAlloc(void);
Node_t *set_node(Node_t *node, NodeType type, const char *name, void *pointer);
#define SetNode(type, name, pointer) set_node(NodeAlloc(), type, name, (void *)pointer)
Node_t *set_branch(Node_t *first_node, ...);
// #define SetBranch(first_node, ...) set_branch(first_node, __VA_ARGS__ , NULL)
#define SetBranch(first_node, ...) set_branch(first_node, __VA_ARGS__, NULL)

void root_scan(Node_t *root);

void menu_init(menu_handle_t *menu, Node_t *root);
void menu_scan_page(menu_handle_t *menu);
void menu_show_path(menu_handle_t *menu);
void menu_show_page(menu_handle_t *menu);
void menu_opt(menu_handle_t *menu, char input);

#endif // MenuCore_H
