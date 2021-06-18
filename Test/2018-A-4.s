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
addi $sp, $sp, -148
lw $t0, -28($fp)
li $t1, 0
move $t0, $t1
sw $t0, -28($fp)
lw $t0, -36($fp)
lw $t1, -28($fp)
move $t0, $t1
sw $t0, -36($fp)
lw $t0, -44($fp)
addi $t1, $fp, -20
move $t0, $t1
sw $t0, -44($fp)
lw $t0, -52($fp)
lw $t1, -36($fp)
move $t0, $t1
sw $t0, -52($fp)
lw $t0, -64($fp)
lw $t1, -52($fp)
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -64($fp)
lw $t0, -76($fp)
lw $t1, -44($fp)
lw $t2, -64($fp)
add $t0, $t1, $t2
sw $t0, -76($fp)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal read
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t0, -80($fp)
move $t0, $v0
sw $t0, -80($fp)
lw $t0, -76($fp)
lw $t1, -80($fp)
sw $t1, 0($t0)
lw $t0, -96($fp)
addi $t1, $fp, -20
move $t0, $t1
sw $t0, -96($fp)
lw $t0, -104($fp)
lw $t1, -36($fp)
move $t0, $t1
sw $t0, -104($fp)
lw $t0, -116($fp)
lw $t1, -104($fp)
li $t2, 4
mul $t0, $t1, $t2
sw $t0, -116($fp)
lw $t0, -128($fp)
lw $t1, -96($fp)
lw $t2, -116($fp)
add $t0, $t1, $t2
sw $t0, -128($fp)
lw $t0, -136($fp)
lw $t1, -128($fp)
lw $t1, 0($t1)
move $t0, $t1
sw $t0, -136($fp)
lw $t0, -136($fp)
move $a0, $t0
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t0, -148($fp)
li $t1, 0
move $t0, $t1
sw $t0, -148($fp)
lw $ra, 4($fp)
addi $sp, $fp, 8
lw $t0, -148($fp)
lw $fp, 0($fp)
move $v0, $t0
jr $ra
