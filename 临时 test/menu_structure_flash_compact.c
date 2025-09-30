// Auto-generated menu structure (Flash Storage - Compact)

#include "menu.h"

// Menu Statistics
#define MENU_TOTAL_NODES	11
#define MENU_MAX_DEPTH		2

// Compact Menu Node Array in Flash
const Node_t menu_nodes[MENU_TOTAL_NODES] = {
	{ "root", NODETYPE_DIR, &menu_nodes[1], NULL },
	{ "sets", NODETYPE_DIR, &menu_nodes[2], &menu_nodes[5] },
	{ "bright", NODETYPE_VAL, (void*)bright, &menu_nodes[3] },
	{ "openled", NODETYPE_FUN, (void*)openled, &menu_nodes[4] },
	{ "closeled", NODETYPE_FUN, (void*)closeled, NULL },
	{ "info", NODETYPE_DIR, &menu_nodes[6], &menu_nodes[9] },
	{ "version", NODETYPE_VAL, (void*)version, &menu_nodes[7] },
	{ "author", NODETYPE_VAL, (void*)author, &menu_nodes[8] },
	{ "email", NODETYPE_VAL, (void*)email, NULL },
	{ "H1", NODETYPE_DIR, &menu_nodes[10], NULL },
	{ "H1_1", NODETYPE_DIR, NULL, NULL },
};

const Node_t *const root = &menu_nodes[0];

