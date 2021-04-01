# 2021Spring-Compilers-Project

**Problem on test 2**

Exp -> Exp LB Exp RB. 为了避免冲突，引入新的状态 *error_Exp*.

Stmt -> Exp error vs. Stmt -> IF LP Exp RP Exp ELSE. 通过-d同时对照syntax.output，发现接受ELSE时率先发生error，而导致前者具体错误会被同一行忽略。故在判断IF中Stmt是否缺少‘;’时加入后者。 

**Problem on test h1.cmm**
负数的处理出现错误
