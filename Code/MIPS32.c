#include "MIPS32.h"
//#define DEBUG_lab3

VariableList varListHead;
extern FILE *out_file;

//============= PROGRAM INITIALIZATION ==================
void __initialize__(){
    fprintf(out_file,".data\n");
    fprintf(out_file,"_prompt: .asciiz \"Enter an integer:\"\n");
    fprintf(out_file,"_ret: .asciiz \"\\n\"\n");
    fprintf(out_file,".globl main\n");
    fprintf(out_file,".text\n");

    /* READ FUNCTION*/
    fprintf(out_file,"read:\n");
    fprintf(out_file,"li $v0, 4\n");
    fprintf(out_file,"la $a0, _prompt\n");
    fprintf(out_file,"syscall\n");
    fprintf(out_file,"li $v0, 5\n");
    fprintf(out_file,"syscall\n");
    fprintf(out_file,"jr $ra\n");
    fprintf(out_file,"\n");

    /* WRITE FUNCTION*/
    fprintf(out_file,"write:\n");
    fprintf(out_file,"li $v0, 1\n");
    fprintf(out_file,"syscall\n");
    fprintf(out_file,"li $v0, 4\n");
    fprintf(out_file,"la $a0, _ret\n");
    fprintf(out_file,"syscall\n");
    fprintf(out_file,"move $v0, $0\n");
    fprintf(out_file,"jr $ra\n\n");

    return;
}

void initialize_regs(){
    strcpy(regs[0].name,"$zero");
    strcpy(regs[1].name,"$at");
    strcpy(regs[2].name,"$v0");
    strcpy(regs[3].name,"$v1");
    strcpy(regs[4].name,"$a0");
    strcpy(regs[5].name,"$a1");
    strcpy(regs[6].name,"$a2");
    strcpy(regs[7].name,"$a3");
    strcpy(regs[8].name,"$t0");
    strcpy(regs[9].name,"$t1");
    strcpy(regs[10].name,"$t2");
    strcpy(regs[11].name,"$t3");
    strcpy(regs[12].name,"$t4");
    strcpy(regs[13].name,"$t5");
    strcpy(regs[14].name,"$t6");
    strcpy(regs[15].name,"$t7");
    strcpy(regs[16].name,"$s0");
    strcpy(regs[17].name,"$s1");
    strcpy(regs[18].name,"$s2");
    strcpy(regs[19].name,"$s3");
    strcpy(regs[20].name,"$s4");
    strcpy(regs[21].name,"$s5");
    strcpy(regs[22].name,"$s6");
    strcpy(regs[23].name,"$s7");
    strcpy(regs[24].name,"$t8");
    strcpy(regs[25].name,"$t9");
    strcpy(regs[26].name,"$k0");
    strcpy(regs[27].name,"$k1");
    strcpy(regs[28].name,"$gp");
    strcpy(regs[29].name,"$sp");
    strcpy(regs[30].name,"$s8");
    strcpy(regs[31].name,"$ra");

    for (int i = 0; i < 32; i++){
        regs[i].used = 0;
        regs[i].variable = NULL;
    }
    return;
}

