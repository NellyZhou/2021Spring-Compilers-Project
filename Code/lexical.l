%{
	#include "SyntaxTree.h"
	#include "syntax.tab.h"
	
	extern int lexical_error;

	/* YY_USER_ACTION */
	int yycolumn = 1;
	#define YY_USER_ACTION \
		yylloc.first_line = yylloc.last_line = yylineno; \
		yylloc.first_column = yycolumn; \
		yylloc.last_column = yycolumn + yyleng - 1; \
		yycolumn += yyleng;
%}
%option yylineno
letter_		[a-zA-Z_]
digital		[0-9]
new_line	[\n\r]
delim		[ \t]
ws		{delim}+

int_oct		0[0-7]+
int_hex		0[xX][0-9A-Fa-f]+

error_oct	0[0-9a-fA-F]+
error_hex	0[xX][0-9A-Za-z]+


id_form		{letter_}({letter_}|{digital})*	

%%
{ws} {
	/* no action and no return */
}

{new_line} {
	yycolumn = 1;
}

{int_hex}	{
//	printf("HEX");
	yylval.type_node = create_TreeNode("INT", __TOKEN_TYPE__, yylineno, HEX_VALUE, yytext);
	return INT;
}

{int_oct}	{
//	printf("OCT");
	yylval.type_node = create_TreeNode("INT", __TOKEN_TYPE__, yylineno, OCT_VALUE, yytext);
	return INT;
}

{error_hex}	{
	lexical_error = 1;
	printf("Error type A at Line %d: Illegal hexadecimal number \'%s\'\n",
		yylineno, yytext);
	return INT;
}

{error_oct}	{	
	lexical_error = 1;
	printf("Error type A at Line %d: Illegal octal number \'%s\'\n",
		yylineno, yytext);
	return INT;
}

0|([1-9]{digital}*)	{
//	printf("INT");
	yylval.type_node = create_TreeNode("INT", __TOKEN_TYPE__, yylineno, INT_VALUE, yytext);
	return INT;
}

0|([1-9]{digital}*)\.{digital}+	{
//	printf("FLOAT");
	yylval.type_node = create_TreeNode("FLOAT", __TOKEN_TYPE__, yylineno, FLOAT_VALUE, yytext);
	return FLOAT;
}

";"	{
//	printf("SEMI");
	yylval.type_node = create_TreeNode("SEMI", __TOKEN_TYPE__, yylineno, -1, yytext);
	return SEMI;
}

"," {
//	printf("COMMA");
	yylval.type_node = create_TreeNode("COMMA", __TOKEN_TYPE__, yylineno, -1, yytext);
	return COMMA;
}

"="	{
//	printf("ASSIGNOP");
	yylval.type_node = create_TreeNode("ASSIGNOP", __TOKEN_TYPE__, yylineno, -1, yytext);
	return ASSIGNOP;
}

>|<|>=|<=|==|!= {
//	printf("RELOP");
	yylval.type_node = create_TreeNode("RELOP", __TOKEN_TYPE__, yylineno, -1, yytext);
	return RELOP;
}

"+"	{
//	printf("PLUS");
	yylval.type_node = create_TreeNode("PLUS", __TOKEN_TYPE__, yylineno, -1, yytext);
	return PLUS;
}

"-"	{
//	printf("MINUS");

	yylval.type_node = create_TreeNode("MINUS", __TOKEN_TYPE__, yylineno, -1, yytext);
	return MINUS;
}

"*"	{
//	printf("STAR");
	yylval.type_node = create_TreeNode("STAR", __TOKEN_TYPE__, yylineno, -1, yytext);
	return STAR;
}

"/"	{
//	printf("DIV");
	yylval.type_node = create_TreeNode("DIV", __TOKEN_TYPE__, yylineno, -1, yytext);
	return DIV;
}

"&&" {
//	printf("AND");
	yylval.type_node = create_TreeNode("AND", __TOKEN_TYPE__, yylineno, -1, yytext);
	return AND;
}

"||"	{
//	printf("OR");
	yylval.type_node = create_TreeNode("OR", __TOKEN_TYPE__, yylineno, -1, yytext);
	return OR;
}

"." {
//	printf("DOT");
	yylval.type_node = create_TreeNode("DOT", __TOKEN_TYPE__, yylineno, -1, yytext);
	return DOT;
}

"!"	{
//	printf("NOT");
	yylval.type_node = create_TreeNode("NOT", __TOKEN_TYPE__, yylineno, -1, yytext);
	return NOT;
}

(int)|(float)	{
//	printf("TYPE");
	yylval.type_node = create_TreeNode("TYPE", __TOKEN_TYPE__, yylineno, TYPE_VALUE, yytext);
	return TYPE;
}

"("	{
//	printf("LP");
	yylval.type_node = create_TreeNode("LP", __TOKEN_TYPE__, yylineno, -1, yytext);
	return LP;
}

")"	{
//	printf("RP");
	yylval.type_node = create_TreeNode("RP", __TOKEN_TYPE__, yylineno, -1, yytext);
	return RP;
}

"["	{
//	printf("LB");
	yylval.type_node = create_TreeNode("LB", __TOKEN_TYPE__, yylineno, -1, yytext);
	return LB;
}

"]"	{
//	printf("RB");
	yylval.type_node = create_TreeNode("RB", __TOKEN_TYPE__, yylineno, -1, yytext);
	return RB;
}

"{"	{
//	printf("LC");
	yylval.type_node = create_TreeNode("LC", __TOKEN_TYPE__, yylineno, -1, yytext);
	return LC;
}

"}"	{
//	printf("RC");
	yylval.type_node = create_TreeNode("RC", __TOKEN_TYPE__, yylineno, -1, yytext);
	return RC;
}

struct	{
//	printf("STRUCT");
	yylval.type_node = create_TreeNode("STRUCT", __TOKEN_TYPE__, yylineno, -1, yytext);
	return STRUCT;
}

return	{
//	printf("RETURN");
	yylval.type_node = create_TreeNode("RETURN", __TOKEN_TYPE__, yylineno, -1, yytext);
	return RETURN;
}

if 	{
//	printf("IF");
	yylval.type_node = create_TreeNode("IF", __TOKEN_TYPE__, yylineno, -1, yytext);
	return IF;
}

else	{
//	printf("ELSE");
	yylval.type_node = create_TreeNode("ELSE", __TOKEN_TYPE__, yylineno, -1, yytext);
	return ELSE;
}

while	{
//	printf("WHILE");
	yylval.type_node = create_TreeNode("WHILE", __TOKEN_TYPE__, yylineno, -1, yytext);
	return WHILE;
}

{id_form}	{
//	printf("ID");
	yylval.type_node = create_TreeNode("ID", __TOKEN_TYPE__, yylineno, ID_VALUE, yytext);
	return ID;
}

.	{
	lexical_error = 1;
	printf("Error type A at Line %d: Mysterious characters \'%s\'\n",
		yylineno, yytext);
	return ID;
}

%%
