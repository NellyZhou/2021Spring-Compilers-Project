#ifndef __SYNTAXTREE_H__
#define __SYNTAXTREE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INT_VALUE 0 
#define FLOAT_VALUE 1
#define TYPE_VALUE 2
#define ID_VALUE 3
#define OCT_VALUE 4
#define HEX_VALUE 5
#define RELOP_VALUE 6

#define __TOKEN_TYPE__ 0
#define __SYMBOL_TYPE__ 1

typedef struct TreeNode{
	char name[32];
	int node_type;
	int line;
	int value_type;
	union {
		unsigned int int_val;
		float float_val;
		char type_val[32];
		char id_val[32];
	};

	struct TreeNode *first_child;
	struct TreeNode *last_child;
	struct TreeNode *next;
}TreeNode;

extern TreeNode * create_TreeNode(char name[], int node_type, int line,  int value_type, char value[]);
extern void connect(TreeNode *parent, TreeNode *child);
extern void show(TreeNode *root, int depth);
extern TreeNode* childNode(TreeNode *root, int index);
extern int countChild(TreeNode *root);


#endif
