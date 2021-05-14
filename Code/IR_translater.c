#include "IR_translater.h"

InterCodes InterCodesList;
static Operand temp_zero, temp_one;

static void initial(){
    temp_zero = (Operand)malloc(sizeof(Operand_));
    {
        temp_zero->kind = CONSTANT;
        temp_zero->u.int_val = 0;
    }
    temp_one = (Operand)malloc(sizeof(Operand_));
    {
        temp_one->kind = CONSTANT;
        temp_one->u.int_val = 1;
    }
    return;
}

//**************************************************************
//*                     High-level Definitions                 *
//**************************************************************
/*
    COMPLETE
*/
void IR_Translater(TreeNode* root){
    initial();
    if (root == NULL)
        return;
    //-> ExtDefList
    if (strcmp(childNode(root, 0)->name, "ExtDefList") == 0){
        translate_ExtDefList(childNode(root, 0));
    }
    return;
}

void translate_ExtDefList(TreeNode* root){
    if (root == NULL)
        return;
    int count_node = countChild(root);
    //-> ExtDef ExtDefList
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "ExtDef") == 0 && strcmp(childNode(root, 1)->name, "ExtDefList") == 0){
        translate_ExtDef(childNode(root, 0));
        translate_ExtDefList(childNode(root, 1));
    }
    //-> *empty*
    return;
}

void translate_ExtDef(TreeNode* root){
    if (root == NULL)
        return;
    int count_node = countChild(root);
    //-> Specifier FunDec CompSt
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Specifier") == 0 && strcmp(childNode(root, 1)->name, "FunDec") == 0 && strcmp(childNode(root, 2)->name, "CompSt") == 0){
        translate_FunDec(childNode(root, 1));
        translate_CompSt(childNode(root, 2));
        return;
    }
    return;
}


//**************************************************************
//*                     Declarators                            *
//**************************************************************
/*
    TODO: 
        translate_FunDec
        translate_vardec

*/
void translate_VarDec(TreeNode* root, bool is_function_param){
    if (root == NULL)
        return;
    int count_node = countChild(root);

    //-> ID
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ID") == 0){
    }
    
    //-> VarDec LB INT RB
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "VarDec") == 0 && strcmp(childNode(root, 1)->name, "LB") == 0 && strcmp(childNode(root, 2)->name, "INT") == 0 && strcmp(childNode(root, 3)->name, "RB") == 0){
        translate_VarDec(childNode(root, 0), is_function_param);
    }
    
    return;
}

void translate_FunDec(TreeNode* root){   
    if (root == NULL)
        return;
    int count_node = countChild(root);

    /*TODO: insert function name label*/

    //-> ID LP VarList RP
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "ID") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "VarList") == 0 && strcmp(childNode(root, 3)->name, "RP") == 0){
        translate_VarList(childNode(root, 2));
    }
    return;
}

void translate_VarList(TreeNode* root){
    if (root == NULL)
        return;
    int count_node = countChild(root);
    
    //-> ParamDec COMMA VarList
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "ParamDec") == 0 && strcmp(childNode(root, 1)->name, "COMMA") == 0 && strcmp(childNode(root, 2)->name, "VarList") == 0){
        translate_ParamDec(childNode(root, 0));
        translate_VarList(childNode(root, 2));
    }
    
    //-> ParamDec
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ParamDec") == 0){
        translate_ParamDec(childNode(root, 0));
    }
    return;
}

void translate_ParamDec(TreeNode* root){
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);

    //-> Specifier VarDec
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "Specifier") == 0 && strcmp(childNode(root, 1)->name, "VarDec") == 0){
	    translate_VarDec(childNode(root, 1), true);
    }
    return;
}


//**************************************************************
//*                     Statements                             *
//**************************************************************
/*
    COMPLETE
*/
void translate_CompSt(TreeNode* root){
    if (root == NULL)
        return;
    int count_node = countChild(root);
     
    //-> LC DefList StmtList RC
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "LC") == 0 && strcmp(childNode(root, 1)->name, "DefList") == 0 && strcmp(childNode(root, 2)->name, "StmtList") == 0 && strcmp(childNode(root, 3)->name, "RC") == 0){
        translate_DefList(childNode(root, 1));
        translate_StmtList(childNode(root, 2));
    }
    
    return;
}

