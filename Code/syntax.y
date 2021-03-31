%{
	#include <stdio.h>
	#include "SyntaxTree.h"
	#include "lex.yy.c"
	extern TreeNode* root;
	extern void yyerror(char*);
	extern int syntax_error;
	extern int yylineno;
	int pre_lineno = -1;
%}
/* declare types */
%union {
	int type_int;
	float type_float;
	double type_double;
	TreeNode * type_node;
}

/* declare tokens */
%token <type_node> INT FLOAT ID
%token <type_node> SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV
%token <type_node> AND OR DOT NOT
%token <type_node> TYPE 
%token <type_node> LP RP LB RB LC RC
%token <type_node> STRUCT RETURN IF ELSE WHILE 

/* declare types */
%type <type_node> Program ExtDefList ExtDef ExtDecList
%type <type_node> Specifier StructSpecifier OptTag Tag 
%type <type_node> VarDec FunDec VarList ParamDec
%type <type_node> CompSt StmtList Stmt 
%type <type_node> DefList Def DecList Dec
%type <type_node> Exp Args 

/* set preference */
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right TAKE_MINUS_VALUE
%left LP RP LB RB DOT

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE


%nonassoc losing_semi
%nonassoc SEMI

%nonassoc losing_rc
%nonassoc having_rc

%nonassoc losing_rb
%nonassoc having_rb

%nonassoc losing_rp
%nonassoc having_rp

%%
/* High-level Definition */
Program : ExtDefList {
		$$ = create_TreeNode("Program", __SYMBOL_TYPE__, @$.first_line, -1, "");
		root = $$;
		connect($$, $1);	
	}
	;

