// Auto-generated menu structure

#include "MenuCore.h"

// Menu Statistics
#define MENU_TOTAL_NODES	11
#define MENU_MAX_DEPTH		2

Node_t *root = SetNode(NODETYPE_DIR, "root", SetBranch(
	SetNode(NODETYPE_DIR, "sets", SetBranch(
		SetNode(NODETYPE_VAL, "bright", bright), 
		SetNode(NODETYPE_FUN, "openled", openled), 
		SetNode(NODETYPE_FUN, "closeled", closeled)
	)), 
	SetNode(NODETYPE_DIR, "info", SetBranch(
		SetNode(NODETYPE_VAL, "version", version), 
		SetNode(NODETYPE_VAL, "author", author), 
		SetNode(NODETYPE_VAL, "email", email)
	)), 
	SetNode(NODETYPE_DIR, "H1", SetBranch(
		SetNode(NODETYPE_DIR, "H1_1", NULL)
	))
));
