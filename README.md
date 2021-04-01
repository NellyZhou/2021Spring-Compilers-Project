# 2021Spring-Compilers-Project

编译原理课程Lab1
任务号：16

# 实验调试记录
**Problem on test 2**

Exp -> Exp LB Exp RB. 为了避免冲突，引入新的状态 *error_Exp*.

Stmt -> Exp error vs. Stmt -> IF LP Exp RP Exp ELSE. 通过-d同时对照syntax.output，发现接受ELSE时率先发生error，而导致前者具体错误会被同一行忽略。故在判断IF中Stmt是否缺少‘;’时加入后者。 

**Problem on test h1.cmm**

负数的处理出现错误。

在语法分析树中，需要将整型值的输出设为 %u，类型应该为unsigned int

# 鸣谢
提供的测试数据
https://github.com/massimodong/compilers-tests
