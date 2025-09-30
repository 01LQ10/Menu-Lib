#ifndef MenuCore_H
#define MenuCore_H
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
	void (*screen_show_str)(int x, int y, char *str, int len, int color); // 显示字符串
	void (*screen_clear)(void);											  // 清屏
	// 屏幕参数
	int screen_x; // 宽
	int screen_y; // 高
	int font_x;
	int font_y;
	int page_line; // 页显示最大行数
	// 状态
	Node_t *stack[20];	   // 栈
	int stack_index;	   // 栈索引
	Node_t *page_buff[20]; // 页缓存
	int page_num;		   // 本页节点总数量 从1开始
	int focus_line;		   // 焦点行 从0开始
	int focus_line_state;  // 焦点行状态: 0:正常 1:选中 2:禁用
} menu_handle_t;

Node_t *NodeAlloc();
Node_t *set_node(Node_t *node, NodeType type, const char *name, void *pointer);
#define SetNode(type, name, pointer) set_node(NodeAlloc(), type, name, (void *)pointer)
Node_t *set_branch(Node_t *first_node, ...);
#define SetBranch(first_node, ...) set_branch(first_node, __VA_ARGS__ __VA_OPT__(,) NULL)

#endif // MenuCore_H
