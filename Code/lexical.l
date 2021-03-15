
%{
%}
%option yylineno
letter_		[a-zA-Z_]
digital		[0-9]

%%
0[0-7]+	{
	printf("OCT");
}

0[xX][0-9A-F]+	{
	printf("HEX");
}

0|([1-9]{digital}*)	{
	printf("INT");
}

0|([1-9]{digital}*)\.{digital}+	{
	printf("FLOAT");
}

";"	{
	printf("SEMI");
}

"," {
	printf("COMMA");
}

=	{
	printf("ASSIGNOP");
}

>|<|>=|<=|==|!= {
	printf("RELOP");
}

"+"	{
	printf("PLUS");
}

"-"	{
	printf("MINUS");
}

\*	{
	printf("STAR");
}

"/"	{
	printf("DIV");
}

"&&" {
	printf("AND");
}

"||"	{
	printf("OR");
}

"." {
	printf("DOT");
}

"!"	{
	printf("NOT");
}

(int)|(float)	{
	printf("TYPE");
}

\(	{
	printf("LP");
}

\)	{
	printf("RP");
}

"["	{
	printf("LB");
}

"]"	{
	printf("RB");
}

"{"	{
	printf("LC");
}

"}"	{
	printf("RC");
}

struct	{
	printf("STRUCT");
}

return	{
	printf("RETURN");
}

if 	{
	printf("IF");
}

else	{
	printf("ELSE");
}

while	{
	printf("WHILE");
}

" "	{
	printf(" ");
}

\n 	{
	printf("\n");
}

\t 	{
	printf("\t");
}

{letter_}({letter_}|{digital})*	{
	printf("ID");
}

.	{
	printf("\nError type A at Line %d: Mysterious characters \'%s\'\n",
		yylineno, yytext);
}

%%
