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
addi $sp, $sp, -8
lw $t0, -8($fp)
lw $t1, 8($fp)
move $t0, $t1
sw $t0, -8($fp)
lw $ra, 4($fp)
addi $sp, $fp, 8
lw $t0, -8($fp)
lw $fp, 0($fp)
move $v0, $t0
jr $ra

main:
addi $sp, $sp, -8
sw $fp, 0($sp)
sw $ra, 4($sp)
move $fp, $sp
addi $sp, $sp, -56
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
addi $sp, $sp, -4
lw $t0, -20($fp)
sw $t0, 0($sp)
jal fact
addi $sp, $sp, 4
lw $t0, -28($fp)
move $t0, $v0
sw $t0, -28($fp)
lw $t1, -36($fp)
lw $t2, -28($fp)
move $t1, $t2
sw $t1, -36($fp)
lw $t0, -44($fp)
lw $t1, -36($fp)
move $t0, $t1
sw $t0, -44($fp)
lw $t0, -44($fp)
move $a0, $t0
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t0, -56($fp)
li $t1, 0
move $t0, $t1
sw $t0, -56($fp)
lw $ra, 4($fp)
addi $sp, $fp, 8
lw $t0, -56($fp)
lw $fp, 0($fp)
move $v0, $t0
jr $ra
