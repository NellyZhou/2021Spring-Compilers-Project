#ifndef __SEMANTICS_H__
#define __SEMANTICS_H__
#include "SyntaxTree.h"
#define HASHLIST_VOLUMN 16384

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
    char* name;         //域的名字
    Type type;          //域的类型
    FieldList tail;     //下一个域
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

extern HashNode* hash_list[HASHLIST_VOLUMN];

//============== hash operations ===================
extern void HashListInitial();
extern unsigned int Hash_pjw(char* name);           //Hash函数
extern bool HashListAdd(HashNodeType node_type, void* u, int current_line);       //插入成功返回true 已经定义过则返回false
extern void* HashListFind(HashNodeType node_type, char* name);


//************** High-level Definitions *****************
extern void SemanticsProgamAnalysis(TreeNode* root);
extern void ExtDefList(TreeNode* root);
extern void ExtDef(TreeNode* root);
extern void ExtDecList(TreeNode* root, Type t);     //表示变量的type


//************** Specifiers ******************************
extern Type Specifier(TreeNode* root);              //需要判断Type是否在HashList中
extern Type structSpecifier(TreeNode* root);
extern char* OptTag(TreeNode* root);
extern char* Tag(TreeNode* root);

//************** Declarators ******************************
extern Variable VarDec(TreeNode* root, Type t, bool in_struct_field);
extern Function FunDec(TreeNode* root, Type t);     //表示return_type
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
extern bool SharingSameName(FieldList f);
//2.3
extern bool isSameField(FieldList a, FieldList b);

#endif
