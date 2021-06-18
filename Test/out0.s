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


test:
addi $sp, $sp, -8
sw $fp, 0($sp)
sw $ra, 4($sp)
move $fp, $sp
addi $sp, $sp, -32
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
lw $t1, 12($fp)
move $t0, $t1
sw $t0, -20($fp)
lw $t0, -20($fp)
move $a0, $t0
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t0, -32($fp)
li $t1, 0
move $t0, $t1
sw $t0, -32($fp)
lw $ra, 4($fp)
addi $sp, $fp, 8
lw $t0, -32($fp)
lw $fp, 0($fp)
move $v0, $t0
jr $ra

main:
addi $sp, $sp, -8
sw $fp, 0($sp)
sw $ra, 4($sp)
move $fp, $sp
addi $sp, $sp, -36
lw $t0, -8($fp)
li $t1, 1
move $t0, $t1
sw $t0, -8($fp)
lw $t0, -16($fp)
li $t1, 2
move $t0, $t1
sw $t0, -16($fp)
addi $sp, $sp, -4
lw $t0, -16($fp)
sw $t0, 0($sp)
addi $sp, $sp, -4
lw $t0, -8($fp)
sw $t0, 0($sp)
jal test
addi $sp, $sp, 8
lw $t0, -36($fp)
li $t1, 0
move $t0, $t1
sw $t0, -36($fp)
lw $ra, 4($fp)
addi $sp, $fp, 8
lw $t0, -36($fp)
lw $fp, 0($fp)
move $v0, $t0
jr $ra
