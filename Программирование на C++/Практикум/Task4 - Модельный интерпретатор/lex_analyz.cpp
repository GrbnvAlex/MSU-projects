#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "stdio.h"
#include "unistd.h"

using namespace std;

/**/enum type_of_lex
{
  LEX_NULL,
  LEX_AND, LEX_BEGIN, LEX_BOOLEAN, LEX_DO, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, 
  LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE,

  LEX_GOTO, LEX_STRING,
 
  LEX_FIN,
  LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ, LEX_LSS, 
  LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_BEG, LEX_END,
  LEX_REM,

  LEX_NUM, 
  LEX_ID, 
  POLIZ_LABEL, 
  POLIZ_ADDRESS,
  POLIZ_GO,
  POLIZ_FGO};

/**/const char lex_out [][16] = {"LEX_NULL","LEX_AND", "LEX_BEGIN", "LEX_BOOLEAN", "LEX_DO", "LEX_ELSE", "LEX_IF", "LEX_FALSE", "LEX_INT", 
		  "LEX_NOT", "LEX_OR", "LEX_PROGRAM", "LEX_READ", "LEX_THEN", "LEX_TRUE", "LEX_VAR", "LEX_WHILE", "LEX_WRITE", 

		  "LEX_GOTO", "LEX_STRING",
	
		  "LEX_FIN",
		  "LEX_SEMICOLON", "LEX_COMMA", "LEX_COLON", "LEX_ASSIGN", "LEX_LPAREN", "LEX_RPAREN", "LEX_EQ", "LEX_LSS",
		  "LEX_GTR", "LEX_PLUS", "LEX_MINUS", "LEX_TIMES", "LEX_SLASH", "LEX_LEQ", "LEX_NEQ", "LEX_GEQ",
		  "LEX_BEG", "LEX_END", "LEX_REM",
		  "LEX_NUM",
		  "LEX_ID",
		  "POLIZ_LABEL",
		  "POLIZ_ADDRESS",
		  "POLIZ_GO",
		  "POLIZ_FGO"};

/* LEX */

class Lex
{
	type_of_lex t_lex;
  	int v_lex;
  	char * s_lex;
public:	
	Lex ( type_of_lex t = LEX_NULL, int v = 0, char * s = NULL): t_lex (t), v_lex (v), s_lex(s) {}
	type_of_lex  get_type () { return t_lex; }
	int get_value () { return v_lex; }
	friend ostream & operator<< (ostream &s, Lex l)
	{
		s << '(' << l.t_lex << ',' << l.v_lex << "); ==> " << lex_out[l.t_lex];
		if (l.s_lex != NULL) s << " ==> " << l.s_lex;
		return s;
	}
};

/* IDENT */

class Ident
{
         char       * name;
         bool         declare;
         type_of_lex  type;
         bool         assign;
         int          value;
public:
                      Ident() { declare = false; assign = false; }
         char       * get_name () { return name; }
         void         put_name (const char *n)
                      {
                      	name = new char [ strlen(n)+1];
                        strcpy(name,n);
                      }
         bool         get_declare () { return declare; }
         void         put_declare () { declare = true; }
         type_of_lex  get_type    () { return type; }
         void         put_type    ( type_of_lex t ) { type = t; }
         bool         get_assign  () { return assign; }
         void         put_assign  (){ assign = true; }
         int          get_value   () { return value; }
         void         put_value   (int v){ value = v; }
};

/* TABL_IDENT */

class Tabl_ident
{
         Ident      * p;
         int          size;
         int          top;
public:
	Tabl_ident ( int max_size )
	{
		p = new Ident [ size = max_size ];
		top = 1;
	}
	~Tabl_ident () { delete [] p; }
         Ident & operator[] ( int k ) { return p[k]; }
         int put ( const char *buf );
};
 
int Tabl_ident::put ( const char *buf )
{
	for ( int j = 1; j < top; j++ )
		if ( !strcmp (buf, p[j].get_name()) ) return j;
	p[top].put_name(buf);
	++top;
	return top-1;
}

Tabl_ident TID ( 100 );

/* SCANNER */

class Scanner
{
	enum state { H, IDENT, NUMB, COM, ALE, DELIM, NEQ, STR, BEG };
	static const char * TW [];
	static type_of_lex words [];
	static const char * TD [];
	static type_of_lex dlms [];
	state CS;
	FILE * fp;
	char c;
	char buf [80];
	int buf_top;
	void clear ()
	{
		buf_top = 0;
		for ( int j = 0; j < 80; j++ ) buf[j] = '\0';
	}
 	void add ()
	{
		buf [ buf_top++ ] = c;
	}
	int look ( const char *buf, const char **list )
	{
		int i = 0;
		while (list[i])
		{
		  if ( !strcmp(buf, list[i]) ) return i;
		  ++i;
		}
		return 0;
	}
	void gc ()
	{
		if ( (c = fgetc (fp)) == EOF) cout << "EOF\n";
	}
public:
	static int num_of_braces;
	Scanner ( const char * program )
	{
		fp = fopen ( program, "r" );
		CS = H;
		clear();
		gc();
	}
	Lex get_lex ();
};

/**/const char * Scanner::TW [] = {"", "and", "boolean", "do", "else", "if", "false", "int", "not", "or", "program",
                            "read", "then", "true", "var", "while", "write", "goto", "string", NULL};
 