ExtDefList : ExtDef ExtDefList{
		$$ = create_TreeNode("ExtDefList", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
	}
	| /* empty*/{
		$$ = NULL;
	}
	;

ExtDef : Specifier ExtDecList SEMI{
		$$ = create_TreeNode("ExtDef", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| Specifier SEMI {
		$$ = create_TreeNode("ExtDef", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
	}
	| Specifier FunDec CompSt {
		$$ = create_TreeNode("ExtDef", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	;

ExtDecList : VarDec{
		$$ = create_TreeNode("ExtDefList", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
	}
	| VarDec COMMA ExtDecList {
		$$ = create_TreeNode("ExtDefList", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	;

/* Specifiers */
Specifier : TYPE {
		$$ = create_TreeNode("Specifier", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);

	}
	| StructSpecifier {
		$$ = create_TreeNode("Specifier", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);

	}
	;

StructSpecifier : STRUCT OptTag LC DefList RC %prec having_rc{
		$$ = create_TreeNode("StructSpecifier", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
		connect($$, $4);
		connect($$, $5);

	}
	| STRUCT Tag {
		$$ = create_TreeNode("StructSpecifier", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		
	}
	;

OptTag : ID {
		$$ = create_TreeNode("OptTag", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);

	}
	| /* empty */ {
		$$ = NULL;
	}
	;

Tag : ID {
		$$ = create_TreeNode("Tag", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);

	}
	;



/* Declarators */
VarDec : ID {
		$$ = create_TreeNode("VarDec", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);

	}
	| VarDec LB INT RB %prec having_rb{
		$$ = create_TreeNode("VarDec", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
		connect($$, $4);
	}
	;

FunDec : ID LP VarList RP %prec having_rp{
		$$ = create_TreeNode("FunDec", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
		connect($$, $4);

	}
	| ID LP RP %prec having_rp{
		$$ = create_TreeNode("FunDec", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);

	}
	;
VarList : ParamDec COMMA VarList{
		$$ = create_TreeNode("VarList", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);

	}
	| ParamDec {
		$$ = create_TreeNode("VarList", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);

	}
	;
ParamDec : Specifier VarDec{
		$$ = create_TreeNode("ParamDec", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);

	}
	;

/* Statements */
CompSt : LC DefList StmtList RC %prec having_rc{
		$$ = create_TreeNode("CompSt", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
		connect($$, $4);

	}
	;

StmtList : Stmt StmtList{
		$$ = create_TreeNode("StmtList", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);

	}
	| /* empty */{
		$$ = NULL;
	}
	;

Stmt : Exp SEMI{
		$$ = create_TreeNode("Stmt", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);

	}
	| CompSt{
		$$ = create_TreeNode("Stmt", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);

	}
	| RETURN Exp SEMI{
		$$ = create_TreeNode("Stmt", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);

	}
	| IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {
		$$ = create_TreeNode("Stmt", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
		connect($$, $4);
		connect($$, $5);

	}
	| IF LP Exp RP Stmt ELSE Stmt{
		$$ = create_TreeNode("Stmt", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
		connect($$, $4);
		connect($$, $5);
		connect($$, $6);
		connect($$, $7);

	}
	| WHILE LP Exp RP Stmt{
		$$ = create_TreeNode("Stmt", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
		connect($$, $4);
		connect($$, $5);

	}
	;

/* Local Definitions */
DefList : Def DefList {
		$$ = create_TreeNode("DefList", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);

	}
	| /* empty */ {
		$$ = NULL;
	}
	;

Def : Specifier DecList SEMI{
		$$ = create_TreeNode("Def", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);

	}
	;

DecList : Dec{
		$$ = create_TreeNode("DecList", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);

	}
	| Dec COMMA DecList{
		$$ = create_TreeNode("DecList", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);

	}
	;

Dec : VarDec{
		$$ = create_TreeNode("Dec", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);

	}
	| VarDec ASSIGNOP Exp{
		$$ = create_TreeNode("Dec", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);

	}
	;

/* Expressions */
Exp : Exp ASSIGNOP Exp{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| Exp AND Exp{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| Exp OR Exp{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| Exp RELOP Exp{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| Exp PLUS Exp{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| Exp MINUS Exp{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| Exp STAR Exp{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| Exp DIV Exp{ 
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| LP Exp RP %prec having_rp{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| MINUS Exp %prec TAKE_MINUS_VALUE{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
	}
	| NOT Exp %prec TAKE_MINUS_VALUE{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
	}
	| ID LP Args RP %prec having_rp{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
		connect($$, $4);
	}
	| ID LP RP %prec having_rp{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| Exp LB Exp RB %prec having_rb{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
		connect($$, $4);
	}
	| Exp DOT ID{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| ID{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
	}
	| INT{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
	} 
	| FLOAT{
		$$ = create_TreeNode("Exp", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
	}
	;

Args : Exp COMMA Args{
		$$ = create_TreeNode("Args", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
		connect($$, $2);
		connect($$, $3);
	}
	| Exp{
		$$ = create_TreeNode("Args", __SYMBOL_TYPE__, @$.first_line, -1, "");
		connect($$, $1);
	}
	;

/* specific errors */
ExtDef : Specifier ExtDecList %prec losing_semi{
		yyerror("Missing \";\".");
	}
	;

ExtDecList : VarDec ExtDecList{	//lose comma
		yyerror("Missing \",\".");
	}
	;

StructSpecifier : STRUCT OptTag LC DefList %prec losing_rc{
		yyerror("Missing \"}\".");
	}
	;

VarDec : VarDec LB INT %prec losing_rb{
		yyerror("Missing \"]\".");
	}
	| VarDec LB error RB{
		yyerror("Illegal index.");
	}
	;

/* error recovery */
Stmt : error SEMI {
		$$ = NULL;
	}
	;

CompSt : error RC {
		$$ = NULL;
	}
	;

Exp : error RP {
		$$ = NULL;
	}
	;

%%

void yyerror(char* msg)	{
	if (pre_lineno == yylineno)
		return;
	pre_lineno = yylineno;
	syntax_error = 1;
	printf("Error type B at Line %d: %s\n", yylineno, msg);
}
