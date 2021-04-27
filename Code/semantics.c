#include "semantics.h"
#include <stdio.h>


HashNode* hash_list[HASHLIST_VOLUMN + 1];

void HashListInitial(){
    for (int i = 0; i < HASHLIST_VOLUMN; i++){
        hash_list[i] = NULL;
    }
    return;
}

unsigned int Hash_pjw(char* name){
    unsigned int val = 0, i;
    for (; *name; ++name){
        val = (val << 2) + *name;
        if (i = val & ~HASHLIST_VOLUMN)
            val = (val ^ (i >> 12)) & HASHLIST_VOLUMN;
    }
    return val;
    
}
bool HashListAdd(HashNodeType node_type, void* u, int current_line){
    if (L2_DEBUG){
   	printf("add hashlist\n");
    }
    HashNode* current_node = (HashNode*)malloc(sizeof(HashNode));
    //node is variable
    if (node_type == VARIABLE){
        current_node->kind = VARIABLE;
        current_node->u.variable = (Variable) u;
        current_node->next = NULL;
        
        char* current_node_name = current_node->u.variable->name;
        unsigned int hash_key = Hash_pjw(current_node_name);
        
        if (hash_list[hash_key] == NULL){
	    if (L2_DEBUG){
   		printf("add variable \"%s\"\n", current_node_name);
	    }
            hash_list[hash_key] = current_node;
        } else {
            HashNode* p = hash_list[hash_key];
            while (p != NULL){
                if (p->kind != FUNCTION && strcmp(current_node_name, p->u.variable->name) == 0){
                    printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", current_line, current_node_name);
                    return false;
                }
                if (p->kind == FUNCTION && strcmp(current_node_name, p->u.function->name) == 0){
                    printf("Error type 3 at Line %d: Redefined variable \"%s\".\n", current_line, current_node_name);
                    return false;
                }
                p = p->next;
            }
	    if (L2_DEBUG){
   		printf("add variable \"%s\"\n", current_node_name);
	    }
            current_node->next = hash_list[hash_key];
            hash_list[hash_key] = current_node;
        }
    }
    
    //node is function
    if (node_type == FUNCTION){
        current_node->kind = FUNCTION;
        current_node->u.function = (Function) u;
        current_node->next = NULL;
        
        char* current_node_name = current_node->u.function->name;
        unsigned int hash_key = Hash_pjw(current_node_name);
        
        if (hash_list[hash_key] == NULL){
	    if (L2_DEBUG){
   		printf("add function \"%s\"\n", current_node_name);
	    }
            hash_list[hash_key] = current_node;
        } else {
            HashNode* p = hash_list[hash_key];
            while (p != NULL){
                if (p->kind != FUNCTION && strcmp(current_node_name, p->u.variable->name) == 0){
                    printf("Error type 4 at Line %d: Redefined function \"%s\".\n", current_line, current_node_name);
                    return false;
                }
                if (p->kind == FUNCTION && strcmp(current_node_name, p->u.function->name) == 0){
                    printf("Error type 4 at Line %d: Redefined function \"%s\".\n", current_line, current_node_name);
                    return false;
                }
                p = p->next;
            }
	    if (L2_DEBUG){
   		printf("add function \"%s\"\n", current_node_name);
	    }
            current_node->next = hash_list[hash_key];
            hash_list[hash_key] = current_node;
        }
    }
    
    if (node_type == STRUCTURE_TYPE){ 
	if (L2_DEBUG){
   	    printf("strucure:\n");
	}
	current_node->kind = STRUCTURE_TYPE;
        current_node->u.variable = (Variable) u;
        current_node->next = NULL;
        
        char* current_node_name = current_node->u.variable->name;
        if (current_node_name == NULL)                                          //unnamed structure
            return true;
	unsigned int hash_key = Hash_pjw(current_node_name);
	if (hash_list[hash_key] == NULL){
	    if (L2_DEBUG){
   		printf("add structure \"%s\"\n", current_node_name);
	    }
            hash_list[hash_key] = current_node;
        } else {
            HashNode* p = hash_list[hash_key];
            while (p != NULL){
                if (p->kind != FUNCTION && strcmp(current_node_name, p->u.variable->name) == 0){
                    printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", current_line, current_node_name);
                    return false;
                }
                if (p->kind == FUNCTION && strcmp(current_node_name, p->u.function->name) == 0){
                    printf("Error type 16 at Line %d: Duplicated name \"%s\".\n", current_line, current_node_name);
                    return false;
                }
                p = p->next;
            }
	    if (L2_DEBUG){
   		printf("add structure \"%s\"\n", current_node_name);
	    }
            current_node->next = hash_list[hash_key];
            hash_list[hash_key] = current_node;
        }
    }
    
    return true;
}

