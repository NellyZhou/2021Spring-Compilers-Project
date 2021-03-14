letter_		[a-zA-Z_]
digital		[0-9]

%%
0|([1-9]{digital}*)	{
	printf("INT");
}

0|([1-9]{digital}*)\.{digital}+	{
	printf("FLOAT");
}

letter_{letter_|digital}*	{
	printf("ID");
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

int | float	{
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
	printf("RC");
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

.	{
	printf("Error type A at Line %d: Mysterious characters \'%s\'\n",
		yylineno, yytext);
}

%%
