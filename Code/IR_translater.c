#include "IR_translater.h"

//#define DEBUG_LAB3

InterCodes InterCodesList;
static Operand temp_zero, temp_one, temp_four;
static int temp_index, label_index;
extern FILE *fp;

static void initial(){
    temp_index = 1;
    label_index = 1;
    InterCodesList = (InterCodes)malloc(sizeof(InterCodes_));
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
    temp_four = (Operand)malloc(sizeof(Operand_));
    {
        temp_four->kind = CONSTANT;
        temp_four->u.int_val = 4;
    }
    return;
}

//==============================================================

Operand new_temp(){
#ifdef DEBUG_LAB3
    printf("new temp\n");
#endif
    char str[20];
    int tmp = temp_index;
    int i = 1;
    str[0] = 't';
    while (tmp != 0){
        int x = tmp % 10;
        str[i] = (char)('0' + x);
        i++; 
        tmp = tmp / 10;
    }
    str[i] = '\0';
    Operand temp = (Operand)malloc(sizeof(Operand_));
    {
        temp->kind = TEMP;
        strcpy(temp->u.value, str);
    }
    temp_index++;
    return temp;
}
Operand new_label(){
    char str[32];
    int tmp = label_index;
    int i = 1;
    str[0] = 'L';
    while (tmp != 0){
        int x = tmp % 10;
        str[i] = (char)('0' + x);
        i++; tmp = tmp / 10;
    }
    str[i] = '\0';
    Operand label = (Operand)malloc(sizeof(Operand_));
    {
        label->kind = LABEL;
        strcpy(label->u.value, str);
    }
    label_index++;
    return label;
}
void insert_intercode_operation(InterCodes intercodes_head, InterCode new_intercode){
    InterCodes ptr = InterCodesList;
    while (ptr->next != NULL){
        ptr = ptr->next;
    }
    InterCodes new_code = (InterCodes)malloc(sizeof(InterCodes_));
    {
        new_code->code = new_intercode;
        new_code->prev = ptr;
        new_code->next = NULL;
    }
    ptr->next = new_code;
    return;
}

//=========================================================================
void show_Op(Operand op){
    if (op == NULL)
        return;
    switch (op->kind)
    {
    case CONSTANT:fprintf(fp,"#%d",op->u.int_val);break;
    case RELOP:fprintf(fp," %s ",op->u.value);break;
    
    default:fprintf(fp,"%s",op->u.value);break;
    }
    return;
}

void show_intercode(InterCode code){
    if (code == NULL)
        return;
    switch (code->kind)
    {
    case SET_LABEL:
        fprintf(fp, "LABEL %s :\n", code->u.sinop.op->u.value);
        break;
    case FUNCTION_ENTRY:
        fprintf(fp, "FUNCTION %s :\n", code->u.sinop.op->u.value);
        break;
    case ASSIGN:
        if (code->u.assign.left == NULL)
            break;
        show_Op(code->u.assign.left);
        fprintf(fp, " := ");
        show_Op(code->u.assign.right);
        fprintf(fp, "\n");
        break;
    case ADD:
    case SUB:
    case MUL:
    case DIV:
        show_Op(code->u.binop.result);
        fprintf(fp, " := ");
        show_Op(code->u.binop.op1);
        switch (code->kind)
        {
        case ADD: fprintf(fp, " + ");break;
        case SUB: fprintf(fp, " - ");break;
        case MUL: fprintf(fp, " * ");break;
        case DIV: fprintf(fp, " / ");break;
        default:break;
        }
        show_Op(code->u.binop.op2);
        fprintf(fp, "\n");
        break;
    case GET_ADDR:
        if (code->u.assign.left == NULL)
            break;
        show_Op(code->u.assign.left);
        fprintf(fp, " := &");
        show_Op(code->u.assign.right);
        fprintf(fp, "\n");
        break;
    case GET_VALUE:
        if (code->u.assign.left == NULL)
            break;
        show_Op(code->u.assign.left);
        fprintf(fp, " := *");
        show_Op(code->u.assign.right);
        fprintf(fp, "\n");
        break;
    case ASSIGN_ADDR:
        if (code->u.assign.left == NULL)
            break;
        fprintf(fp, "*");
        show_Op(code->u.assign.left);
        fprintf(fp, " := ");
        show_Op(code->u.assign.right);
        fprintf(fp, "\n");
        break;
        break;
    case GOTO:
        fprintf(fp, "GOTO ");
        show_Op(code->u.sinop.op);
        fprintf(fp, "\n");
        break;
    case IFGOTO: 
        fprintf(fp, "IF ");
        show_Op(code->u.ifgoto_op.op1);
        show_Op(code->u.ifgoto_op.relop);
        show_Op(code->u.ifgoto_op.op2);
        fprintf(fp, " GOTO ");
        show_Op(code->u.ifgoto_op.label);
        fprintf(fp, "\n");
        break;
    case RETURN:
        fprintf(fp, "RETURN ");
        show_Op(code->u.sinop.op);
        fprintf(fp, "\n");
        break;
    case DEC:
        fprintf(fp, "DEC ");
        show_Op(code->u.dec_op.op);
        fprintf(fp, " %u\n", code->u.dec_op.size);
        break;
    case ARG:
        fprintf(fp, "ARG ");
        show_Op(code->u.sinop.op);
        fprintf(fp, "\n");
        break;
    case CALL:
        if (code->u.assign.left == NULL){
            Operand t1 = new_temp();
            code->u.assign.left = t1;
        }
        show_Op(code->u.assign.left);
        fprintf(fp, " := ");
        fprintf(fp, "CALL ");
        show_Op(code->u.assign.right);
        fprintf(fp, "\n");
        break;
    case PARAM:
        fprintf(fp, "PARAM ");
        show_Op(code->u.sinop.op);
        fprintf(fp, "\n");
        break;
    case READ:
        fprintf(fp, "READ ");
        show_Op(code->u.sinop.op);
        fprintf(fp, "\n");
        break;
    case WRITE:
        fprintf(fp, "WRITE ");
        show_Op(code->u.sinop.op);
        fprintf(fp, "\n");
        break;
    
    default:
        break;
    }
}