void* HashListFind(HashNodeType node_type, char* name){
    if (name == NULL)
        return NULL;
    unsigned int hash_key = Hash_pjw(name);
    HashNode* p = hash_list[hash_key];
    while (p != NULL){
        if (p->kind == VARIABLE && strcmp(name, p->u.variable->name) == 0){
            if (node_type == VARIABLE)
                return p->u.variable;
            else
                return NULL;
        }
        if (p->kind == FUNCTION && strcmp(name, p->u.function->name) == 0){
            if (node_type == FUNCTION)
                return p->u.function;
            else
                return NULL;
        }
        if (p->kind == STRUCTURE_TYPE && strcmp(name, p->u.variable->name) == 0){
            if (node_type == STRUCTURE_TYPE)
                return p->u.variable;
            else
                return NULL;
        }
        p = p->next;
    }
    return NULL;
}

//**************************************************************
//*                     High-level Definitions                 *
//**************************************************************

void SemanticsProgramAnalysis(TreeNode* root){
    if (root == NULL)
        return;

    if (L2_DEBUG)
	printf("reached program\n");

    //-> ExtDefList
    if (strcmp(childNode(root, 0)->name, "ExtDefList") == 0){
        ExtDefList(childNode(root, 0));
    }
    return;
}

void ExtDefList(TreeNode* root){
    if (root == NULL)
        return;
    int count_node = countChild(root);
    
    if (L2_DEBUG){
	printf("reached ExtDefList\n");
	//printf("reached ExtDefList with child nodes: \n %s %s \n", childNode(root, 0)->name, childNode(root, 1)->name);
        //printf("%d %d\n",strcmp(childNode(root, 0)->name, "ExtDef"), strcmp(childNode(root, 1)->name, "ExtDefList"));
    }
    //-> ExtDef ExtDefList
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "ExtDef") == 0 && strcmp(childNode(root, 1)->name, "ExtDefList") == 0){
        ExtDef(childNode(root, 0));
        ExtDefList(childNode(root, 1));
    }
    //-> *empty*
    return;
}

void ExtDef(TreeNode* root){
    if (L2_DEBUG)
	printf("reached ExtDef\n");
    
    if (root == NULL)
        return;
    int count_node = countChild(root);
    
    //-> Specifier ExtDecList SEMI
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Specifier") == 0 && strcmp(childNode(root, 1)->name, "ExtDecList") == 0 && strcmp(childNode(root, 2)->name, "SEMI") == 0){
        Type t = Specifier(childNode(root, 0));
        if (t == NULL)
            return;
        ExtDecList(childNode(root, 1), t);
        return;
    }
    
    //-> Specifier SEMI
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "Specifier") == 0 && strcmp(childNode(root, 1)->name, "SEMI") == 0){
        Specifier(childNode(root, 0));
        return;
    }
    
    //-> Specifier FunDec CompSt
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Specifier") == 0 && strcmp(childNode(root, 1)->name, "FunDec") == 0 && strcmp(childNode(root, 2)->name, "CompSt") == 0){
        Type t = Specifier(childNode(root, 0));
        Function f = FunDec(childNode(root, 1), t);
        CompSt(childNode(root, 2), f);
        return;
    }
    return;
}

void ExtDecList(TreeNode* root, Type t){
    if (root == NULL)
        return;
    int count_node = countChild(root);
    
    //-> VarDec
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "VarDec") == 0){
        VarDec(childNode(root, 0), t, false);
        return;
    }
    
    //-> VarDec COMMA ExtDecList
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "VarDec") == 0 && strcmp(childNode(root, 1)->name, "COMMA") == 0 && strcmp(childNode(root, 2)->name, "ExtDecList") == 0){
        VarDec(childNode(root, 0), t, false);
        ExtDecList(childNode(root, 2), t);
        return;
    }
}

//**************************************************************
//*                     Specifiers                             *
//**************************************************************

Type Specifier(TreeNode* root){
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
    
    if (L2_DEBUG)
	printf("reached Specifier\n");
    
    //-> TYPE
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "TYPE") == 0){
        TreeNode* current_child = childNode(root, 0);
        Type tmp_type = (Type)malloc(sizeof(Type_));
        tmp_type->kind = BASIC;
        if (strcmp(current_child->type_val, "int") == 0){
            tmp_type->u.basic = BASIC_INT;
        }
        if (strcmp(current_child->type_val, "float") == 0){
            tmp_type->u.basic = BASIC_FLOAT;
        }
        return tmp_type;
    }
        
    //-> StructSpecifier
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "StructSpecifier") == 0){
        return structSpecifier(childNode(root, 0));
    }
    return NULL;
}


