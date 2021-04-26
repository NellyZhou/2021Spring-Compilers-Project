#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__
#include "SyntaxTree.h"
#define HASHLIST_VOLUMN 0x3fff

//#define L2_DEBUG true
#define L2_DEBUG false

typedef int bool;
#define true 1
#define false 0

typedef struct Type_* Type;
typedef struct Type_ Type_;
typedef struct FieldList_* FieldList;
typedef struct FieldList_ FieldList_;
typedef struct Variable_* Variable;
typedef struct Variable_ Variable_;
typedef struct Function_* Function;
typedef struct Function_ Function_;
typedef enum {
    VARIABLE, FUNCTION, STRUCTURE_TYPE
} HashNodeType;
typedef enum{
    BASIC, ARRAY, STRUCTURE
} BasicType;
#define BASIC_INT       0
#define BASIC_FLOAT     1

struct Type_{
    BasicType kind;
    union{
        int basic;
        struct {
            Type elem;
            int size;
        }array;
        FieldList structure;
    }u;
};

struct FieldList_{
    char* name;         //field name
    int line;
    Type type;          //field type
    FieldList tail;     //next field
};

struct Variable_{
    char* name;
    Type type;
};

struct Function_{
    char* name;
    Type return_type;
    int param_nums;
    FieldList params;
};

typedef struct HashNode{
    HashNodeType kind;
    union {
        Variable variable;
        Function function;
    }u;
    struct HashNode* next;
}HashNode;

extern HashNode* hash_list[HASHLIST_VOLUMN + 1];

//============== hash operations ===================
extern void HashListInitial();
extern unsigned int Hash_pjw(char* name);           //Hash Function
extern bool HashListAdd(HashNodeType node_type, void* u, int current_line);       //insert successfully return true; false
extern void* HashListFind(HashNodeType node_type, char* name);


//************** High-level Definitions *****************
extern void SemanticsProgramAnalysis(TreeNode* root);
extern void ExtDefList(TreeNode* root);
extern void ExtDef(TreeNode* root);
extern void ExtDecList(TreeNode* root, Type t);   


//************** Specifiers ******************************
extern Type Specifier(TreeNode* root);             
extern Type structSpecifier(TreeNode* root);
extern char* OptTag(TreeNode* root);
extern char* Tag(TreeNode* root);

//************** Declarators ******************************
extern Variable VarDec(TreeNode* root, Type t, bool in_struct_field);
extern Function FunDec(TreeNode* root, Type t);    
extern FieldList VarList(TreeNode* root, int* count);
extern Variable ParamDec(TreeNode* root);


//************** Statements ******************************
extern void CompSt(TreeNode* root, Function f);
extern void StmtList(TreeNode* root, Function f);
extern void Stmt(TreeNode* root, Function f);

//************** Local Definitions *************************
extern FieldList DefList(TreeNode* root, bool in_struct_field);
extern FieldList Def(TreeNode* root, bool in_struct_field);
extern FieldList DecList(TreeNode* root, Type t, bool in_struct_field);
extern FieldList Dec(TreeNode* root, Type t, bool in_struct_field);

//************** Expressions ******************************
extern Type Exp(TreeNode* root);
extern void Args(TreeNode* root, FieldList f, char* function_name);


//============== Checking Conditions ======================
//error 5/7/8
extern bool isSameType(Type a, Type b);
//error 6
extern bool isLeftValueExp(TreeNode* root);
//error 7
extern bool isLogicExp(Type a);
extern bool isArithExp(Type a);
//error 15
extern bool SharingSameName(FieldList f, int current_line);
//2.3
extern bool isSameField(FieldList a, FieldList b);

#endif