void translate_StmtList(TreeNode* root){
    if (root == NULL)
        return;
    int count_node = countChild(root);
     
    //-> Stmt StmtList
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "Stmt") == 0 && strcmp(childNode(root, 1)->name, "StmtList") == 0){
        translate_Stmt(childNode(root, 0));
        translate_StmtList(childNode(root, 1));
    }
    
    //-> *empty*
    return;
}

void translate_Stmt(TreeNode* root){
    if (root == NULL)
        return;
    int count_node = countChild(root);
     
    //-> Exp SEMI
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "SEMI") == 0){
        translate_Exp(childNode(root, 0), NULL);
    }
    
    //-> CompSt
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "CompSt") == 0){
        translate_CompSt(childNode(root, 0));
    }
    
    //-> RETURN Exp SEMI
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "RETURN") == 0 && strcmp(childNode(root, 1)->name, "Exp") == 0 && strcmp(childNode(root, 2)->name, "SEMI") == 0){
        Operand t1 = new_temp();
        translate_Exp(childNode(root, 1), t1);
        // [RETURN t1]
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = RETURN;
            new_code->u.sinop.op = t1; 
        }
        insert_intercode_operation(InterCodesList, new_code);
    }
    
    //-> IF LP Exp RP Stmt
    if (count_node == 5 && strcmp(childNode(root, 0)->name, "IF") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0 && strcmp(childNode(root, 3)->name, "RP") == 0 && strcmp(childNode(root, 4)->name, "Stmt") == 0){
        Operand label1 = new_label();
        Operand label2 = new_label();
        // code1
        translate_Cond(childNode(root, 2), label1, label2);
        // [LABEL label1]
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SET_LABEL;
            new_code->u.sinop.op = label1; 
        }
        insert_intercode_operation(InterCodesList, new_code);
        // code2
        translate_Stmt(childNode(root, 4));
        // [LABEL label2]
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SET_LABEL;
            new_code->u.sinop.op = label2; 
        }
        insert_intercode_operation(InterCodesList, new_code);
    }
    
    //-> IF LP Exp RP Stmt ELSE Stmt
    if (count_node == 7 && strcmp(childNode(root, 0)->name, "IF") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0 && strcmp(childNode(root, 3)->name, "RP") == 0 && strcmp(childNode(root, 4)->name, "Stmt") == 0 && strcmp(childNode(root, 5)->name, "ELSE") == 0 && strcmp(childNode(root, 6)->name, "Stmt") == 0){
        Operand label1 = new_label();
        Operand label2 = new_label();
        Operand label3 = new_label();
        // code1
        translate_Cond(childNode(root, 2), label1, label2);
        // [LABEL label1]
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SET_LABEL;
            new_code->u.sinop.op = label1; 
        }
        insert_intercode_operation(InterCodesList, new_code);
        // code2
        translate_Stmt(childNode(root, 4));
        // [GOTO label3]
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = GOTO;
            new_code->u.sinop.op = label3; 
        }
        insert_intercode_operation(InterCodesList, new_code);
        // [LABEL label2]
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SET_LABEL;
            new_code->u.sinop.op = label2; 
        }
        insert_intercode_operation(InterCodesList, new_code);
        // code3
        translate_Stmt(childNode(root, 6));
        // [LABEL label3]
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SET_LABEL;
            new_code->u.sinop.op = label3; 
        }
        insert_intercode_operation(InterCodesList, new_code);
    }
    
    //-> WHILE LP Exp RP Stmt
    if (count_node == 5 && strcmp(childNode(root, 0)->name, "WHILE") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0 && strcmp(childNode(root, 3)->name, "RP") == 0 && strcmp(childNode(root, 4)->name, "Stmt") == 0){
        Operand label1 = new_label();
        Operand label2 = new_label();
        Operand label3 = new_label();
        // [LABEL label1]
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SET_LABEL;
            new_code->u.sinop.op = label1; 
        }
        insert_intercode_operation(InterCodesList, new_code);
        // code1
        translate_Cond(childNode(root, 2), label2, label3);
        // [LABEL label2]
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SET_LABEL;
            new_code->u.sinop.op = label2; 
        }
        insert_intercode_operation(InterCodesList, new_code);
        // code2
        translate_Stmt(childNode(root, 4));
        // [GOTO label1]
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = GOTO;
            new_code->u.sinop.op = label1; 
        }
        insert_intercode_operation(InterCodesList, new_code);
        // [LABEL label3]
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SET_LABEL;
            new_code->u.sinop.op = label3; 
        }
        insert_intercode_operation(InterCodesList, new_code);
    }
    return;
}