//============= TRANSLATE INTERCODES ====================
void MIPS_translater(InterCodes InterCodesList){
    varListHead = (VariableList)malloc(sizeof(VariableList_));
    varListHead->next = NULL;

    __initialize__();
    initialize_regs();
    
    InterCodes ptr = InterCodesList->next;
    int left, right, op1, op2, dest, res;
    Operand label_op, relop_op;
    int count_lines = 0;
    while (ptr != NULL){
        InterCode tmp_code = ptr->code;
#ifdef DEBUG_lab3
        printf("%d\n", count_lines);
#endif
        count_lines++;
        switch (tmp_code->kind)
        {
            case SET_LABEL:
                fprintf(out_file,"%s:\n", tmp_code->u.sinop.op->u.value);
                break;
            case FUNCTION_ENTRY:
                #ifdef DEBUG_lab3
                printf("FUNC_enty\n");
                #endif
                translate_FUNCTION_ENTRY(ptr);
                break;
            case ASSIGN:
                if (tmp_code->u.assign.left == NULL)
                    break;
                left = assign_op2reg(tmp_code->u.assign.left, MIPS_VAR);
                right = assign_op2reg(tmp_code->u.assign.right, MIPS_VAR);
                fprintf(out_file, "move %s, %s\n", regs[left].name, regs[right].name);
                assign_reg2memeory(left);
                free_regs();
                break;
            case ADD:
            case SUB:
            case MUL:
            case DIV:
                dest = assign_op2reg(tmp_code->u.binop.result, MIPS_VAR);
                op1 = assign_op2reg(tmp_code->u.binop.op1, MIPS_VAR);
                op2 = assign_op2reg(tmp_code->u.binop.op2, MIPS_VAR);
                switch (tmp_code->kind){
                    case ADD:
                        fprintf(out_file, "add ");
                        break;
                    case SUB:
                        fprintf(out_file, "sub ");
                        break;
                    case MUL:
                        fprintf(out_file, "mul ");
                        break;
                    case DIV:
                        fprintf(out_file, "div ");
                        break;
                    default: break;
                }
                fprintf(out_file, "%s, %s, %s\n", regs[dest].name, regs[op1].name, regs[op2].name);
                assign_reg2memeory(dest);
                free_regs();
                break;
            case GET_ADDR:
                if (tmp_code->u.assign.left == NULL)
                    break;
                left = assign_op2reg(tmp_code->u.assign.left, MIPS_VAR);
                right = assign_op2reg(tmp_code->u.assign.right, MIPS_ADDRESS);
                fprintf(out_file, "move %s, %s\n", regs[left].name, regs[right].name);
                assign_reg2memeory(left);
                free_regs();
                break;
            case GET_VALUE:
                if (tmp_code->u.assign.left == NULL)
                    break;
                left = assign_op2reg(tmp_code->u.assign.left, MIPS_VAR);
                right = assign_op2reg(tmp_code->u.assign.right, MIPS_VALUE);
                fprintf(out_file, "move %s, %s\n", regs[left].name, regs[right].name);
                assign_reg2memeory(left);
                free_regs();
                break;
            case ASSIGN_ADDR:
                if (tmp_code->u.assign.left == NULL)
                    break;
                left = assign_op2reg(tmp_code->u.assign.left, MIPS_VAR);
                right = assign_op2reg(tmp_code->u.assign.right, MIPS_ADDRESS);
                fprintf(out_file, "sw %s, 0(%s)\n", regs[right].name, regs[left].name);
                free_regs();
                break;
            case GOTO:
                label_op = tmp_code->u.sinop.op;
                fprintf(out_file, "j %s\n", label_op->u.value);
                break;
            case IFGOTO: 
                op1 = assign_op2reg(tmp_code->u.ifgoto_op.op1, MIPS_VAR);
                op2 = assign_op2reg(tmp_code->u.ifgoto_op.op2, MIPS_VAR);
                relop_op = tmp_code->u.ifgoto_op.relop;
                label_op = tmp_code->u.ifgoto_op.label;
                if (strcmp(relop_op->u.value, "==") == 0){
                    fprintf(out_file, "beq ");
                } else if (strcmp(relop_op->u.value, "!=") == 0){
                    fprintf(out_file, "bne ");
                } else if (strcmp(relop_op->u.value, ">") == 0){
                    fprintf(out_file, "bgt ");
                } else if (strcmp(relop_op->u.value, "<") == 0){
                    fprintf(out_file, "blt ");
                } else if (strcmp(relop_op->u.value, ">=") == 0){
                    fprintf(out_file, "bge ");
                } else if (strcmp(relop_op->u.value, "<=") == 0){
                    fprintf(out_file, "ble ");
                }
                fprintf(out_file, "%s, %s, %s\n", regs[op1].name, regs[op2].name, label_op->u.value);
                free_regs();
                break;
            case RETURN:
                translate_RETURN(ptr);
                break;
            case ARG:
            case CALL:
                translate_CALL(ptr);
                break;
            case READ:
                fprintf(out_file, "addi $sp, $sp, -4\n");
                fprintf(out_file, "sw $ra, 0($sp)\n");
                fprintf(out_file, "jal read\n");
                fprintf(out_file, "lw $ra, 0($sp)\n");
                fprintf(out_file, "addi $sp, $sp, 4\n");
                
                dest = assign_op2reg(tmp_code->u.sinop.op, MIPS_VAR);
                fprintf(out_file, "move %s, $v0\n", regs[dest].name);
                assign_reg2memeory(dest);
                free_regs();
                break;
            case WRITE:
                res = assign_op2reg(tmp_code->u.sinop.op, MIPS_VAR);
                fprintf(out_file, "move $a0, %s\n", regs[res].name);
                fprintf(out_file, "addi $sp, $sp, -4\n");
                fprintf(out_file, "sw $ra, 0($sp)\n");
                fprintf(out_file, "jal write\n");
                fprintf(out_file, "lw $ra, 0($sp)\n");
                fprintf(out_file, "addi $sp, $sp, 4\n");
                free_regs();
                break;
            default:
                break;
        }
        ptr = ptr->next;
    }
    free_regs();
    fclose(out_file);
    return;
}

