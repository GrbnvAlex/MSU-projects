#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20140101

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "calc.y"
	#include <stdio.h>
#line 21 "calc.c"

#ifndef YYSTYPE
typedef int YYSTYPE;
#endif

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define YYERRCODE 256
static const short yylhs[] = {                           -1,
    0,    1,    1,    1,    2,    2,    2,    3,    3,    4,
    4,    4,    4,    4,    4,    4,    4,    4,    4,    4,
};
static const short yylen[] = {                            2,
    1,    3,    3,    1,    3,    3,    1,    3,    1,    2,
    2,    2,    2,    2,    2,    2,    2,    2,    2,    0,
};
static const short yydefred[] = {                         0,
    0,    0,    0,    0,    7,    0,    0,    0,    0,    0,
    0,   10,   11,   12,   13,   14,   15,   16,   17,   18,
   19,    8,    0,    0,    5,    6,
};
static const short yydgoto[] = {                          2,
    3,    4,    5,    6,
};
static const short yysindex[] = {                       -40,
  -40,    0,  -14,  -12,    0,  -46,   -5,  -40,  -40,  -40,
  -40,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -12,  -12,    0,    0,
};
static const short yyrindex[] = {                        17,
  -29,    0,   15,   37,    0,   34,    0,    1,    1,    1,
    1,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   45,   61,    0,    0,
};
static const short yygindex[] = {                         0,
   38,   24,   73,    0,
};
#define YYTABLESIZE 106
static const short yytable[] = {                          1,
   20,   12,   14,   16,   18,   20,   13,   15,   17,   19,
   21,   20,   20,   20,    1,   20,   20,   20,   20,   20,
   20,   20,   20,   20,   20,   20,   20,   20,    8,   10,
    9,   23,   24,    9,   11,   22,    4,    8,    7,    9,
    0,   20,   20,   20,    2,   20,    0,   20,   20,   20,
   20,   20,   20,   20,   20,   20,   20,   20,   20,   20,
    3,   20,    0,   20,   20,   20,   20,   20,   20,   20,
   20,   20,   20,   20,    9,    9,    9,    4,    9,    4,
    9,    4,   25,   26,    0,    2,    0,    2,    0,    2,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    3,    0,    3,    0,    3,
};
static const short yycheck[] = {                         40,
    0,   48,   49,   50,   51,   52,   53,   54,   55,   56,
   57,   41,   42,   43,    0,   45,    0,   47,   48,   49,
   50,   51,   52,   53,   54,   55,   56,   57,   43,   42,
   45,    8,    9,    0,   47,   41,    0,   43,    1,   45,
   -1,   41,   42,   43,    0,   45,   -1,   47,   48,   49,
   50,   51,   52,   53,   54,   55,   56,   57,   42,   43,
    0,   45,   -1,   47,   48,   49,   50,   51,   52,   53,
   54,   55,   56,   57,   41,   42,   43,   41,   45,   43,
   47,   45,   10,   11,   -1,   41,   -1,   43,   -1,   45,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   41,   -1,   43,   -1,   45,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 57
#define YYTRANSLATE(a) ((a) > YYMAXTOKEN ? (YYMAXTOKEN + 1) : (a))
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'","'*'","'+'",0,"'-'",0,"'/'","'0'","'1'","'2'","'3'",
"'4'","'5'","'6'","'7'","'8'","'9'","illegal-symbol",
};
static const char *yyrule[] = {
"$accept : P",
"P : S1",
"S1 : S1 '+' S2",
"S1 : S1 '-' S2",
"S1 : S2",
"S2 : S2 '*' S3",
"S2 : S2 '/' S3",
"S2 : S3",
"S3 : '(' S1 ')'",
"S3 : N",
"N : N '0'",
"N : N '5'",
"N : N '1'",
"N : N '6'",
"N : N '2'",
"N : N '7'",
"N : N '3'",
"N : N '8'",
"N : N '4'",
"N : N '9'",
"N :",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 10000
#define YYMAXDEPTH  10000
#endif
#endif

#define YYINITSTACKSIZE 200

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 16 "calc.y"
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
#line 201 "calc.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = (int) (data->s_mark - data->s_base);
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = yyname[YYTRANSLATE(yychar)];
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 5 "calc.y"
	{ printf ( "calc: %d\n", yystack.l_mark[0] ); }
break;
case 2:
#line 6 "calc.y"
	{yyval = yystack.l_mark[-2] + yystack.l_mark[0];}
break;
case 3:
#line 6 "calc.y"
	{yyval = yystack.l_mark[-2] - yystack.l_mark[0];}
break;
case 4:
#line 6 "calc.y"
	{yyval = yystack.l_mark[0];}
break;
case 5:
#line 7 "calc.y"
	{yyval = yystack.l_mark[-2] * yystack.l_mark[0];}
break;
case 6:
#line 7 "calc.y"
	{if (yystack.l_mark[0] != 0) yyval = yystack.l_mark[-2] / yystack.l_mark[0]; else {printf("divide by zero\n"); return 1;}}
break;
case 7:
#line 7 "calc.y"
	{yyval = yystack.l_mark[0];}
break;
case 8:
#line 8 "calc.y"
	{yyval = yystack.l_mark[-1];}
break;
case 9:
#line 8 "calc.y"
	{yyval = yystack.l_mark[0]; }
break;
case 10:
#line 9 "calc.y"
	{yyval = yystack.l_mark[-1]*10 + 0;}
break;
case 11:
#line 9 "calc.y"
	{yyval = yystack.l_mark[-1]*10 + 5;}
break;
case 12:
#line 10 "calc.y"
	{yyval = yystack.l_mark[-1]*10 + 1;}
break;
case 13:
#line 10 "calc.y"
	{yyval = yystack.l_mark[-1]*10 + 6;}
break;
case 14:
#line 11 "calc.y"
	{yyval = yystack.l_mark[-1]*10 + 2;}
break;
case 15:
#line 11 "calc.y"
	{yyval = yystack.l_mark[-1]*10 + 7;}
break;
case 16:
#line 12 "calc.y"
	{yyval = yystack.l_mark[-1]*10 + 3;}
break;
case 17:
#line 12 "calc.y"
	{yyval = yystack.l_mark[-1]*10 + 8;}
break;
case 18:
#line 13 "calc.y"
	{yyval = yystack.l_mark[-1]*10 + 4;}
break;
case 19:
#line 13 "calc.y"
	{yyval = yystack.l_mark[-1]*10 + 9;}
break;
case 20:
#line 14 "calc.y"
	{yyval = 0;}
break;
#line 483 "calc.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = yyname[YYTRANSLATE(yychar)];
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
