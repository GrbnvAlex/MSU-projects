%{
	#include <stdio.h>
	int stretch=0;
%}
%%
P: S { printf ( "stretch: %d\n", stretch ); }
S: '('F')'S { stretch++; } | { $$=0; }
F: '('F')'F { $$ = 1; } | { $$=0; }
%%
main() {
	printf ( "type a string, please: " );
	yyparse ();
}
yylex () {
	int c;
	c = getchar();
	if ( c=='\n' ) return 0;
	yylval = c;
	return c;
};
yyerror ( char *s ) {
	printf ( "stretch eval: %s\n", s );
};