Type structSpecifier(TreeNode* root){
    if (L2_DEBUG)
	printf("reached structSpecifier\n");
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
    //-> STRUCT OptTag LC DefList RC
    if (count_node == 5 && strcmp(childNode(root, 0)->name, "STRUCT") == 0 && strcmp(childNode(root, 1)->name, "OptTag") == 0 && strcmp(childNode(root, 2)->name, "LC") == 0 && strcmp(childNode(root, 3)->name, "DefList") == 0 && strcmp(childNode(root, 4)->name, "RC") == 0){
        char* struct_name = OptTag(childNode(root, 1));
        
        Variable v = (Variable)malloc(sizeof(Variable_));
        if (struct_name == NULL){
            v->name = NULL;
        } else {
            v->name = (char*)malloc(sizeof(char) * (strlen(struct_name) + 1));
            strcpy(v->name, struct_name);
        }
        
        v->type = (Type)malloc(sizeof(Type_));
        Type t = v->type;
        t->kind = STRUCTURE;
        t->u.structure = DefList(childNode(root, 3), true);
        
        FieldList f = t->u.structure;
        //if (f == NULL)  return NULL;
        if (SharingSameName(f, childNode(root, 1)->line)) return NULL;                //error 15
        if (!HashListAdd(STRUCTURE_TYPE, v, childNode(root, 1)->line)) return NULL;   //error 3
       
        Type tmp_type = (Type)malloc(sizeof(Type_));
        tmp_type->kind = t->kind;
        tmp_type->u = t->u;
        return tmp_type;
    }
    
    //-> STRUCT Tag
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "STRUCT") == 0 && strcmp(childNode(root, 1)->name, "Tag") == 0){
        
        char* struct_name = Tag(childNode(root, 1));
        Variable current_struct = HashListFind(STRUCTURE_TYPE, struct_name);
        if (current_struct == NULL){
            TreeNode* node_struct = childNode(root, 0);
            printf("Error type 17 at Line %d: Undefined structure\"%s\".\n", node_struct->line, struct_name);
            return NULL;
        } else {
            Type tmp_type = (Type)malloc(sizeof(Type_));
            tmp_type->kind = current_struct->type->kind;
            tmp_type->u = current_struct->type->u;
            return tmp_type;
        }
    }
    return NULL;
}


char* OptTag(TreeNode* root){
    if (L2_DEBUG)
	printf("reached OptTag\n");
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
    //-> ID
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ID") == 0){
        TreeNode* current_id = childNode(root, 0);
        return current_id->id_val;
    }
    //-> *empty*
    return NULL;
    
}

char* Tag(TreeNode* root){
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
    //-> ID
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ID") == 0){
        TreeNode* current_id = childNode(root, 0);
        return current_id->id_val;
    }
    return NULL;
}

//**************************************************************
//*                     Declarators                            *
//**************************************************************
Variable VarDec(TreeNode* root, Type t, bool in_struct_field){
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
    
    if (L2_DEBUG)
	printf("reached vardec\n");

    //-> ID
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ID") == 0){
        TreeNode* node_id = childNode(root, 0);
        Variable current_variable = (Variable)malloc(sizeof(Variable_));
        current_variable->name = (char*)malloc(sizeof(char) * (strlen(node_id->id_val) + 1));
        strcpy(current_variable->name, node_id->id_val);
        current_variable->type = t;
        
        if (!in_struct_field)
            HashListAdd(VARIABLE, current_variable, node_id->line);
        
        return current_variable;
    }
    
    //-> VarDec LB INT RB
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "VarDec") == 0 && strcmp(childNode(root, 1)->name, "LB") == 0 && strcmp(childNode(root, 2)->name, "INT") == 0 && strcmp(childNode(root, 3)->name, "RB") == 0){
        TreeNode* node_int = childNode(root, 2);
        unsigned int add_type_size = node_int->int_val;
        
        Type new_type = (Type)malloc(sizeof(Type_));
        new_type->kind = ARRAY;
        new_type->u.array.size = add_type_size;
        new_type->u.array.elem = t;
        
        return VarDec(childNode(root, 0), new_type, in_struct_field);
    }
    
    return NULL;
}

Function FunDec(TreeNode* root, Type t){     //t for return_type
    if (L2_DEBUG)
	printf("reached FunDec\n");
    
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
    
    //-> ID LP VarList RP
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "ID") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "VarList") == 0 && strcmp(childNode(root, 3)->name, "RP") == 0){
        Function f = (Function)malloc(sizeof(Function_));
        TreeNode* node_id = childNode(root, 0);
        f->name = (char*)malloc(sizeof(char) * (strlen(node_id->id_val) + 1));
        strcpy(f->name, node_id->id_val);
        f->return_type = t;
        f->params = VarList(childNode(root, 2), &f->param_nums);
        HashListAdd(FUNCTION, f, node_id->line);
        return f;
    }
    
    //-> ID LP RP
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "ID") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "RP") == 0){
        Function f = (Function)malloc(sizeof(Function_));
        TreeNode* node_id = childNode(root, 0);
        f->name = (char*)malloc(sizeof(char) * (strlen(node_id->id_val) + 1));
        strcpy(f->name, node_id->id_val);
        f->return_type = t;
        f->params = NULL;
        f->param_nums = 0;
        HashListAdd(FUNCTION, f, node_id->line);
        return f;
    }
    
    return NULL;
}