//**************************************************************
//*                     Local Definitions                      *
//**************************************************************
/*
    TODO:
        lots of things
*/
void translate_DefList(TreeNode* root){
    if (root == NULL)
        return;
    int count_node = countChild(root);
    
    //-> Def DefList
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "Def") == 0 && strcmp(childNode(root, 1)->name, "DefList") == 0){
        translate_Def(childNode(root, 0));
        translate_DefList(childNode(root, 1));
    }
    //-> *empty*
    return;
}

void translate_Def(TreeNode* root){
    if (root == NULL)
        return;
    int count_node = countChild(root);
     
    //-> Specifier DecList SEMI
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Specifier") == 0 && strcmp(childNode(root, 1)->name, "DecList") == 0 && strcmp(childNode(root, 2)->name, "SEMI") == 0){
        translate_DecList(childNode(root, 1));
    }
    return;
}

void translate_DecList(TreeNode* root){
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);
     
    //-> Dec
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "Dec") == 0){
        translate_Dec(childNode(root, 0));
    }
    
    //-> Dec COMMA DecList
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Dec") == 0 && strcmp(childNode(root, 1)->name, "COMMA") == 0 && strcmp(childNode(root, 2)->name, "DecList") == 0){
        translate_Dec(childNode(root, 0));
        translate_DecList(childNode(root, 2));
    }
    return;
}

void translate_Dec(TreeNode* root){
    if (root == NULL)
        return;
    int count_node = countChild(root);
    
    //-> VarDec
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "VarDec") == 0){
        translate_VarDec(childNode(root, 0), false);
    }
    
    //-> VarDec ASSIGNOP Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "VarDec") == 0 && strcmp(childNode(root, 1)->name, "ASSIGNOP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        translate_VarDec(childNode(root, 0), false);
        
        translate_Exp(childNode(root, 2), 1/*temp_variable*/);
    }
    return;
}

//**************************************************************
//*                     Expressions                            *
//**************************************************************
/*
    TODO
        translate_Exp: 
            structure & array 
            mind pointer in ID(ARGS)
        translate_Exp_Left:
            except ID
*/

