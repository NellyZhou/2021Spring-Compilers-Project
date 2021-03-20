#include "SyntaxTree.h"


TreeNode * create_TreeNode(char name[], int node_type, int line,  int value_type, char value[]){
	TreeNode *ptr = malloc(sizeof(TreeNode));
	strcpy(ptr->name, name);
	ptr->node_type = node_type;
	if (node_type == __SYMBOL_TYPE__){
		ptr->line = line;
	} else if (node_type == __TOKEN_TYPE__){
		ptr->value_type = value_type;
		switch (value_type){
			case INT_VALUE: ptr->int_val = atoi(value); break;
			case FLOAT_VALUE: ptr->float_val = atof(value); break;
			case TYPE_VALUE: strcpy(ptr->type_val, value); break;
			case ID_VALUE: strcpy(ptr->id_val, value); break;
			case OCT_VALUE: ptr->int_val = (int)strtol(value, NULL, 8); break;
			case HEX_VALUE: ptr->int_val = (int)strtol(value, NULL, 16); break;
			defualt: break;
		}
	}
	ptr->first_child = NULL;
	ptr->last_child = NULL;
	ptr->next = NULL;
	return ptr;
}

void connect(TreeNode *parent, TreeNode *child){
	if (child == NULL || child->next != NULL) return;
	if (parent->first_child == NULL){
		parent->first_child = child;
		parent->last_child = child;
		return;
	}
	parent->last_child->next = child;
	parent->last_child = child;
	return;
}

void show(TreeNode *root, int depth){
	for (int i = 0; i < depth * 2; i++)
		printf(" ");
	printf("%s", root->name);
	if (root->node_type == __SYMBOL_TYPE__){
		printf(" (%d)\n", root->line);
	} else if (root->node_type == __TOKEN_TYPE__){
		switch (root->value_type){
			case OCT_VALUE:
			case HEX_VALUE:
			case INT_VALUE: printf(": %d", root->int_val); break;
			case FLOAT_VALUE: printf(": %f", root->float_val); break;
			case TYPE_VALUE: printf(": %s", root->type_val); break;
			case ID_VALUE: printf(": %s", root->id_val); break;
			defualt: break;
		}
		printf("\n");
	}

	TreeNode *ptr = root->first_child;
	while (ptr != NULL){
		show(ptr, depth + 1);
		ptr = ptr->next;
	}
	return;
}
