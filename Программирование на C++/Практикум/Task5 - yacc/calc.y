%{
	#include <stdio.h>
%}
%%
P: S1 { printf ( "calc: %d\n", $1 ); }
S1: S1'+'S2 {$$ = $1 + $3;} | S1'-'S2 {$$ = $1 - $3;} | S2 {$$ = $1;}
S2: S2'*'S3 {$$ = $1 * $3;} | S2'/'S3 {if ($3 != 0) $$ = $1 / $3; else {printf("divide by zero\n"); return 1;}} | S3 {$$ = $1;}
S3: '('S1')' {$$ = $2;} | N {$$ = $1; }
N: N'0' {$$ = $1*10 + 0;} | N'5' {$$ = $1*10 + 5;} |
   N'1' {$$ = $1*10 + 1;} | N'6' {$$ = $1*10 + 6;} |
   N'2' {$$ = $1*10 + 2;} | N'7' {$$ = $1*10 + 7;} | 
   N'3' {$$ = $1*10 + 3;} | N'8' {$$ = $1*10 + 8;} |
   N'4' {$$ = $1*10 + 4;} | N'9' {$$ = $1*10 + 9;} |
   {$$ = 0;}
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
	printf ( "calc eval: %s\n", s );
};