void translate_Exp(TreeNode* root, Operand place){
    if (root == NULL)
        return;
    int count_node = countChild(root);

    //-> Exp ASSIGNOP Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "ASSIGNOP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Operand left = translate_Exp_left(childNode(root, 0));
        Operand t1 = new_temp();
        translate_Exp(childNode(root, 2), t1);
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = ASSIGN;
            new_code->u.assign.left = left;
            new_code->u.assign.right = t1;
        }
        insert_intercode_operation(InterCodesList, new_code);
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = ASSIGN;
            new_code->u.assign.left = place;
            new_code->u.assign.right = left;
        }
        insert_intercode_operation(InterCodesList, new_code);
    }
        
    //-> Exp PLUS Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "PLUS") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Operand t1 = new_temp();
        Operand t2 = new_temp();
        translate_Exp(childNode(root, 0), t1);
        translate_Exp(childNode(root, 2), t2);
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = ADD;
            new_code->u.binop.op1 = t1;
            new_code->u.binop.op2 = t2;
            new_code->u.binop.result = place;
        }
        insert_intercode_operation(InterCodesList, new_code);
    }
    
    //-> Exp MINUS Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "MINUS") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Operand t1 = new_temp();
        Operand t2 = new_temp();
        translate_Exp(childNode(root, 0), t1);
        translate_Exp(childNode(root, 2), t2);
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SUB;
            new_code->u.binop.op1 = t1;
            new_code->u.binop.op2 = t2;
            new_code->u.binop.result = place;
        }
        insert_intercode_operation(InterCodesList, new_code);
    }
    
    //-> Exp STAR Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "STAR") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Operand t1 = new_temp();
        Operand t2 = new_temp();
        translate_Exp(childNode(root, 0), t1);
        translate_Exp(childNode(root, 2), t2);
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = MUL;
            new_code->u.binop.op1 = t1;
            new_code->u.binop.op2 = t2;
            new_code->u.binop.result = place;
        }
        insert_intercode_operation(InterCodesList, new_code);        
    }
    
    //-> Exp DIV Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "DIV") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Operand t1 = new_temp();
        Operand t2 = new_temp();
        translate_Exp(childNode(root, 0), t1);
        translate_Exp(childNode(root, 2), t2);
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = DIV;
            new_code->u.binop.op1 = t1;
            new_code->u.binop.op2 = t2;
            new_code->u.binop.result = place;
        }
        insert_intercode_operation(InterCodesList, new_code);       
    }
    
    //-> LP Exp RP
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "LP") == 0 && strcmp(childNode(root, 1)->name, "Exp") == 0 && strcmp(childNode(root, 2)->name, "RP") == 0){
        translate_Exp(childNode(root, 1), place);
    }
    
    //-> MINUS Exp
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "MINUS") == 0 && strcmp(childNode(root, 1)->name, "Exp") == 0){
        Operand t1 = new_temp();
        translate_Exp(childNode(root, 1), t1);
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SUB;
            new_code->u.binop.op1 = temp_zero;
            new_code->u.binop.op2 = t1;
            new_code->u.binop.result = place;
        }
        insert_intercode_operation(InterCodesList, new_code);
    }

    //-> Exp AND Exp
    //-> Exp OR Exp
    //-> Exp RELOP Exp
    //-> NOT Exp
    if ((count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "AND") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0)
    || (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "OR") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0)
    || (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "RELOP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0)
    || (count_node == 2 && strcmp(childNode(root, 0)->name, "NOT") == 0 && strcmp(childNode(root, 1)->name, "Exp") == 0)){
        Operand label1 = new_label();
        Operand label2 = new_label();
        // code0 = [place := #0]
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = ASSIGN;
            new_code->u.assign.left = place; 
            new_code->u.assign.right = temp_zero;
        }
        insert_intercode_operation(InterCodesList, new_code);
        translate_Cond(root, label1, label2);
        // code1 = [label label1]
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SET_LABEL;
            new_code->u.sinop.op = label1; 
        }
        insert_intercode_operation(InterCodesList, new_code);
        // code2 = [place := #1]
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = ASSIGN;
            new_code->u.assign.left = place; 
            new_code->u.assign.right = temp_one;
        }
        insert_intercode_operation(InterCodesList, new_code);
        // code3 = [LABEL label2]
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SET_LABEL;
            new_code->u.sinop.op = label2; 
        }
        insert_intercode_operation(InterCodesList, new_code);
    }
    
 
    //-> ID LP Args RP
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "ID") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "Args") == 0 && strcmp(childNode(root, 3)->name, "RP") == 0){
        char *current_func_name = childNode(root, 0)->id_val;
        Operand right = (Operand)malloc(sizeof(Operand_));
        {
            right->kind = FUNCTION;
            strcpy(right->u.value, current_func_name);
        }
        // arg_list = NULL
        ArgList arg_list = (ArgList)malloc(sizeof(ArgList_));
        {
            arg_list->param = NULL;
            arg_list->next = NULL;
        }
        translate_Args(childNode(root, 2), arg_list);
        if (strcmp(current_func_name, "write") == 0){
            // [WRITE arg_list[1]]
            InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
            {
                new_code->kind = WRITE;
                /* mind pointer */
                new_code->u.sinop.op = arg_list->next->param; 
            }
            insert_intercode_operation(InterCodesList, new_code);
            // [place := #0]
            InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
            {
                new_code->kind = ASSIGN;
                new_code->u.assign.left = place; 
                new_code->u.assign.right = temp_zero;
            }
            insert_intercode_operation(InterCodesList, new_code);
        } else {
            ArgList ptr = arg_list->next;
            InterCode new_code;
            while (ptr != 0){
                new_code = (InterCode)malloc(sizeof(InterCode_));
                {
                    new_code->kind = ARG;
                    new_code->u.sinop.op = ptr->param; 
                }
                insert_intercode_operation(InterCodesList, new_code);
                ptr = ptr->next;
            }
            // [place := CALL function.name]
            new_code = (InterCode)malloc(sizeof(InterCode_));
            {
                new_code->kind = CALL;
                new_code->u.assign.left = place; 
                new_code->u.assign.right = right;
            }
            insert_intercode_operation(InterCodesList, new_code);
        }
    }
    
    //-> ID LP RP
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "ID") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "RP") == 0){
        char *current_func_name = childNode(root, 0)->id_val;
        Operand right = (Operand)malloc(sizeof(Operand_));
        {
            right->kind = FUNCTION;
            strcpy(right->u.value, current_func_name);
        }
        if (strcmp(current_func_name, "read") == 0){
            InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
            {
                new_code->kind = READ;
                new_code->u.sinop.op = place; 
            }
            insert_intercode_operation(InterCodesList, new_code);
        } else {
            InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
            {
                new_code->kind = CALL;
                new_code->u.assign.left = place; 
                new_code->u.assign.right = right;
            }
            insert_intercode_operation(InterCodesList, new_code);
        }
    }
    
    //-> Exp LB Exp RB
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "LB") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0 && strcmp(childNode(root, 3)->name, "RB") == 0){
        /* TODO */
    }
    
    
    //-> Exp DOT ID
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "DOT") == 0 && strcmp(childNode(root, 2)->name, "ID") == 0){
        /* TODO */
    }
    
    //-> ID
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ID") == 0){
        char *current_var_name = childNode(root, 0)->id_val;
        Operand right = (Operand)malloc(sizeof(Operand_));
        {
            right->kind = VARIABLE;
            strcpy(right->u.value, current_var_name);
        }
        InterCode new_code = (InterCode)malloce(sizeof(InterCode_));
        {
            new_code->kind = ASSIGN;
            new_code->u.assign.left = place;
            new_code->u.assign.right = right;
        }
        insert_intercode_operation(InterCodesList, new_code);
    }
    
    //-> INT
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "INT") == 0){
        unsigned int value = childNode(root, 0)->int_val;
        Operand right = (Operand)malloc(sizeof(Operand_));
        {
            right->kind = CONSTANT;
            right->u.int_val = value;
        }
        InterCode new_code = (InterCode)malloce(sizeof(InterCode_));
        {
            new_code->kind = ASSIGN;
            new_code->u.assign.left = place;
            new_code->u.assign.right = right;
        }
        insert_intercode_operation(InterCodesList, new_code);
    }
    
    return;
}