FieldList VarList(TreeNode* root, int* count){
    if (L2_DEBUG)
	printf("reached VarList\n");
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
    
    //-> ParamDec COMMA VarList
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "ParamDec") == 0 && strcmp(childNode(root, 1)->name, "COMMA") == 0 && strcmp(childNode(root, 2)->name, "VarList") == 0){
        Variable v = ParamDec(childNode(root, 0));
        
        FieldList f = (FieldList)malloc(sizeof(FieldList_));
        f->name = (char*) malloc(sizeof(char) * (strlen(v->name) + 1));
        strcpy(f->name, v->name);
        f->type = v->type;
        f->tail = VarList(childNode(root, 2), count);
        
        (*count)++;
        return f;
    }
    
    //-> ParamDec
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ParamDec") == 0){
        Variable v = ParamDec(childNode(root, 0));
        
        FieldList f = (FieldList)malloc(sizeof(FieldList_));
        f->name = (char*) malloc(sizeof(char) * (strlen(v->name) + 1));
        strcpy(f->name, v->name);
        f->type = v->type;
        f->tail = NULL;
        
        *count = 1;
        return f;
    }
    return NULL;
}

Variable ParamDec(TreeNode* root){
    if (root == NULL)
        return NULL;
    if (L2_DEBUG){
	printf("reached ParamDec\n");
//	printf("\t %d child\n", count_node);
    }
    int count_node = countChild(root);
    //-> Specifier VarDec
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "Specifier") == 0 && strcmp(childNode(root, 1)->name, "VarDec") == 0){
        Type t = Specifier(childNode(root, 0));
	return VarDec(childNode(root, 1), t, false);
    }
    return NULL;
}

//**************************************************************
//*                     Statements                             *
//**************************************************************
void CompSt(TreeNode* root, Function f){
    if (L2_DEBUG)
	printf("reached CompSt\n");
    if (root == NULL)
        return;
    int count_node = countChild(root);
     
    //-> LC DefList StmtList RC
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "LC") == 0 && strcmp(childNode(root, 1)->name, "DefList") == 0 && strcmp(childNode(root, 2)->name, "StmtList") == 0 && strcmp(childNode(root, 3)->name, "RC") == 0){
        DefList(childNode(root, 1), false);
        StmtList(childNode(root, 2), f);
    }
    
    return;
}

void StmtList(TreeNode* root, Function f){
    if (L2_DEBUG)
	printf("reached StmtList\n");
    if (root == NULL)
        return;
    int count_node = countChild(root);
     
    //-> Stmt StmtList
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "Stmt") == 0 && strcmp(childNode(root, 1)->name, "StmtList") == 0){
        Stmt(childNode(root, 0), f);
        StmtList(childNode(root, 1), f);
    }
    
    //-> *empty*
    return;
}

void Stmt(TreeNode* root, Function f){
    if (L2_DEBUG)
	printf("reached Stmt\n");
    if (root == NULL)
        return;
    int count_node = countChild(root);
     
    //-> Exp SEMI
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "SEMI") == 0){
        Exp(childNode(root, 0));
    }
    
    //-> CompSt
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "CompSt") == 0){
        CompSt(childNode(root, 0), f);
    }
    
    //-> RETURN Exp SEMI
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "RETURN") == 0 && strcmp(childNode(root, 1)->name, "Exp") == 0 && strcmp(childNode(root, 2)->name, "SEMI") == 0){
        Type exp_type = Exp(childNode(root, 1));
        Type rtn_type = f->return_type;
        if (!isSameType(exp_type, rtn_type)){
            TreeNode* node_rtn = childNode(root, 0);
            printf("Error type 8 at Line %d: Type mismatched for return.\n", node_rtn->line);
        }
    }
    
    //-> IF LP Exp RP Stmt
    if (count_node == 5 && strcmp(childNode(root, 0)->name, "IF") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0 && strcmp(childNode(root, 3)->name, "RP") == 0 && strcmp(childNode(root, 4)->name, "Stmt") == 0){
        Type exp_type = Exp(childNode(root, 2));
        if (!isLogicExp(exp_type)){
            TreeNode* node_lp = childNode(root, 1);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", node_lp->line);
        }
        Stmt(childNode(root, 4), f);
    }
    
    //-> IF LP Exp RP Stmt ELSE Stmt
    if (count_node == 7 && strcmp(childNode(root, 0)->name, "IF") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0 && strcmp(childNode(root, 3)->name, "RP") == 0 && strcmp(childNode(root, 4)->name, "Stmt") == 0 && strcmp(childNode(root, 5)->name, "ELSE") == 0 && strcmp(childNode(root, 6)->name, "Stmt") == 0){
        Type exp_type = Exp(childNode(root, 2));
        if (!isLogicExp(exp_type)){
            TreeNode* node_lp = childNode(root, 1);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", node_lp->line);
        }
        Stmt(childNode(root, 4), f);
        Stmt(childNode(root, 6), f);
    }
    
    //-> WHILE LP Exp RP Stmt
    if (count_node == 5 && strcmp(childNode(root, 0)->name, "WHILE") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0 && strcmp(childNode(root, 3)->name, "RP") == 0 && strcmp(childNode(root, 4)->name, "Stmt") == 0){
        Type exp_type = Exp(childNode(root, 2));
        if (!isLogicExp(exp_type)){
            TreeNode* node_lp = childNode(root, 1);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", node_lp->line);
        }
        Stmt(childNode(root, 4), f);
    }
    
    return;
}

