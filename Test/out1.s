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


main:
addi $sp, $sp, -8
sw $fp, 0($sp)
sw $ra, 4($sp)
move $fp, $sp
addi $sp, $sp, -208
lw $t0, -8($fp)
li $t1, 0
move $t0, $t1
sw $t0, -8($fp)
lw $t0, -16($fp)
lw $t1, -8($fp)
move $t0, $t1
sw $t0, -16($fp)
lw $t0, -24($fp)
li $t1, 1
move $t0, $t1
sw $t0, -24($fp)
lw $t0, -32($fp)
lw $t1, -24($fp)
move $t0, $t1
sw $t0, -32($fp)
lw $t0, -40($fp)
li $t1, 0
move $t0, $t1
sw $t0, -40($fp)
lw $t0, -48($fp)
lw $t1, -40($fp)
move $t0, $t1
sw $t0, -48($fp)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal read
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t0, -52($fp)
move $t0, $v0
sw $t0, -52($fp)
lw $t0, -60($fp)
lw $t1, -52($fp)
move $t0, $t1
sw $t0, -60($fp)
L1:
lw $t0, -68($fp)
lw $t1, -48($fp)
move $t0, $t1
sw $t0, -68($fp)
lw $t0, -76($fp)
lw $t1, -60($fp)
move $t0, $t1
sw $t0, -76($fp)
lw $t0, -68($fp)
lw $t1, -76($fp)
blt $t0, $t1, L2
j L3
L2:
lw $t0, -92($fp)
lw $t1, -16($fp)
move $t0, $t1
sw $t0, -92($fp)
lw $t0, -100($fp)
lw $t1, -32($fp)
move $t0, $t1
sw $t0, -100($fp)
lw $t0, -112($fp)
lw $t1, -92($fp)
lw $t2, -100($fp)
add $t0, $t1, $t2
sw $t0, -112($fp)
lw $t0, -120($fp)
lw $t1, -112($fp)
move $t0, $t1
sw $t0, -120($fp)
lw $t0, -128($fp)
lw $t1, -32($fp)
move $t0, $t1
sw $t0, -128($fp)
lw $t0, -128($fp)
move $a0, $t0
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t0, -140($fp)
lw $t1, -32($fp)
move $t0, $t1
sw $t0, -140($fp)
lw $t0, -16($fp)
lw $t1, -140($fp)
move $t0, $t1
sw $t0, -16($fp)
lw $t0, -156($fp)
lw $t1, -120($fp)
move $t0, $t1
sw $t0, -156($fp)
lw $t0, -32($fp)
lw $t1, -156($fp)
move $t0, $t1
sw $t0, -32($fp)
lw $t0, -172($fp)
lw $t1, -48($fp)
move $t0, $t1
sw $t0, -172($fp)
lw $t0, -180($fp)
li $t1, 1
move $t0, $t1
sw $t0, -180($fp)
lw $t0, -192($fp)
lw $t1, -172($fp)
lw $t2, -180($fp)
add $t0, $t1, $t2
sw $t0, -192($fp)
lw $t0, -48($fp)
lw $t1, -192($fp)
move $t0, $t1
sw $t0, -48($fp)
j L1
L3:
lw $t0, -208($fp)
li $t1, 0
move $t0, $t1
sw $t0, -208($fp)
lw $ra, 4($fp)
addi $sp, $fp, 8
lw $t0, -208($fp)
lw $fp, 0($fp)
move $v0, $t0
jr $ra
