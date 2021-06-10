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


fact:
addi $sp, $sp, -8
sw $fp, 0($sp)
sw $ra, 4($sp)
move $fp, $sp
addi $sp, $sp, -100
lw $t0, -8($fp)
lw $t1, 8($fp)
move $t0, $t1
sw $t0, -8($fp)
lw $t0, -8($fp)
move $a0, $t0
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t0, -20($fp)
lw $t1, 8($fp)
move $t0, $t1
sw $t0, -20($fp)
lw $t0, -28($fp)
li $t1, 1
move $t0, $t1
sw $t0, -28($fp)
lw $t0, -20($fp)
lw $t1, -28($fp)
beq $t0, $t1, L1
j L2
L1:
lw $t0, -44($fp)
lw $t1, 8($fp)
move $t0, $t1
sw $t0, -44($fp)
lw $ra, 4($fp)
addi $sp, $fp, 8
lw $t0, -44($fp)
lw $fp, 0($fp)
move $v0, $t0
jr $ra
j L3
L2:
lw $t0, -52($fp)
lw $t1, 8($fp)
move $t0, $t1
sw $t0, -52($fp)
lw $t0, -60($fp)
lw $t1, 8($fp)
move $t0, $t1
sw $t0, -60($fp)
lw $t0, -68($fp)
li $t1, 1
move $t0, $t1
sw $t0, -68($fp)
lw $t0, -80($fp)
lw $t1, -60($fp)
lw $t2, -68($fp)
sub $t0, $t1, $t2
sw $t0, -80($fp)
addi $sp, $sp, -4
lw $t0, -80($fp)
sw $t0, 0($sp)
jal fact
addi $sp, $sp, 4
lw $t0, -88($fp)
move $t0, $v0
sw $t0, -88($fp)
lw $t1, -100($fp)
lw $t2, -52($fp)
lw $t3, -88($fp)
mul $t1, $t2, $t3
sw $t1, -100($fp)
lw $ra, 4($fp)
addi $sp, $fp, 8
lw $t0, -100($fp)
lw $fp, 0($fp)
move $v0, $t0
jr $ra
L3:

main:
addi $sp, $sp, -8
sw $fp, 0($sp)
sw $ra, 4($sp)
move $fp, $sp
addi $sp, $sp, -96
addi $sp, $sp, -4
sw $ra, 0($sp)
jal read
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t0, -4($fp)
move $t0, $v0
sw $t0, -4($fp)
lw $t0, -12($fp)
lw $t1, -4($fp)
move $t0, $t1
sw $t0, -12($fp)
lw $t0, -20($fp)
lw $t1, -12($fp)
move $t0, $t1
sw $t0, -20($fp)
lw $t0, -28($fp)
li $t1, 1
move $t0, $t1
sw $t0, -28($fp)
lw $t0, -20($fp)
lw $t1, -28($fp)
bgt $t0, $t1, L4
j L5
L4:
lw $t0, -44($fp)
lw $t1, -12($fp)
move $t0, $t1
sw $t0, -44($fp)
addi $sp, $sp, -4
lw $t0, -44($fp)
sw $t0, 0($sp)
jal fact
addi $sp, $sp, 4
lw $t0, -52($fp)
move $t0, $v0
sw $t0, -52($fp)
lw $t1, -60($fp)
lw $t2, -52($fp)
move $t1, $t2
sw $t1, -60($fp)
j L6
L5:
lw $t0, -68($fp)
li $t1, 1
move $t0, $t1
sw $t0, -68($fp)
lw $t0, -60($fp)
lw $t1, -68($fp)
move $t0, $t1
sw $t0, -60($fp)
L6:
lw $t0, -84($fp)
lw $t1, -60($fp)
move $t0, $t1
sw $t0, -84($fp)
lw $t0, -84($fp)
move $a0, $t0
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t0, -96($fp)
li $t1, 0
move $t0, $t1
sw $t0, -96($fp)
lw $ra, 4($fp)
addi $sp, $fp, 8
lw $t0, -96($fp)
lw $fp, 0($fp)
move $v0, $t0
jr $ra