//**************************************************************
//*                     Local Definitions                      *
//**************************************************************
FieldList DefList(TreeNode* root, bool in_struct_field){
    if (L2_DEBUG)
	printf("reached DefList\n");
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
    
    //-> Def DefList
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "Def") == 0 && strcmp(childNode(root, 1)->name, "DefList") == 0){
        FieldList def_field = Def(childNode(root, 0), in_struct_field);
        FieldList deflist_field = DefList(childNode(root, 1), in_struct_field);
        if (in_struct_field){
            FieldList tmp_field = def_field;
	    if (tmp_field == NULL)
		    return deflist_field;
            while (tmp_field->tail != NULL){
                tmp_field = tmp_field->tail;
            }
            tmp_field->tail = deflist_field;
            return def_field;
        } else {
            return NULL;
        }
    }
    //-> *empty*
    return NULL;
}

FieldList Def(TreeNode* root, bool in_struct_field){
    if (L2_DEBUG)
	printf("reached Def\n");
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
     
    //-> Specifier DecList SEMI
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Specifier") == 0 && strcmp(childNode(root, 1)->name, "DecList") == 0 && strcmp(childNode(root, 2)->name, "SEMI") == 0){
        Type t = Specifier(childNode(root, 0));
        if (t == NULL)
            return NULL;
        FieldList tmp_field = DecList(childNode(root, 1), t, in_struct_field);
        if (in_struct_field){
            return tmp_field;
        } else {
            return NULL;
        }
    }
    return NULL;
}

FieldList DecList(TreeNode* root, Type t, bool in_struct_field){
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
     
    //-> Dec
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "Dec") == 0){
        FieldList dec_field = Dec(childNode(root, 0), t, in_struct_field);
        if (in_struct_field){
            return dec_field;
        } else {
            return NULL;
        }
    }
    
    //-> Dec COMMA DecList
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Dec") == 0 && strcmp(childNode(root, 1)->name, "COMMA") == 0 && strcmp(childNode(root, 2)->name, "DecList") == 0){
        FieldList dec_field = Dec(childNode(root, 0), t, in_struct_field);
        FieldList declist_field = DecList(childNode(root, 2), t, in_struct_field);
        if (in_struct_field){
            dec_field->tail = declist_field;
            return dec_field;
        } else {
            return NULL;
        }
    }
    return NULL;
}

FieldList Dec(TreeNode* root, Type t, bool in_struct_field){
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
    
    //-> VarDec
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "VarDec") == 0){
        Variable current_variable = VarDec(childNode(root, 0), t, in_struct_field);
        if (in_struct_field){
            FieldList tmp_field = (FieldList)malloc(sizeof(FieldList_));
            tmp_field->name = (char*)malloc(sizeof(char) * (strlen(current_variable->name) + 1));
            strcpy(tmp_field->name, current_variable->name);
            tmp_field->type = current_variable->type;
	    tmp_field->line = childNode(root, 0)->line;
            tmp_field->tail = NULL;
            return tmp_field;
        } else {
            return NULL;
        }
    }
    
    //-> VarDec ASSIGNOP Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "VarDec") == 0 && strcmp(childNode(root, 1)->name, "ASSIGNOP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Variable current_variable = VarDec(childNode(root, 0), t, in_struct_field);
        TreeNode* node_assignop = childNode(root, 1);
        if (in_struct_field){
            printf("Error type 15 at Line %d: Illegal structure definition assignment.\n", node_assignop->line);
            FieldList tmp_field = (FieldList)malloc(sizeof(FieldList_));
            tmp_field->name = (char*)malloc(sizeof(char) * (strlen(current_variable->name) + 1));
            strcpy(tmp_field->name, current_variable->name);
            tmp_field->type = current_variable->type;
	    tmp_field->line = childNode(root, 0)->line;
            tmp_field->tail = NULL;
            return tmp_field;
        } else {
            Type exp_type = Exp(childNode(root, 2));
            Type variable_type = current_variable->type;
            if (!isSameType(exp_type, variable_type)){
                printf("Error type 5 at Line %d: Type mismatched for assignment.\n", node_assignop->line);
            }
            return NULL;
        }
    }
    return NULL;
}