void show_IR(){
    InterCodes ptr = InterCodesList->next;
    while (ptr != NULL){
#ifdef DEBUG_LAB3
    printf("print one\n");
#endif 
        show_intercode(ptr->code);
#ifdef DEBUG_LAB3
    printf("successfully\n");
#endif
        ptr = ptr->next;
    }
}

//==============================================================
/*
    TODO
        calculate_offset    [ok]
        calculate_size      [ok]
        change_dec_size     [ok]
*/

bool isArray(TreeNode* root){
    if (root == NULL)
        return false;
    int count_node = countChild(root);

    return (count_node == 4 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "LB") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0 && strcmp(childNode(root, 3)->name, "RB") == 0);
}
unsigned int calculate_offset(unsigned int offset, FieldList tmp_field, char *field_name){
    if (strcmp(tmp_field->name, field_name) == 0)
        return offset;
    else 
        return calculate_offset(offset + calculate_size(tmp_field->type), tmp_field->tail, field_name);
    return 0;
}
unsigned int calculate_size(Type t){
    if (t->kind == BASIC)
        return 4;
    if (t->kind == ARRAY)
        return t->u.array.size * calculate_size(t->u.array.elem);
    FieldList ptr = t->u.structure;
    unsigned int ans = 0;
    while (ptr != NULL){
        ans += calculate_size(ptr->type);
        ptr = ptr->tail;
    }
    return ans;
}

void change_dec_size(InterCodes intercodes_head, unsigned size){
    InterCodes ptr = intercodes_head;
    while (ptr->next != NULL){
        ptr  = ptr->next;
    }
    ptr = ptr->prev;
    if (ptr == NULL)
        return;
    ptr->code->u.dec_op.size *= size;
    return;
}