/**/type_of_lex Scanner::words [] = {LEX_NULL, LEX_AND, LEX_BOOLEAN, LEX_DO, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT,
                     		 LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE, LEX_GOTO,
				 LEX_STRING, LEX_NULL};
 
const char * Scanner::TD [] = {"", "}", ";", ",", ":", "=", "(", ")", "==", "<", ">", "+", "-", "*", "/", "<=", "!=", ">=", "{", "}", "%", NULL};
 
type_of_lex Scanner::dlms  [] = {LEX_NULL, LEX_FIN, LEX_SEMICOLON, LEX_COMMA, LEX_COLON, LEX_ASSIGN, LEX_LPAREN, LEX_RPAREN, LEX_EQ,
                     		 LEX_LSS, LEX_GTR, LEX_PLUS, LEX_MINUS, LEX_TIMES, LEX_SLASH, LEX_LEQ, LEX_NEQ, LEX_GEQ, LEX_BEG, LEX_END,
				 LEX_REM, LEX_NULL};
 
Lex Scanner::get_lex () 
{
	int d, j;
	char c1;
	char * str = NULL;
	char * tmp = NULL;
	int i = 1;
 	
	CS = H;
	do
	{
		switch(CS)
    		{
      			case H:
				if ( c==' ' || c == '\n' || c== '\r' || c == '\t' ) 
		  		gc();
				else if ( isalpha(c) )
				{
					clear();
					add();
					gc();
					CS = IDENT;
				}
				else if ( isdigit(c) )
				{
					d = c - '0';
					gc();
					CS = NUMB;
				}

				else if (c == '"')
				{
					gc();
					CS = STR;
				}

				else if ( c== '/' )
				{
					gc();
					if (c == '*')
					{
						gc();
						CS = COM;
					}
					else
					{
						d = c;
						c = '/';
						clear();
						add();
						if ( j = look ( buf, TD) )
						{
						  	c = d;
						  	return Lex ( dlms[j], j );
						}
					}
				}

				else if ( c== '=' || c== '<' || c== '>' )
				{ 
					clear(); 
					add(); 
					gc(); 
					CS = ALE; 
				}
				else if (c == '}')
				{
					clear(); 
					add();
					j = look ( buf, TD );
					gc();
					Scanner :: num_of_braces--;
					if (Scanner :: num_of_braces <= 0)
						return Lex(LEX_FIN, j);
					else
						return Lex(LEX_END, 23);
				}
				else if (c == '{') 
				{
					clear(); 
					add();
					j = look ( buf, TD );
					gc();
					Scanner :: num_of_braces++;
					return Lex(LEX_BEGIN, j);
				}
				else if (c == '!')
				{
					clear();
					add();
					gc();
					CS = NEQ;
				}
				else 
					CS = DELIM;
				break;

      			case IDENT:
				if ( isalpha(c) || isdigit(c) ) 
				{
					add(); 
				  	gc();
				}
				else if ( j = look (buf, TW) )
				  	return Lex (words[j], j);
				else
				{
				  	j = TID.put(buf);
				  	return Lex (LEX_ID, j);
				}
				break;

		     	case NUMB:
				if ( isdigit(c) ) 
				{
				  	d = d * 10 + (c - '0'); gc();
				}
				else
				  	return Lex ( LEX_NUM, d);
				break;

			case COM:
				if (c == '*')
				{
					gc();
					if (c == '/')
					{
						gc();
						CS = H;
					}
					break;
				}
				if (feof(fp)) throw "The commentary doesn't end";
				gc();
				break;

		      	case ALE:
				if ( c== '=')
				{
				  	add();
				  	gc();
				  	j = look ( buf, TD );
				  	return Lex ( dlms[j], j);
				}
				else
				{
				  	j = look ( buf, TD );
				  	return Lex ( dlms[j], j );
				}
				break;
		      	case NEQ:
				if (c == '=')
				{
				  	add();
				  	gc();
				  	j = look ( buf, TD );
				  	return Lex ( LEX_NEQ, j );
				}
				else
				  	throw '!';
				break;
		      	case DELIM:
				clear();
				add();
				if ( j = look ( buf, TD) )
				{
				  	gc();
				  	return Lex ( dlms[j], j );
				}
				else
				  	throw c;
			      	break;
			case STR:
				if (feof(fp)) throw "EOF!!! The string doesn't end";
				if (c == '"')
				{
					gc();
					return Lex(LEX_STRING, 0, str);
				}
				else
				{
					tmp = new char [i+1];
					if (str != NULL) strcpy (tmp, str);
					delete str; str = NULL;
					str = new char [++i+1];
					if (tmp != NULL) strcpy (str, tmp);
					str [i-2] = c;
					str [i-1] = '\0';
					delete tmp; tmp = NULL;
					gc();
				}
				break;
	    } // switch CS
  	} while (true); // do
}

int Scanner :: num_of_braces = 0;

int main ()
{
	Lex a;
	Scanner scan ("program1");


	try
  	{
		do
		{
			a = scan.get_lex();
			cout << a << endl;
		}
		while (a.get_type() != LEX_FIN);
    		return 0;
  	}
  	catch (char c)
	{
    		cout << "unexpected symbol " << c << endl;
    		return 1;
 	}
  	catch (Lex l)
  	{
    		cout << "unexpected lexeme";
    		cout << l;
    		return 1;
  	}
  	catch (const char * source)
  	{
   		cout << source << endl;
    		return 1;
  	}

}
