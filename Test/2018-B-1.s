.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
li $v0, 4
la $a0, _prompt
syscall
li $v0, 5
syscall
jr $ra

write:
li $v0, 1
syscall
li $v0, 4
la $a0, _ret
syscall
move $v0, $0
jr $ra


hanoi:
addi $sp, $sp, -8
sw $fp, 0($sp)
sw $ra, 4($sp)
move $fp, $sp
addi $sp, $sp, -280
lw $t0, -8($fp)
lw $t1, 8($fp)
move $t0, $t1
sw $t0, -8($fp)
lw $t0, -16($fp)
li $t1, 1
move $t0, $t1
sw $t0, -16($fp)
lw $t0, -8($fp)
lw $t1, -16($fp)
beq $t0, $t1, L1
j L2
L1:
lw $t0, -32($fp)
lw $t1, 12($fp)
move $t0, $t1
sw $t0, -32($fp)
lw $t0, -40($fp)
li $t1, 1000
move $t0, $t1
sw $t0, -40($fp)
lw $t0, -52($fp)
lw $t1, -32($fp)
lw $t2, -40($fp)
mul $t0, $t1, $t2
sw $t0, -52($fp)
lw $t0, -60($fp)
lw $t1, 20($fp)
move $t0, $t1
sw $t0, -60($fp)
lw $t0, -72($fp)
lw $t1, -52($fp)
lw $t2, -60($fp)
add $t0, $t1, $t2
sw $t0, -72($fp)
lw $t0, -72($fp)
move $a0, $t0
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
j L3
L2:
lw $t0, -84($fp)
lw $t1, 8($fp)
move $t0, $t1
sw $t0, -84($fp)
lw $t0, -92($fp)
li $t1, 1
move $t0, $t1
sw $t0, -92($fp)
lw $t0, -104($fp)
lw $t1, -84($fp)
lw $t2, -92($fp)
sub $t0, $t1, $t2
sw $t0, -104($fp)
lw $t0, -112($fp)
lw $t1, 12($fp)
move $t0, $t1
sw $t0, -112($fp)
lw $t0, -120($fp)
lw $t1, 20($fp)
move $t0, $t1
sw $t0, -120($fp)
lw $t0, -128($fp)
lw $t1, 16($fp)
move $t0, $t1
sw $t0, -128($fp)
addi $sp, $sp, -4
lw $t0, -128($fp)
sw $t0, 0($sp)
addi $sp, $sp, -4
lw $t0, -120($fp)
sw $t0, 0($sp)
addi $sp, $sp, -4
lw $t0, -112($fp)
sw $t0, 0($sp)
addi $sp, $sp, -4
lw $t0, -104($fp)
sw $t0, 0($sp)
jal hanoi
addi $sp, $sp, 16
lw $t0, -156($fp)
lw $t1, 12($fp)
move $t0, $t1
sw $t0, -156($fp)
lw $t0, -164($fp)
li $t1, 1000
move $t0, $t1
sw $t0, -164($fp)
lw $t0, -176($fp)
lw $t1, -156($fp)
lw $t2, -164($fp)
mul $t0, $t1, $t2
sw $t0, -176($fp)
lw $t0, -184($fp)
lw $t1, 20($fp)
move $t0, $t1
sw $t0, -184($fp)
lw $t0, -196($fp)
lw $t1, -176($fp)
lw $t2, -184($fp)
add $t0, $t1, $t2
sw $t0, -196($fp)
lw $t0, -196($fp)
move $a0, $t0
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t0, -208($fp)
lw $t1, 8($fp)
move $t0, $t1
sw $t0, -208($fp)
lw $t0, -216($fp)
li $t1, 1
move $t0, $t1
sw $t0, -216($fp)
lw $t0, -228($fp)
lw $t1, -208($fp)
lw $t2, -216($fp)
sub $t0, $t1, $t2
sw $t0, -228($fp)
lw $t0, -236($fp)
lw $t1, 16($fp)
move $t0, $t1
sw $t0, -236($fp)
lw $t0, -244($fp)
lw $t1, 12($fp)
move $t0, $t1
sw $t0, -244($fp)
lw $t0, -252($fp)
lw $t1, 20($fp)
move $t0, $t1
sw $t0, -252($fp)
addi $sp, $sp, -4
lw $t0, -252($fp)
sw $t0, 0($sp)
addi $sp, $sp, -4
lw $t0, -244($fp)
sw $t0, 0($sp)
addi $sp, $sp, -4
lw $t0, -236($fp)
sw $t0, 0($sp)
addi $sp, $sp, -4
lw $t0, -228($fp)
sw $t0, 0($sp)
jal hanoi
addi $sp, $sp, 16
L3:
lw $t0, -280($fp)
li $t1, 0
move $t0, $t1
sw $t0, -280($fp)
lw $ra, 4($fp)
addi $sp, $fp, 8
lw $t0, -280($fp)
lw $fp, 0($fp)
move $v0, $t0
jr $ra

main:
addi $sp, $sp, -8
sw $fp, 0($sp)
sw $ra, 4($sp)
move $fp, $sp
addi $sp, $sp, -76
lw $t0, -8($fp)
li $t1, 3
move $t0, $t1
sw $t0, -8($fp)
lw $t0, -16($fp)
lw $t1, -8($fp)
move $t0, $t1
sw $t0, -16($fp)
lw $t0, -24($fp)
lw $t1, -16($fp)
move $t0, $t1
sw $t0, -24($fp)
lw $t0, -32($fp)
li $t1, 1
move $t0, $t1
sw $t0, -32($fp)
lw $t0, -40($fp)
li $t1, 2
move $t0, $t1
sw $t0, -40($fp)
lw $t0, -48($fp)
li $t1, 3
move $t0, $t1
sw $t0, -48($fp)
addi $sp, $sp, -4
lw $t0, -48($fp)
sw $t0, 0($sp)
addi $sp, $sp, -4
lw $t0, -40($fp)
sw $t0, 0($sp)
addi $sp, $sp, -4
lw $t0, -32($fp)
sw $t0, 0($sp)
addi $sp, $sp, -4
lw $t0, -24($fp)
sw $t0, 0($sp)
jal hanoi
addi $sp, $sp, 16
lw $t0, -76($fp)
li $t1, 0
move $t0, $t1
sw $t0, -76($fp)
lw $ra, 4($fp)
addi $sp, $fp, 8
lw $t0, -76($fp)
lw $fp, 0($fp)
move $v0, $t0
jr $ra