//**************************************************************
//*                     Expressions                            *
//**************************************************************
Type Exp(TreeNode* root){
    if (L2_DEBUG)
	printf("reached Exp  ");
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
    if (L2_DEBUG){
	printf("with %d childs\n", count_node);
		printf("\n\n");
		show(root, 0);
		printf("\n\n");
    
    }
    //-> Exp ASSIGNOP Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "ASSIGNOP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        if (L2_DEBUG){
		printf("\t a = b\n");
		printf("\n\n");
		show(childNode(root, 0), 0);
		printf("\n\n");
	}
        Type exp_type_a = Exp(childNode(root, 0));
        Type exp_type_b = Exp(childNode(root, 2));
        if (!isLeftValueExp(childNode(root, 0))){
            TreeNode* node_tmp = childNode(root, 1);
            printf("Error type 6 at Line %d: The left-hand side of an assignment must be a variable.\n", node_tmp->line);
            return NULL;
        }
        if (exp_type_a == NULL || exp_type_b == NULL){
            return NULL;
        }
        if (!isSameType(exp_type_a, exp_type_b)){
            TreeNode* node_tmp = childNode(root, 1);
            printf("Error type 5 at Line %d: Type mismatched for assignment.\n", node_tmp->line);
        }
        return exp_type_a;
    }
    
    //-> Exp AND Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "AND") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Type exp_type_a = Exp(childNode(root, 0));
        Type exp_type_b = Exp(childNode(root, 2));
        if (exp_type_a == NULL || exp_type_b == NULL){
            return NULL;
        }
        if (isLogicExp(exp_type_a) && isSameType(exp_type_a, exp_type_b)){
            return exp_type_a;
        } else {
            TreeNode* node_tmp = childNode(root, 1);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", node_tmp->line);
            return NULL;
        }
    }
    
    //-> Exp OR Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "OR") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Type exp_type_a = Exp(childNode(root, 0));
        Type exp_type_b = Exp(childNode(root, 2));
        if (exp_type_a == NULL || exp_type_b == NULL){
            return NULL;
        }
        if (isLogicExp(exp_type_a) && isSameType(exp_type_a, exp_type_b)){
            return exp_type_a;
        } else {
            TreeNode* node_tmp = childNode(root, 1);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", node_tmp->line);
            return NULL;
        }
    }
    
    //-> Exp RELOP Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "RELOP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Type exp_type_a = Exp(childNode(root, 0));
        Type exp_type_b = Exp(childNode(root, 2));
        if (exp_type_a == NULL || exp_type_b == NULL){
            //return NULL;
            Type tmp_type = (Type)malloc(sizeof(Type_));
            tmp_type->kind = BASIC;
            tmp_type->u.basic = BASIC_INT;
            return tmp_type;
        }
        if (isArithExp(exp_type_a) && isSameType(exp_type_a, exp_type_b)){
            Type tmp_type = (Type)malloc(sizeof(Type_));
            tmp_type->kind = BASIC;
            tmp_type->u.basic = BASIC_INT;
            return tmp_type;
        } else {
            TreeNode* node_tmp = childNode(root, 1);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", node_tmp->line);
            //return NULL;
            Type tmp_type = (Type)malloc(sizeof(Type_));
            tmp_type->kind = BASIC;
            tmp_type->u.basic = BASIC_INT;
            return tmp_type;
        }
    }
    
    //-> Exp PLUS Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "PLUS") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Type exp_type_a = Exp(childNode(root, 0));
        Type exp_type_b = Exp(childNode(root, 2));
        if (exp_type_a == NULL || exp_type_b == NULL){
            return NULL;
        }
        if (isArithExp(exp_type_a) && isSameType(exp_type_a, exp_type_b)){
            return exp_type_a;
        } else {
            TreeNode* node_tmp = childNode(root, 1);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", node_tmp->line);
            return NULL;
        }
    }
    
    //-> Exp MINUS Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "MINUS") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Type exp_type_a = Exp(childNode(root, 0));
        Type exp_type_b = Exp(childNode(root, 2));
        if (exp_type_a == NULL || exp_type_b == NULL){
            return NULL;
        }
        if (isArithExp(exp_type_a) && isSameType(exp_type_a, exp_type_b)){
            return exp_type_a;
        } else {
            TreeNode* node_tmp = childNode(root, 1);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", node_tmp->line);
            return NULL;
        }
        
    }
    
    //-> Exp STAR Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "STAR") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Type exp_type_a = Exp(childNode(root, 0));
        Type exp_type_b = Exp(childNode(root, 2));
        if (exp_type_a == NULL || exp_type_b == NULL){
            return NULL;
        }
        if (isArithExp(exp_type_a) && isSameType(exp_type_a, exp_type_b)){
            return exp_type_a;
        } else {
            TreeNode* node_tmp = childNode(root, 1);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", node_tmp->line);
            return NULL;
        }
        
    }
    
    //-> Exp DIV Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "DIV") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Type exp_type_a = Exp(childNode(root, 0));
        Type exp_type_b = Exp(childNode(root, 2));
        if (exp_type_a == NULL || exp_type_b == NULL){
            return NULL;
        }
        if (isArithExp(exp_type_a) && isSameType(exp_type_a, exp_type_b)){
            return exp_type_a;
        } else {
            TreeNode* node_tmp = childNode(root, 1);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", node_tmp->line);
            return NULL;
        }
    }
    
    //-> LP Exp RP
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "LP") == 0 && strcmp(childNode(root, 1)->name, "Exp") == 0 && strcmp(childNode(root, 2)->name, "RP") == 0){
        return Exp(childNode(root, 1));
    }
    
    //-> MINUS Exp
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "MINUS") == 0 && strcmp(childNode(root, 1)->name, "Exp") == 0){
        Type exp_type = Exp(childNode(root, 1));
        if (exp_type == NULL)
            return NULL;
        if (!isArithExp(exp_type)){
            TreeNode* node_minus = childNode(root, 1);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", node_minus->line);
            return NULL;
        }
        return exp_type;
    }
    
    //-> NOT Exp
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "NOT") == 0 && strcmp(childNode(root, 1)->name, "Exp") == 0){
        Type exp_type = Exp(childNode(root, 1));
        if (exp_type == NULL)
            return NULL;
        if (!isLogicExp(exp_type)){
            TreeNode* node_not = childNode(root, 1);
            printf("Error type 7 at Line %d: Type mismatched for operands.\n", node_not->line);
            return NULL;
        }
        return exp_type;
    }
    
    //-> ID LP Args RP
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "ID") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "Args") == 0 && strcmp(childNode(root, 3)->name, "RP") == 0){
        TreeNode* node_id = childNode(root, 0);
        Function current_func = HashListFind(FUNCTION, node_id->id_val);
        if (current_func == NULL){
            if (HashListFind(VARIABLE, node_id->id_val) == NULL){
                printf("Error type 2 at Line %d: Undefined function \"%s\".\n", node_id->line, node_id->id_val);
            } else {
                printf("Error type 11 at Line %d: \"%s\" is not a function.\n", node_id->line, node_id->id_val);
            }
            return NULL;
        }
        
        Args(childNode(root, 2), current_func->params, current_func->name);
        
        return current_func->return_type;
    }
    
    //-> ID LP RP
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "ID") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "RP") == 0){
        TreeNode* node_id = childNode(root, 0);
        Function current_func = HashListFind(FUNCTION, node_id->id_val);
        if (current_func == NULL){
            if (HashListFind(VARIABLE, node_id->id_val) == NULL){
                printf("Error type 2 at Line %d: Undefined function \"%s\".\n", node_id->line, node_id->id_val);
            } else {
                printf("Error type 11 at Line %d: \"%s\" is not a function.\n", node_id->line, node_id->id_val);
            }
            return NULL;
        }
        if (current_func->param_nums != 0){
            printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments.\n", node_id->line, node_id->id_val);
        }
        return current_func->return_type;
    }
    
    //-> Exp LB Exp RB
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "LB") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0 && strcmp(childNode(root, 3)->name, "RB") == 0){
        if (L2_DEBUG){
		printf("\t a[b]\n");
	}
        Type exp_type_a = Exp(childNode(root, 0));
        Type exp_type_b = Exp(childNode(root, 2));
        TreeNode* node_lb = childNode(root, 1);
        if (L2_DEBUG){
		printf("exp_type_a is type %d\n", exp_type_a->kind);
	}
        if (exp_type_a == NULL || exp_type_b == NULL)
            return NULL;
        if (exp_type_a->kind != ARRAY){
            printf("Error type 10 at Line %d: Expression before \"[]\" is not an array.\n", node_lb->line);
            return NULL;
        }
        if (!isLogicExp(exp_type_b)){
            printf("Error type 12 at Line %d: Expression between \"[]\" is not an integer.\n", node_lb->line);
            return NULL;
        }
        return exp_type_a->u.array.elem;
    }
    
    
    //-> Exp DOT ID
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "DOT") == 0 && strcmp(childNode(root, 2)->name, "ID") == 0){
        if (L2_DEBUG){
		printf("\t a.b\n");
	}
        Type exp_type = Exp(childNode(root, 0));
        TreeNode* node_dot = childNode(root, 1);
        TreeNode* node_id = childNode(root, 2);
        
        if (exp_type == NULL)
            return NULL;
        if (exp_type->kind != STRUCTURE){
            printf("Error type 13 at Line %d: Illegal use of \".\".\n", node_dot->line);
            return NULL;
        }
        
        Type object_type = NULL;
        FieldList field_ptr = exp_type->u.structure;
        while (field_ptr != NULL){
            if (strcmp(field_ptr->name, node_id->id_val) == 0){
                object_type = field_ptr->type;
                break;
            }
            field_ptr = field_ptr->tail;
        }
        
        if (object_type == NULL){
            printf("Error type 14 at Line %d: Non-existent field \"%s\".\n", node_dot->line, node_id->id_val);
            return NULL;
        } else {
            return object_type;
        }
        return NULL;
    }
    
    //-> ID
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ID") == 0){
        TreeNode* node_id = childNode(root, 0);
        Variable current_variable = HashListFind(VARIABLE, node_id->id_val);
        if (current_variable != NULL){
            return current_variable->type;
        } else {
            printf("Error type 1 at Line %d: Undefined variable \"%s\".\n", node_id->line, node_id->id_val);
            return NULL;
        }
    }
    
    //-> INT
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "INT") == 0){
        Type t = (Type)malloc(sizeof(Type_));
        t->kind = BASIC;
        t->u.basic = BASIC_INT;
        return t;
    }
    
    //-> FLOAT
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "FLOAT") == 0){
        Type t = (Type)malloc(sizeof(Type_));
        t->kind = BASIC;
        t->u.basic = BASIC_FLOAT;
        return t;
    }
    return NULL;
}