//**************************************************************
//*                     High-level Definitions                 *
//**************************************************************
/*
    COMPLETE
*/
InterCodes IR_Translater(TreeNode* root){
    initial();
    if (root == NULL)
        return NULL;
#ifdef DEBUG_LAB3
    printf("translate start up\n");
#endif
    //-> ExtDefList
    if (strcmp(childNode(root, 0)->name, "ExtDefList") == 0){
        translate_ExtDefList(childNode(root, 0));
    }
#ifdef DEBUG_LAB3
    printf("translate successfully---------\n");
#endif
    //show_IR();
    return InterCodesList;
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
    COMPLETE

    TODO: 
        translate_FunDec    complete
        translate_VarDec:
            in_function:
                normal      [ok]
                sturct      [ok]
            not_params:
                normal      [ok]
                struct      [ok]
                array       [ok]

*/
Operand translate_VarDec(TreeNode* root, bool is_function_param, Type t){
#ifdef DEBUG_LAB3
    printf("tanslate_FunDec\n");
#endif
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);

    //-> ID
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ID") == 0){
        char *current_var_name = childNode(root, 0)->id_val;
        Operand tmp = (Operand)malloc(sizeof(Operand_));
        {
            tmp->kind = IR_VARIABLE;
            strcpy(tmp->u.value, current_var_name);
        }

        if (is_function_param){
            // [PARAM ID]
            InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
            {
                new_code->kind = PARAM;
                new_code->u.sinop.op = tmp;
            }
            insert_intercode_operation(InterCodesList, new_code);
            return NULL;
        } else {
            if (t->kind != STRUCTURE)
                return tmp;
            // [DEC t1 [size]]
            Operand t1 = new_temp();
            InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
            {
                new_code->kind = DEC;
                new_code->u.dec_op.op = t1;
                new_code->u.dec_op.size = calculate_size(t);
            }
            insert_intercode_operation(InterCodesList, new_code);

            // [ID := &t1]
            new_code = (InterCode)malloc(sizeof(InterCode_));
            {
                new_code->kind = GET_ADDR;
                new_code->u.assign.left = tmp;
                new_code->u.assign.right = t1;
            }
            insert_intercode_operation(InterCodesList, new_code);
            /* TODO */
            return NULL;
        } 
    
    }
    
    //-> VarDec LB INT RB
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "VarDec") == 0 && strcmp(childNode(root, 1)->name, "LB") == 0 && strcmp(childNode(root, 2)->name, "INT") == 0 && strcmp(childNode(root, 3)->name, "RB") == 0){
        if (is_function_param){
            printf("Cannot translate: Code contains illegal function parameters.\n");
            assert(0);
            return NULL;
        } else {
            TreeNode * next_root = childNode(root, 0);
            if (countChild(next_root) != 1){
               printf("Cannot translate: Code contains multi-dimension array.\n");
               assert(0);
               return NULL; 
            }
            unsigned int size = childNode(root, 2)->int_val;
            
            Operand var = translate_VarDec(childNode(root, 0), is_function_param, t);
            if (t->kind == STRUCTURE){
                change_dec_size(InterCodesList, size);
                return NULL;
            }
            // [DEC ID [size]]
            InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
            {
                new_code->kind = DEC;
                new_code->u.dec_op.op = var;
                new_code->u.dec_op.size = size * 4;
            }
            insert_intercode_operation(InterCodesList, new_code);
            return NULL;
        }
        
    }
    
    return NULL;
}

void translate_FunDec(TreeNode* root){   
#ifdef DEBUG_LAB3
    printf("tanslate_FunDec\n");
#endif
    if (root == NULL)
        return;
    int count_node = countChild(root);

    //-> ID LP VarList RP
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "ID") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "VarList") == 0 && strcmp(childNode(root, 3)->name, "RP") == 0){
        char *current_func_name = childNode(root, 0)->id_val;
        Operand func = (Operand)malloc(sizeof(Operand_));
        {
            func->kind = IR_FUNCTION;
            strcpy(func->u.value, current_func_name);
        }
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = FUNCTION_ENTRY;
            new_code->u.sinop.op = func;
        }
        insert_intercode_operation(InterCodesList, new_code);
        translate_VarList(childNode(root, 2));
    }

    //-> ID LP RP
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "ID") == 0 && strcmp(childNode(root, 1)->name, "LP") == 0 && strcmp(childNode(root, 2)->name, "RP") == 0){
        char *current_func_name = childNode(root, 0)->id_val;
        Operand func = (Operand)malloc(sizeof(Operand_));
        {
            func->kind = IR_FUNCTION;
            strcpy(func->u.value, current_func_name);
        }
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = FUNCTION_ENTRY;
            new_code->u.sinop.op = func;
        }
        insert_intercode_operation(InterCodesList, new_code);
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
        return;
    int count_node = countChild(root);

    //-> Specifier VarDec
    if (count_node == 2 && strcmp(childNode(root, 0)->name, "Specifier") == 0 && strcmp(childNode(root, 1)->name, "VarDec") == 0){
	    Type t = Specifier(childNode(root, 0));
        translate_VarDec(childNode(root, 1), true, t);
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
#ifdef DEBUG_LAB3
    printf("tanslate_CompSt\n");
#endif
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
#ifdef DEBUG_LAB3
    printf("tanslate_StmtList\n");
#endif
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
#ifdef DEBUG_LAB3
    printf("tanslate_Stmt\n");
#endif
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
    COMPLETE

    TODO:
        translate_Dec:      [ok]

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
        Type t = Specifier(childNode(root, 0));
        translate_DecList(childNode(root, 1), t);
    }
    return;
}