void translate_FUNCTION_ENTRY(InterCodes ptr){
    InterCode tmp_code = ptr->code;
    fprintf(out_file, "\n");
    fprintf(out_file,"%s:\n", tmp_code->u.sinop.op->u.value);
    ptr = ptr->next;
    fprintf(out_file,"addi $sp, $sp, -8\n");
    fprintf(out_file,"sw $fp, 0($sp)\n");
    fprintf(out_file,"sw $ra, 4($sp)\n");
    fprintf(out_file,"move $fp, $sp\n");
    int offset = 0;
    while (ptr != NULL && ptr->code != NULL && ptr->code->kind == PARAM){
        InterCode tmp_code = ptr->code;
        assign_stack(tmp_code->u.sinop.op, offset * 4 + 8);
        offset = offset + 4;
        ptr = ptr->next;
    }
    offset = 0;
    #ifdef DEBUG_lab3
    printf("step 2\n");
    #endif
    while (ptr != NULL && ptr->code != NULL && ptr->code->kind != FUNCTION_ENTRY){
        InterCode tmp_code = ptr->code;
        switch (tmp_code->kind){
            case ASSIGN:
            case GET_ADDR:
            case GET_VALUE:
            case ASSIGN_ADDR:
                if (tmp_code->u.assign.left == NULL)
                    break;
                offset = offset - 2 * 4;
                assign_stack(tmp_code->u.assign.left, offset);
                assign_stack(tmp_code->u.assign.right, offset + 4);
                break;
            case ADD:
            case SUB:
            case MUL:
            case DIV:
                #ifdef DEBUG_lab3
                printf("+-*/\n");
                #endif
                offset = offset - 3 * 4;
                assign_stack(tmp_code->u.binop.result, offset);
                assign_stack(tmp_code->u.binop.op1, offset + 4);
                assign_stack(tmp_code->u.binop.op2, offset + 2 * 4);
                break;
            case ARG:
            case READ:
            case WRITE:
                #ifdef DEBUG_lab3
                printf("ARG/READ/WRITE\n");
                #endif
                offset = offset - 4;
                assign_stack(tmp_code->u.sinop.op, offset);
                break;
            case IFGOTO: 
                offset = offset - 2 * 4;
                assign_stack(tmp_code->u.ifgoto_op.op1, offset);
                assign_stack(tmp_code->u.ifgoto_op.op2, offset + 4);
                break;
            case DEC:
                #ifdef DEBUG_lab3
                printf("DEC\n");
                #endif
                offset = offset - tmp_code->u.dec_op.size;
                assign_stack(tmp_code->u.dec_op.op, offset);
                break;
            case CALL:
                #ifdef DEBUG_lab3
                printf("CALL\n");
                #endif
                offset = offset - 4;
                assign_stack(tmp_code->u.assign.left, offset);
                break;
            default:break;
        }
        ptr = ptr->next;
    }
    fprintf(out_file, "addi $sp, $sp, %d\n", offset);
    #ifdef DEBUG_lab3
    printf("step 3\n");
    #endif
    return;
}

