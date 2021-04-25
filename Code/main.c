#include <stdio.h>
#include "SyntaxTree.h"
#include "semantics.h"

//#define __YY_DEBUG__

TreeNode *root = NULL;
int lexical_error = 0;
int syntax_error = 0;
int pre_lineno = -1;

extern FILE* yyin;
#ifdef __YY_DEBUG__
extern int yydebug;
#endif
extern int yylex (void);

extern int yyparse(void);
extern void yyrestart(FILE *);

int main(int argc, char** argv){
	if (argc <= 1) return 1;
        FILE* f = fopen(argv[1], "r");
	if (!f){
            perror(argv[1]);
            return 1;
	}

	yyrestart(f);
#ifdef __YY_DEBUG__
	yydebug = 1;
#endif
	yyparse();

	if (!lexical_error && !syntax_error){
	//	show(root, 0);
		SemanticsProgramAnalysis(root);
	}

	return 0;
}