void translate_Args(TreeNode* root, ArgList arg_list){
    if (root == NULL)
        return;
    int count_node = countChild(root);
    
    //-> Exp COMMA Args
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "COMMA") == 0 && strcmp(childNode(root, 2)->name, "Args") == 0){
        Operand t1 = new_temp();
        translate_Exp(childNode(root, 0), t1);
        // arg_list = t1 + arg_list
        ArgList new_arg = (ArgList)malloc(sizeof(ArgList_));
        {
            new_arg->param = t1;
            new_arg->next = arg_list->next;   
        }
        arg_list->next = new_arg;
        // 
        translate_Args(childNode(root, 2), arg_list);
    }
    
    //-> Exp
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "Exp") == 0){
        Operand t1 = new_temp();
        translate_Exp(childNode(root, 0), t1);
        // arg_list = t1 + arg_list
        ArgList new_arg = (ArgList)malloc(sizeof(ArgList_));
        {
            new_arg->param = t1;
            new_arg->next = arg_list->next;   
        }
        arg_list->next = new_arg;
    }
    return;
}

void translate_Cond(TreeNode* root, Operand label_true, Operand label_false){
    if (root == NULL)
        return;
    int count_node = countChild(root);

    //-> Exp AND Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "AND") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Operand label1 = new_label();
        // code1
        translate_Cond(childNode(root, 0), label1, label_false);
        // [LABEL label1]
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SET_LABEL;
            new_code->u.sinop.op = label1; 
        }
        insert_intercode_operation(InterCodesList, new_code);
        // code2
        translate_Cond(childNode(root, 2), label_true, label_false);
        return;
    }
    
    //-> Exp OR Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "OR") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Operand label1 = new_label();
        // code1
        translate_Cond(childNode(root, 0), label_true, label1);
        // [LABEL label1]
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = SET_LABEL;
            new_code->u.sinop.op = label1; 
        }
        insert_intercode_operation(InterCodesList, new_code);
        // code2
        translate_Cond(childNode(root, 2), label_true, label_false);
        return;
    }
    
    //-> Exp RELOP Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "RELOP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Operand t1 = new_temp();
        Operand t2 = new_temp();
        translate_Exp(childNode(root, 0), t1);
        translate_Exp(childNode(root, 2), t2);
        char *current_rel_name = childNode(root, 1)->type_val;
        Operand relop = (Operand)malloc(sizeof(Operand_));
        {
            relop->kind = RELOP;
            strcpy(relop->u.value, current_rel_name);
        }
        // [IF t1 op t2 GOTO label_true]
        InterCode new_code = (InterCode)malloce(sizeof(InterCode_));
        {
            new_code->kind = IFGOTO;
            new_code->u.ifgoto_op.op1 = t1;
            new_code->u.ifgoto_op.op2 = t2;
            new_code->u.ifgoto_op.relop = relop;
            new_code->u.ifgoto_op.label = label_true;
        }
        insert_intercode_operation(InterCodesList, new_code);
        // [GOTO label_false]
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = GOTO;
            new_code->u.sinop.op = label_false; 
        }
        insert_intercode_operation(InterCodesList, new_code);
        return;
    }

    //-> NOT Exp
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "NOT") == 0 && strcmp(childNode(root, 1)->name, "Exp") == 0){
        translate_Cond(childNode(root, 1), label_false, label_true);
        return;
    }

    // (other cases)
    Operand t1 = new_temp();
    translate_Exp(root, t1);
    Operand relop = (Operand)malloc(sizeof(Operand_));
    {
        relop->kind = RELOP;
        strcpy(relop->u.value, "!=");
    }
    // [IF t1 != #0 GOTO label_true]
    InterCode new_code = (InterCode)malloce(sizeof(InterCode_));
    {
        new_code->kind = IFGOTO;
        new_code->u.ifgoto_op.op1 = t1;
        new_code->u.ifgoto_op.op2 = temp_zero;
        new_code->u.ifgoto_op.relop = relop;
        new_code->u.ifgoto_op.label = label_true;
    }
    insert_intercode_operation(InterCodesList, new_code);
    // [GOTO label_false]
    new_code = (InterCode)malloc(sizeof(InterCode_));
    {
        new_code->kind = GOTO;
        new_code->u.sinop.op = label_false; 
    }
    insert_intercode_operation(InterCodesList, new_code);
    return;
}

Operand translate_Exp_left(TreeNode* root){
    if (root == NULL)
        return;
    int count_node = countChild(root);

    //-> Exp LB Exp RB
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "LB") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0 && strcmp(childNode(root, 3)->name, "RB") == 0){
        return true;
    }
    //-> Exp DOT ID
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "DOT") == 0 && strcmp(childNode(root, 2)->name, "ID") == 0){
        
    }
    //-> ID
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ID") == 0){
        char *current_var_name = childNode(root, 0)->id_val;
        Operand current_var = (Operand)malloc(sizeof(Operand_));
        {
            current_var->kind = VARIABLE;
            strcpy(current_var->u.value, current_var_name);
        }    
        return current_var;
    }
    return NULL;
}