void Args(TreeNode* root, FieldList f, char* function_name){
    if (root == NULL)
        return;
    int count_node = countChild(root);
    
    //-> Exp COMMA Args
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "COMMA") == 0 && strcmp(childNode(root, 2)->name, "Args") == 0){
        Type exp_type = Exp(childNode(root, 0));
        Type field_type = f->type;
        if (!isSameType(exp_type, field_type) || f->tail == NULL){
            TreeNode* node_tmp = childNode(root, 1);
            printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments.\n", node_tmp->line, function_name);
            return;
        }
        Args(childNode(root, 2), f->tail, function_name);
    }
    
    //-> Exp
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "Exp") == 0){
        Type exp_type = Exp(childNode(root, 0));
        Type field_type = f->type;
        if (!isSameType(exp_type, field_type) || f->tail != NULL){
            TreeNode* node_tmp = childNode(root, 0);
            printf("Error type 9 at Line %d: Function \"%s\" is not applicable for arguments.\n", node_tmp->line, function_name);
            return;
        }
    }
    return;
}


//**************************************************************
//*                     Checking Conditions                    *
//**************************************************************
//error 5/7/8
bool isSameType(Type a, Type b){
    if (L2_DEBUG)
	printf("checked isSameType\n");
    if (a == NULL || b == NULL)
        return false;
    if (a->kind != b->kind)
        return false;
    if (a->kind == BASIC){
        return (a->u.basic == b->u.basic);
    }
    if (a->kind == ARRAY){
        return isSameType(a->u.array.elem, b->u.array.elem);
    }
    if (a->kind == STRUCTURE){
        return isSameField(a->u.structure, b->u.structure);
    }
    return false;
}

