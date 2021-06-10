#ifndef __IR_TRANSLATER_H__
#define __IR_TRANSLATER_H__
#include "SyntaxTree.h"
#include "semantics.h"
#include <stdlib.h>
#include <assert.h>

typedef int bool;
#define true 1
#define false 0


typedef struct Operand_* Operand;
typedef struct InterCode_* InterCode;
typedef struct InterCodes_* InterCodes;
typedef struct ArgList_* ArgList;
struct Operand_ {
    enum {
        IR_VARIABLE, CONSTANT, ADDRESS,
        IR_FUNCTION,
        RELOP,
        TEMP, LABEL /*..*/
    }kind;
    union {
        unsigned int int_val;
        char value[32];
        Operand addr_val;
    }u;
}Operand_;

struct InterCode_{
    enum {        
        SET_LABEL, FUNCTION_ENTRY,
        ASSIGN, 
        ADD, SUB, MUL, DIV,
        GET_ADDR, GET_VALUE, ASSIGN_ADDR,
        GOTO, IFGOTO, 
        RETURN, DEC,ARG,CALL,PARAM,
        READ, WRITE
    }kind;
    union 
    {
        struct {Operand op;} sinop; 
        struct {Operand right, left;} assign;
        struct {Operand result, op1, op2;} binop;
        struct {Operand op1, relop, op2, label;} ifgoto_op;
        struct {Operand op; unsigned int size;} dec_op;
        /*...*/    
    }u;
    
}InterCode_;

struct ArgList_{
    Operand param;
    struct ArgList_* next;
}ArgList_;

struct InterCodes_ {InterCode code; struct InterCodes_ *prev, *next;} InterCodes_;
extern InterCodes InterCodesList;

//
extern Operand new_temp();
extern Operand new_label();
extern void insert_intercode_operation(InterCodes intercodes_head, InterCode new_intercode);

//
extern void show_Op(Operand op);
extern void show_intercode(InterCode code);
extern void show_IR();
//
extern bool isArray(TreeNode* root);
extern unsigned int calculate_offset(unsigned int offset, FieldList tmp_type, char *field_name);
extern unsigned int calculate_size(Type t);
extern void change_dec_size(InterCodes intercodes_head, unsigned size);

//************** High-level Definitions *****************
extern InterCodes IR_Translater(TreeNode* root);
extern void translate_ExtDefList(TreeNode* root);
extern void translate_ExtDef(TreeNode* root);
extern void translate_ExtDecList(TreeNode* root);   

//************** Declarators ******************************
extern Operand translate_VarDec(TreeNode* root, bool is_function_param, Type t);
extern void translate_FunDec(TreeNode* root);    
extern void translate_VarList(TreeNode* root);
extern void translate_ParamDec(TreeNode* root);


//************** Statements ******************************
extern void translate_CompSt(TreeNode* root);
extern void translate_StmtList(TreeNode* root);
extern void translate_Stmt(TreeNode* root);

//************** Local Definitions *************************
extern void translate_DefList(TreeNode* root);
extern void translate_Def(TreeNode* root);
extern void translate_DecList(TreeNode* root, Type t);
extern void translate_Dec(TreeNode* root, Type t);

//************** Expressions ******************************
extern void translate_Exp(TreeNode* root, Operand place);
extern void translate_Args(TreeNode* root, ArgList arg_list);
extern void translate_Cond(TreeNode* root, Operand label_true, Operand label_false);

//

extern Operand translate_Exp_left(TreeNode* root, Operand place);


#endif