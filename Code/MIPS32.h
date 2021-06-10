#ifndef __MIPS32_H__
#define __MIPS32_H__

#include "IR_translater.h"

typedef struct Var_* Var;
typedef struct VariableList_* VariableList;
typedef enum{
    MIPS_VAR, MIPS_ADDRESS, MIPS_VALUE
} OperandType;

struct Register{
    char name[32];
    bool used;
    Var variable;
}regs[32];

struct Var_{
    char name[32];
    int offset;
    int reg_index;
}Var_;

struct VariableList_{
    Var variable;
    VariableList next;
}VariableList_;

extern VariableList varListHead;
//============= PROGRAM INITIALIZATION ==================
extern void __initialize__();
extern void initialize_regs();

//============= TRANSLATE INTERCODES ====================
extern void MIPS_translater(InterCodes InterCodesList);
extern void translate_FUNCTION_ENTRY(InterCodes ptr);
extern InterCodes translate_CALL(InterCodes ptr);
extern void translate_RETURN(InterCodes ptr);

//============= OPTIONAL FUNCTIONS ======================
extern int get_free_reg();
extern int assign_op2reg(Operand op, OperandType type);
extern int assign_reg2memeory(int index);
extern void free_regs();
extern void add_op2var(Var v);
extern Var find_var(Operand op);

extern void assign_stack(Operand op, int offset);

#endif