//error 6
bool isLeftValueExp(TreeNode* root){
    if (root == NULL)
        return false;
    int count_node = countChild(root);
    
    //-> Exp LB Exp RB
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "LB") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0 && strcmp(childNode(root, 3)->name, "RB") == 0){
        return true;
    }
    //-> Exp DOT ID
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "DOT") == 0 && strcmp(childNode(root, 2)->name, "ID") == 0){
        return true;
    }
    //-> ID
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ID") == 0){
        return true;
    }
    return false;
}

//error 7
bool isLogicExp(Type a){
    if (a == NULL)
	return false;
    return (a->kind == BASIC && a->u.basic == BASIC_INT);
}

bool isArithExp(Type a){
    if (a == NULL)
	return false;
    return (a->kind == BASIC);
}

//error 15
bool SharingSameName(FieldList f, int current_line){
    if (L2_DEBUG)
	    printf("reached SharingSameNAme\n");
    if (f == NULL) return false;
    if (L2_DEBUG){
	FieldList p = f;
	while (p != NULL){
		printf("\t%s\n", p->name);
		p = p->tail;
	}
   	for (FieldList field_i = f; field_i->tail != NULL; field_i = field_i->tail)
        	for (FieldList field_j = field_i->tail; field_j != NULL; field_j = field_j->tail)
		{    
   			printf("\t compare: %s %s %d\n", field_i->name, field_j->name, strcmp(field_i->name, field_j->name));
		}	
    }
    for (FieldList field_i = f; field_i->tail != NULL; field_i = field_i->tail)
        for (FieldList field_j = field_i->tail; field_j != NULL; field_j = field_j->tail)
            if (strcmp(field_i->name, field_j->name) == 0){
                printf("Error type 15 at Line %d: Redefined field \"%s\".\n", field_j->line, field_i->name);
		return true;
	    }
    return false;
}

//2.3
bool isSameField(FieldList a, FieldList b){
    if (L2_DEBUG)
	    printf("check isSameField\n");
    if (a == NULL && b == NULL)
        return true;
    if (a == NULL || b == NULL)
        return false;
    if (!isSameType(a->type, b->type))
        return false;
    return isSameField(a->tail, b->tail);
}