void translate_DecList(TreeNode* root, Type t){
    if (root == NULL)
        return;
    int count_node = countChild(root);
     
    //-> Dec
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "Dec") == 0){
        translate_Dec(childNode(root, 0), t);
    }
    
    //-> Dec COMMA DecList
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Dec") == 0 && strcmp(childNode(root, 1)->name, "COMMA") == 0 && strcmp(childNode(root, 2)->name, "DecList") == 0){
        translate_Dec(childNode(root, 0), t);
        translate_DecList(childNode(root, 2), t);
    }
    return;
}

void translate_Dec(TreeNode* root, Type t){
    if (root == NULL)
        return;
    int count_node = countChild(root);
    
    //-> VarDec
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "VarDec") == 0){
        translate_VarDec(childNode(root, 0), false, t);
    }
    
    //-> VarDec ASSIGNOP Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "VarDec") == 0 && strcmp(childNode(root, 1)->name, "ASSIGNOP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Operand tmp = translate_VarDec(childNode(root, 0), false, t);
        Operand t1 = new_temp();
        translate_Exp(childNode(root, 2), t1/*temp_variable*/);
        if (tmp != NULL){
            // [tmp := t1]
            InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
            {
                new_code->kind = ASSIGN;
                new_code->u.assign.left = tmp;
                new_code->u.assign.right = t1;
            }
            insert_intercode_operation(InterCodesList, new_code);
        } else {
            /* TODO */
            printf("Cannot translate: Codes contain illegal initialization.\n");
            assert(0);
            return;
        }
    }
    return;
}

//**************************************************************
//*                     Expressions                            *
//**************************************************************
/*
    COMPLETE
    TODO:
        translate_Exp: 
            structure & array               [ok]
            mind pointer in ID(ARGS)        [ok]
        translate_Exp_Left:
            array                           [ok]
            structure                       [ok]
*/
/*
    WARN:
        translate_Exp_left:
            1. use the function 'Exp' which will print error information
            2. struct get_addr/assign
*/


void translate_Exp(TreeNode* root, Operand place){
#ifdef DEBUG_LAB3
    printf("tanslate_Exp\n");
#endif
    if (root == NULL)
        return;
    int count_node = countChild(root);

    //-> Exp ASSIGNOP Exp
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "ASSIGNOP") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0){
        Operand t0 = new_temp();
        Operand left = translate_Exp_left(childNode(root, 0), t0);
        Operand t1 = new_temp();
        translate_Exp(childNode(root, 2), t1);
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            if (left != NULL){
                new_code->kind = ASSIGN;
                new_code->u.assign.left = left;
            } else {
                new_code->kind = ASSIGN_ADDR;
                new_code->u.assign.left = t0;
            }
            new_code->u.assign.right = t1;
        }
        insert_intercode_operation(InterCodesList, new_code);
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = ASSIGN;
            new_code->u.assign.left = place;
            new_code->u.assign.right = t1;
            
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
            right->kind = IR_FUNCTION;
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
            new_code = (InterCode)malloc(sizeof(InterCode_));
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
            right->kind = IR_FUNCTION;
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
        Operand t1 = new_temp();
        translate_Exp_left(root, t1);
        // [place := *t1]
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = GET_VALUE;
            new_code->u.assign.left = place;
            new_code->u.assign.right = t1;
        }
        insert_intercode_operation(InterCodesList, new_code);
    }
    
    
    //-> Exp DOT ID
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "DOT") == 0 && strcmp(childNode(root, 2)->name, "ID") == 0){
        /* TODO */
        Operand t1 = new_temp();
        translate_Exp_left(root, t1);
        // [place := *t1]
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = GET_VALUE;
            new_code->u.assign.left = place;
            new_code->u.assign.right = t1;
        }
        insert_intercode_operation(InterCodesList, new_code);
    }
    
    //-> ID
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ID") == 0){
        char *current_var_name = childNode(root, 0)->id_val;
        Operand right = (Operand)malloc(sizeof(Operand_));
        {
            right->kind = IR_VARIABLE;
            strcpy(right->u.value, current_var_name);
        }
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
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
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
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
        Type t = Exp(childNode(root, 0));
        if (t->kind == BASIC){
            translate_Exp(childNode(root, 0), t1);
        } else {
            Operand var = translate_Exp_left(childNode(root, 0), t1);
            if (var != NULL)
                t1 = var;
        }
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
        Type t = Exp(childNode(root, 0));
        if (t->kind == BASIC){
            translate_Exp(childNode(root, 0), t1);
        } else {
            Operand var = translate_Exp_left(childNode(root, 0), t1);
            if (var != NULL)
                t1 = var;
        }
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
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
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
    InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
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