void translate_CALL(InterCodes ptr){
    int offset = 0;
    while (ptr != NULL && ptr->code != NULL && ptr->code->kind == ARG){
        offset = offset + 4;
        fprintf(out_file, "addi $sp, $sp, -4\n");
        int reg_index = assign_op2reg(ptr->code->u.sinop.op, MIPS_VAR);
        fprintf(out_file, "sw %s, 0($sp)\n", regs[reg_index].name);
        free_regs();
        ptr = ptr->next;
    }
    InterCode tmp_code = ptr->code;
    fprintf(out_file,"jal %s\n", tmp_code->u.assign.right->u.value);
    fprintf(out_file, "addi $sp, $sp, %d\n", offset);
    int index = get_free_reg(tmp_code->u.assign.left);
    fprintf(out_file,"move %s, $v0\n", regs[index].name);
    assign_reg2memeory(index);
    return;
}

void translate_RETURN(InterCodes ptr){
    fprintf(out_file,"lw $ra, 4($fp)\n");
    fprintf(out_file,"addi $sp, $fp, 8\n");
    int index = assign_op2reg(ptr->code->u.sinop.op, MIPS_VAR);
    fprintf(out_file,"lw $fp, 0($fp)\n");
    fprintf(out_file,"move $v0, %s\n",regs[index].name);
    fprintf(out_file,"jr $ra\n");
    free_regs();
    return;
}

//============= OPTIONAL FUNCTIONS ======================
// TODO:
//      assign_stack        [ok]
int get_free_reg(){
    for (int i = 8; i < 16; i++){
        if (regs[i].used == 0){
            regs[i].used = 1;
            return i;
        }
    }
    printf("No reg is free to use!\n");
    assert(0);
}

int assign_op2reg(Operand op, OperandType type){
    int i = get_free_reg();
    if (op->kind == CONSTANT){
        regs[i].variable = NULL;
        fprintf(out_file, "li %s, %u\n",regs[i].name,op->u.int_val);
    } else {
        Var tmp = find_var(op);
        regs[i].variable = tmp;
        tmp->reg_index = i;
        switch (type){
            case MIPS_VAR:
                fprintf(out_file,"lw %s, %d($fp)\n",regs[i].name,tmp->offset);
                break;
            case MIPS_ADDRESS:
                /* GET ADDRESS */
                fprintf(out_file,"addi %s, $fp, %d\n", regs[i].name, tmp->offset);
                break;
            case MIPS_VALUE:
                /* GET VALUE */
                fprintf(out_file,"lw %s, %d($fp)\n",regs[i].name, tmp->offset);
                fprintf(out_file,"lw %s, 0(%s)\n",regs[i].name, regs[i].name);
                break;
            default:break;
        }       
    }
    return i;
}

int assign_reg2memeory(int index){
    int offset = regs[index].variable->offset;
    fprintf(out_file,"sw %s, %d($fp)\n",regs[index].name, offset);
}

void free_regs(){
    for (int i = 8; i < 16; i++){
        regs[i].used = 0;
    }
}

void add_op2var(Var v){
    VariableList tmp = (VariableList)malloc(sizeof(VariableList_));
    tmp->variable = v;
    tmp->next = varListHead->next;
    varListHead->next = tmp;
    return;
}

Var find_var(Operand op){
    if (op->kind == CONSTANT)
        return NULL;
    VariableList ptr = varListHead->next;
    while (ptr != NULL){
        if (strcmp(ptr->variable->name, op->u.value) == 0)
            return ptr->variable;
        ptr = ptr->next;
    }
    return NULL;
}

void assign_stack(Operand op, int offset){
    if (op->kind == CONSTANT)
        return;
    if (offset < 0 && find_var(op) != NULL)
        return;
    Var tmp = (Var)malloc(sizeof(Var_));
    strcpy(tmp->name, op->u.value);
    tmp->offset = offset;
    add_op2var(tmp);
    return;
}