Operand translate_Exp_left(TreeNode* root, Operand place){
    if (root == NULL)
        return NULL;
    int count_node = countChild(root);

    //-> Exp LB Exp RB
    if (count_node == 4 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "LB") == 0 && strcmp(childNode(root, 2)->name, "Exp") == 0 && strcmp(childNode(root, 3)->name, "RB") == 0){
        // [t1 := &var]
        Operand t1 = new_temp(); 
        Operand var = translate_Exp_left(childNode(root, 0), t1);
        
        if (isArray(childNode(root, 0))){
            printf("Cannot translate: Code contains variables of multi-dimensional array type.\n");
            assert(0);
            return NULL;
        }
        // to calculate Exp2, and place it in t2
        Operand t2 = new_temp();
        translate_Exp(childNode(root, 2), t2);
        // to calculate size
        Type t = Exp(childNode(root, 0));
        Operand size = (Operand)malloc(sizeof(Operand_));
        {
            size->kind = CONSTANT;
            size->u.int_val = calculate_size(t->u.array.elem);
        }
        // [t3 := t2 * size]
        Operand t3 = new_temp();
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = MUL;
            new_code->u.binop.result = t3;
            new_code->u.binop.op1 = t2;
            new_code->u.binop.op2 = size;
        }
        insert_intercode_operation(InterCodesList, new_code);
        // [place := t1 + t3]
        new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = ADD;
            new_code->u.binop.result = place;
            new_code->u.binop.op1 = t1;
            new_code->u.binop.op2 = t3;
        }
        insert_intercode_operation(InterCodesList, new_code);
        return NULL;
    }
    
    //-> Exp DOT ID
    if (count_node == 3 && strcmp(childNode(root, 0)->name, "Exp") == 0 && strcmp(childNode(root, 1)->name, "DOT") == 0 && strcmp(childNode(root, 2)->name, "ID") == 0){
        // [t1 := &var]
        Operand t1 = new_temp(); 
        Operand var = translate_Exp_left(childNode(root, 0), t1);
        
        // calculate the offset
        Type temp_type = Exp(childNode(root, 0));
        char * current_field_name = childNode(root, 2)->id_val;
        unsigned int size = calculate_offset(0, temp_type->u.structure, current_field_name);
        Operand offset = (Operand)malloc(sizeof(Operand_));
        {
            offset->kind = CONSTANT;
            offset->u.int_val = size;
        }
        // [place := t1 + offset]
        InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
        {
            new_code->kind = ADD;
            new_code->u.binop.result = place;
            new_code->u.binop.op1 = t1;
            new_code->u.binop.op2 = offset;
        }
        insert_intercode_operation(InterCodesList, new_code);

        return NULL;
    }
    
    //-> ID
    if (count_node == 1 && strcmp(childNode(root, 0)->name, "ID") == 0){
        char *current_var_name = childNode(root, 0)->id_val;
        Operand current_var = (Operand)malloc(sizeof(Operand_));
        {
            current_var->kind = IR_VARIABLE;
            strcpy(current_var->u.value, current_var_name);
        }  

        Type t = Exp(root);
        if (t->kind == BASIC)
            return current_var;
        if (t->kind == ARRAY && t->u.array.elem->kind == BASIC){
            // [place := &var]
            InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
            {
                new_code->kind = GET_ADDR;
                new_code->u.assign.left = place;
                new_code->u.assign.right = current_var;
            }
            insert_intercode_operation(InterCodesList, new_code);
            return NULL;
        } else {
            // [place := var]
            InterCode new_code = (InterCode)malloc(sizeof(InterCode_));
            {
                new_code->kind = ASSIGN;
                new_code->u.assign.left = place;
                new_code->u.assign.right = current_var;
            }
            insert_intercode_operation(InterCodesList, new_code);
            return NULL;
        }
    }
    return NULL;
}
