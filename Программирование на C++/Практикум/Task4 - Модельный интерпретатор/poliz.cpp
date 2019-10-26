#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include "stdio.h"
#include "unistd.h"

using namespace std;

/////////////////////////////////////////////////////////////////
 
template < class T, int max_size >
class Stack
{
         T            s [max_size];
         int          top;
public:
                      Stack () { top = 0; }
         void         reset () { top = 0; }
         void         push ( T i );
         T            pop ();
         bool         is_empty () { return top == 0; }
         bool         is_full  () { return top == max_size; }
};
 
template < class T, int max_size >
void Stack < T, max_size > :: push (T i)
{
  if ( !is_full() )
    s [top++] = i;
  else
    throw "Stack_is_full";
}
 
template <class T, int max_size >
T Stack < T, max_size > :: pop ()
{
  if ( !is_empty() )
    return s[--top];
  else
    throw "stack_is_empty";
}

/////////////////////////////////////////////////////////////////

enum type_of_lex
{
  LEX_NULL,
  LEX_AND, LEX_BEGIN, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT, 
  LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE,

  LEX_BREAK, LEX_STRING,
 
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

const char lex_out [][16] = {"LEX_NULL","LEX_AND", "LEX_BEGIN", "LEX_BOOL", "LEX_DO", "LEX_ELSE", "LEX_IF", "LEX_FALSE", "LEX_INT", 
		  "LEX_NOT", "LEX_OR", "LEX_PROGRAM", "LEX_READ", "LEX_THEN", "LEX_TRUE", "LEX_VAR", "LEX_WHILE", "LEX_WRITE", 

		  "LEX_BREAK", "LEX_STRING",
	
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
		//s << '(' << l.t_lex << ',' << l.v_lex << "); ==> " << lex_out[l.t_lex];
		s << lex_out[l.t_lex];
		if (l.s_lex != NULL) s << " ==> " << l.s_lex;
		return s;
	}
};

/* IDENT */

class Ident
{
         char *	      name;
         bool         declare;
         type_of_lex  type;
         bool         assign;
         int          value;
	 char *	      str;
public:
                      Ident() { declare = false; assign = false; }
         char *	      get_name () { return name; }
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

	 char *       get_str     () 
	 {
		char * tmp = new char [sizeof(str) + 1];
		strcmp (tmp, str);
		return tmp;
	 }
	 void	      put_str     (const char * s) 
	 {
	 	str = new char [sizeof(s) + 1];
		strcmp (str, s); 
	 }
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

/////////////////////////  Класс Poliz  /////////////////////////////
 
class Poliz
{
         Lex        * p;
         int          size;
         int          free;
public:
                      Poliz (int max_size)
                      {
                        p = new Lex [size = max_size];
                        free = 0;
                      }
                     ~Poliz() { delete [] p; }
         void         put_lex ( Lex l )
                      {
                      	p [free] = l;
                        free++;
                      }
         void         put_lex ( Lex l, int place) { p [place] = l; }
         void         blank    () { free++; }
         int          get_free () { return free; }
         Lex        & operator[] (int index)
                      {
                        if ( index > size )
                          throw "POLIZ:out of array";
                        else
                          if ( index > free )
                            throw "POLIZ:indefinite element of array";
                          else
                            return p[index];
                      }
         void         print ()
                      {
                        for ( int i = 0; i < free; ++i )
			  //cout << p[i] << ' ';
                          cout << p[i] << endl;
			cout << endl;
                      }
};
 
////////////////////////////////////////////////////////////////////

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


const char * Scanner::TW [] = {"", "and", "bool", "do", "else", "if", "false", "int", "not", "or", "program",
                               "read", "then", "true", "var", "while", "write", "break", "string", NULL};
 
type_of_lex Scanner::words [] = {LEX_NULL, LEX_AND, LEX_BOOL, LEX_DO, LEX_ELSE, LEX_IF, LEX_FALSE, LEX_INT,
                     		 LEX_NOT, LEX_OR, LEX_PROGRAM, LEX_READ, LEX_THEN, LEX_TRUE, LEX_VAR, LEX_WHILE, LEX_WRITE, LEX_BREAK,
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

///////////////////////////  Класс Parser  /////////////////////////////////
 
class Parser 
{
         Lex          curr_lex;
         type_of_lex  c_type;
         int          c_val;
         Scanner      scan;
         Stack < int, 100 > st_int;
         Stack < type_of_lex, 100 >  st_lex;
	 Stack < int, 100 > st_break;
         void         P();
         void         D1();
         void         B();
         void         S();
         void         E();
         void         E1();
         void         E2();
         void         E3();
         void         E4();
         void         E5();
         void         dec ( type_of_lex type);
         void         check_id ();
         void         check_op ();
         void         check_not ();
         void         eq_type ();
         void         eq_bool ();
         void         check_id_in_read ();
         void         gl ()
                      {
                      	curr_lex = scan.get_lex();
                        c_type = curr_lex.get_type();
                        c_val = curr_lex.get_value();
                      }
	static bool   am_i_in_cycle;
	static int    break_counter;
public:
         Poliz        prog;
         Parser (const char *program ) : scan (program) ,prog (1000) {}
         void         analyze();
};
 
void Parser::analyze ()
{
	gl();
  	P();
  	prog.print();
  	cout << "\nThe syntax analization has finished SUCCESSFULLY!\n\n";
}


void Parser :: P ()
{
	if (c_type == LEX_PROGRAM)
		gl ();
	else throw curr_lex;
	
	if (c_type == LEX_BEGIN)
		gl ();
	else throw curr_lex;
	
	while (c_type == LEX_INT || c_type == LEX_BOOL || c_type == LEX_STRING)
	{
		D1();
	  	if (c_type == LEX_SEMICOLON) 
			gl();
	  	else
	    		throw curr_lex;
	}

	B();

  	if (c_type != LEX_FIN)
    		throw curr_lex;
}

void Parser :: D1 ()
{
	st_int.reset ();
	type_of_lex tmp = c_type;

	do
	{
		gl ();
		if (c_type != LEX_ID) throw curr_lex;
		st_int.push (c_val);
		st_lex.push (tmp);
    		gl ();

		if (c_type == LEX_ASSIGN)
		{
			gl ();
			prog.put_lex (Lex ( POLIZ_ADDRESS, c_val) ); //+
			E ();
			eq_type ();
			prog.put_lex (Lex (LEX_ASSIGN) ); //+
			
		}
		else st_lex.pop ();
	}
	while (c_type == LEX_COMMA);
	
	if (tmp == LEX_INT) 		dec (LEX_INT);
	else if (tmp == LEX_BOOL) 	dec (LEX_BOOL);
	else if (tmp == LEX_STRING) 	dec (LEX_STRING);
	else throw curr_lex;	
	
}

void Parser :: B ()
{
	do
	{
		S ();
	}
	while (c_type != LEX_END && c_type != LEX_FIN);
}

void Parser::S ()
{
	int pl0, pl1, pl2, pl3; //+
	/* IF () {} ELSE {} */
  	if (c_type == LEX_IF)
  	{
    		gl();

		if (c_type != LEX_LPAREN) throw "Expected ("; gl ();

    		E();
    		eq_bool();

		if (c_type != LEX_RPAREN) throw "Expected )"; gl ();		

    		pl2 = prog.get_free (); //+
    		prog.blank(); //+
    		prog.put_lex (Lex(POLIZ_FGO)); //+

    		if (c_type != LEX_BEGIN) throw "Expected {"; gl ();

		B ();

		pl3 = prog.get_free(); //+
	        prog.blank(); //+
	        prog.put_lex (Lex(POLIZ_GO)); //+
	        prog.put_lex (Lex(POLIZ_LABEL, prog.get_free()), pl2); //+

		if (c_type != LEX_END) throw "Expected }"; gl ();		
		if (c_type == LEX_ELSE)
		{
			gl();
			if (c_type != LEX_BEGIN) throw "Expected {"; gl ();

			B();
			prog.put_lex (Lex(POLIZ_LABEL, prog.get_free()), pl3); //+

			if (c_type != LEX_END) throw "Expected }"; gl ();

		}
  	}
	/* DO {} WHILE (); */ //!!! POLIZ
  	else if (c_type == LEX_DO)
	{
		bool previous_flag = am_i_in_cycle;
		am_i_in_cycle = true;
		int prev_break_num = break_counter;
		break_counter = 0;

    		pl0=prog.get_free(); //+
    		gl ();

		if (c_type != LEX_BEGIN) throw curr_lex; gl ();	

    		B ();

		if (c_type != LEX_END) throw curr_lex; gl ();
    		if (c_type != LEX_WHILE) throw curr_lex; gl ();
		if (c_type != LEX_LPAREN) throw curr_lex; gl ();

		E ();
		eq_bool ();

		if (c_type != LEX_RPAREN) throw curr_lex; gl ();
		if (c_type != LEX_SEMICOLON) throw curr_lex; gl ();

		pl1=prog.get_free(); //+
		prog.blank(); //+
    		prog.put_lex (Lex(POLIZ_FGO)); //+

		prog.put_lex (Lex (POLIZ_LABEL, pl0)); //+
		prog.put_lex (Lex ( POLIZ_GO)); //+
		prog.put_lex (Lex ( POLIZ_LABEL, prog.get_free()), pl1); //+


		for (int i = 0; i < break_counter; i++)
		{
			prog.put_lex( Lex (POLIZ_LABEL, prog.get_free() - 1), st_break.pop() );
		}

		am_i_in_cycle = previous_flag;
		break_counter = prev_break_num;	
  	}
	/* WHILE () DO {}; */
  	else if (c_type == LEX_WHILE)
	{
		bool previous_flag = am_i_in_cycle;
		am_i_in_cycle = true;
		int prev_break_num = break_counter;
		break_counter = 0;

    		pl0=prog.get_free(); //+
    		gl ();

		if (c_type != LEX_LPAREN) throw curr_lex; gl ();

		E ();
		eq_bool ();

		if (c_type != LEX_RPAREN) throw curr_lex; gl ();

		pl1=prog.get_free(); //+
		prog.blank(); //+
    		prog.put_lex (Lex(POLIZ_FGO)); //+

		if (c_type != LEX_BEGIN) throw curr_lex; gl ();	

    		B ();

		if (c_type != LEX_END) throw curr_lex; gl ();

		prog.put_lex (Lex (POLIZ_LABEL, pl0)); //+
		prog.put_lex (Lex ( POLIZ_GO)); //+
		prog.put_lex (Lex ( POLIZ_LABEL, prog.get_free()), pl1); //+


		for (int i = 0; i < break_counter; i++)
		{
			prog.put_lex( Lex (POLIZ_LABEL, prog.get_free() - 1), st_break.pop() );
		}
		
		am_i_in_cycle = previous_flag;
		break_counter = prev_break_num;		
  	}

	/* READ */
  	else if (c_type == LEX_READ)
  	{
    		gl ();

    		if (c_type != LEX_LPAREN) throw curr_lex;
		
		do
		{
			gl ();
			if (c_type != LEX_ID) throw curr_lex;
			check_id_in_read();
			prog.put_lex (Lex ( POLIZ_ADDRESS, c_val) ); //+
			prog.put_lex (Lex (LEX_READ)); 		     //+
			gl();
		}
		while (c_type == LEX_COMMA);
      			
	    	if (c_type != LEX_RPAREN)    throw curr_lex; gl ();
		if (c_type != LEX_SEMICOLON) throw curr_lex; gl ();
  	}
	/* WRITE */
  	else if (c_type == LEX_WRITE)
  	{
    		gl ();

    		if (c_type != LEX_LPAREN) throw curr_lex;
		
		do
		{
      			gl ();
			E ();
			prog.put_lex (Lex(LEX_WRITE)); //+
		}
		while (c_type == LEX_COMMA);

		if (c_type != LEX_RPAREN)    throw curr_lex; gl ();
		if (c_type != LEX_SEMICOLON) throw curr_lex; gl ();
  	}
	/* ASSIGN */
  	else if ( c_type == LEX_ID )
  	{
    		check_id ();
    		prog.put_lex (Lex ( POLIZ_ADDRESS, c_val) ); //+
		gl ();


		if (c_type != LEX_ASSIGN) throw curr_lex; gl ();
	
		E ();
		eq_type ();
		
		prog.put_lex (Lex (LEX_ASSIGN) ); //+

		if (c_type != LEX_SEMICOLON ) throw curr_lex; gl ();
  	}
	/* BREAK */
	else if ( c_type == LEX_BREAK )
	{
		if (am_i_in_cycle == false) throw "POLIZ : Break used out of cycle!!!\n";
		gl ();
		if ( c_type != LEX_SEMICOLON ) throw curr_lex; gl ();

		st_break.push (prog.get_free ());
		break_counter++;
		prog.blank ();
		prog.put_lex (Lex (POLIZ_GO) );
	}
  	else if ( c_type != LEX_FIN )
	{
		cerr << "S -> no way\n";
		throw curr_lex;
	}
}

void Parser::E () 
{
	E1();
	while ( c_type == LEX_OR )
  	{
    		st_lex.push (c_type);
    		gl();
    		E1(); 
    		check_op();
  	}
}
 
void Parser::E1 ()
{
	E2();
  	while ( c_type == LEX_AND )
  	{
    		st_lex.push (c_type);
    		gl();
    		E2();
    		check_op();
  	}
}
 
void Parser::E2 ()
{
	E3();
  	if ( c_type == LEX_EQ  || c_type == LEX_LSS || c_type == LEX_GTR ||
             c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ )
  	{
    		st_lex.push (c_type);
   		gl();
   		E3();
   		check_op();
  	}
}

void Parser::E3 ()
{
	E4();
  	while ( c_type == LEX_PLUS || c_type == LEX_MINUS)
  	{
    		st_lex.push (c_type);
   		gl();
   		E4();
   		check_op();
  	}
}

void Parser::E4 ()
{
	E5();
  	while ( c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_REM)
  	{
    		st_lex.push (c_type);
   		gl();
   		E5();
   		check_op();
  	}
}

void Parser::E5 () 
{
	if ( c_type == LEX_ID ) 
  	{
    		check_id();
    		prog.put_lex (Lex (LEX_ID, c_val)); //+
    		gl();
  	}
  	else if ( c_type == LEX_NUM ) 
  	{
	    	st_lex.push ( LEX_INT );
	    	prog.put_lex ( curr_lex ); //+
	    	gl();
  	}
	else if ( c_type == LEX_STRING ) 
  	{
	    	st_lex.push ( LEX_STRING );
	    	prog.put_lex ( curr_lex ); //+
	    	gl();
  	}
  	else if ( c_type == LEX_TRUE) 
  	{
    		st_lex.push ( LEX_BOOL );
    		prog.put_lex (Lex (LEX_TRUE, 1) ); //+
    		gl();
  	}
  	else if ( c_type == LEX_FALSE)
  	{
    		st_lex.push ( LEX_BOOL );
    		prog.put_lex (Lex (LEX_FALSE, 0) ); //+
    		gl();
  	}
  	else if (c_type == LEX_NOT) 
  	{
    		gl(); 
    		E5(); 
    		check_not();
  	}
  	else if ( c_type == LEX_LPAREN ) 
  	{
    		gl(); 
    		E();
    		if ( c_type != LEX_RPAREN) throw curr_lex; gl();
  	}
  	else 
	{
		cerr << "Parser : E5 () : Unexpected lexemm";
		throw curr_lex;
	}
}
 
 
void Parser::dec ( type_of_lex type ) 
{
	int i;
  	while ( !st_int.is_empty()) 
  	{
    		i = st_int.pop();
    		if ( TID[i].get_declare() ) 
      			throw "Double declaration";
	    	else 
	    	{
	      		TID[i].put_declare();
	      		TID[i].put_type(type);
	    	}
  	}
}
 
void Parser::check_id () 
{
	if ( TID[c_val].get_declare() )
    		st_lex.push ( TID[c_val].get_type() );
  	else 
    		throw "Not declared";
}
 
void Parser::check_op () 
{
	type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOL;
 
  	t2 = st_lex.pop();
  	op = st_lex.pop();
  	t1 = st_lex.pop();

	if (op == LEX_PLUS)
		if (t1 == LEX_STRING) 
		{
			r = LEX_STRING;
			t = t1;
		}
		else r = LEX_INT;

  	if (op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH || op == LEX_REM)
    		r = LEX_INT;

  	if (op == LEX_OR || op == LEX_AND)
    		t = LEX_BOOL;

	if (op == LEX_EQ  || op == LEX_LSS || op == LEX_GTR ||
            op == LEX_LEQ || op == LEX_GEQ || op == LEX_NEQ)
	{
		if (t1 == LEX_STRING) t = LEX_STRING;
	}

  	if (t1 == t2  &&  t1 == t) 
    		st_lex.push(r);
  	else
	{
    		throw "Wrong types are in operation";
	}

  	prog.put_lex (Lex (op) ); //+
}
 
void Parser::check_not () 
{
	if (st_lex.pop() != LEX_BOOL)
    		throw "Wrong type is in not";
  	else 
  	{
    		st_lex.push (LEX_BOOL);

    		prog.put_lex (Lex (LEX_NOT)); //+
  	}
}
 
void Parser::eq_type () 
{
	if (st_lex.pop() != st_lex.pop())
    		throw "Wrong types are in =";
}
 
void Parser::eq_bool () 
{
	if ( st_lex.pop() != LEX_BOOL )
    		throw "Expression is not bool";
}
 
void Parser::check_id_in_read ()
{
	if ( !TID [c_val].get_declare() )
    		throw "Not declared";
}

/*
class Executer
{
	Lex	pc_el;
public:
	void	execute ( Poliz & prog );
};
 
void Executer::execute ( Poliz & prog )
{
	Stack < int, 100 > args;
	int i, j, index = 0, size = prog.get_free();
	while ( index < size )
	{
		pc_el = prog [ index ];
    		switch ( pc_el.get_type () )
	    	{
			case LEX_TRUE: case LEX_FALSE: case LEX_NUM: 
			case POLIZ_ADDRESS: case POLIZ_LABEL:
				args.push ( pc_el.get_value () );
				break;

	      		case LEX_ID:
				i = pc_el.get_value ();
				if ( TID[i].get_assign () )
				{
		  			args.push ( TID[i].get_value () );
		  			break;
				}
				else
		  			throw "POLIZ: indefinite identifier";

	      		case LEX_NOT:
				args.push( !args.pop() );
				break;

	      		case LEX_OR:
				i = args.pop();
				args.push ( args.pop() || i );
				break;

	      		case LEX_AND:
				i = args.pop();
				args.push ( args.pop() && i );
				break;

	      		case POLIZ_GO:
				index = args.pop() - 1;
				break;

	      		case POLIZ_FGO:
				i = args.pop();
				if ( !args.pop() ) index = i-1;
				break;

	      		case LEX_WRITE:
				cout << args.pop () << endl;
				break;

	      		case LEX_READ:
			{
		  		int k;
		  		i = args.pop ();
		  		if ( TID[i].get_type () == LEX_INT )
		  		{
		    			cout << "Input int value for" << TID[i].get_name () << endl;
		    			cin >> k;
		  		}
		  		else
		  		{
		    			char j[20];
					rep:
						cout << "Input boolean value (true or false) for" << TID[i].get_name() << endl;
						cin >> j;
						if 	(!strcmp(j, "true"))  k = 1;
						else if (!strcmp(j, "false")) k = 0;
						else
						{
							cout << "Error in input:true/false" << endl;
							goto rep;
						}
				}
				TID[i].put_value (k);
				TID[i].put_assign ();
				break;
			}

		      	case LEX_PLUS:
				args.push ( args.pop() + args.pop() );
				break;

			case LEX_TIMES:
				args.push ( args.pop() * args.pop() );
				break;

			case LEX_MINUS:
				i = args.pop();
				args.push ( args.pop() - i );
				break;

			case LEX_SLASH:
				i = args.pop();
				if (!i)
				{
					args.push(args.pop() / i);
					break;
				}
				else throw "POLIZ:divide by zero";

			case LEX_EQ:
				args.push ( args.pop() == args.pop() );
				break;

			case LEX_LSS:
				i = args.pop();
				args.push ( args.pop() < i);
				break;

			case LEX_GTR:
				i = args.pop();
				args.push ( args.pop() > i );
				break;

			case LEX_LEQ:
				i = args.pop();
				args.push ( args.pop() <= i );
				break;

			case LEX_GEQ:
				i = args.pop();
				args.push ( args.pop() >= i );
				break;

			case LEX_NEQ:
				i = args.pop();
				args.push ( args.pop() != i );
				break;

			case LEX_ASSIGN:
				i = args.pop();
				j = args.pop();
				TID[j].put_value(i);
				TID[j].put_assign(); 
				break;

			default:
				throw "POLIZ: unexpected elem";
		}//end of switch

		++index;

	};//end of while
	cout << "Finish of executing!!!" << endl;
}
 
class Interpretator
{
	Parser   pars;
	Executer E;
public:
	Interpretator  (char* program): pars (program) {}
	void	interpretation ();
};
 
void Interpretator::interpretation () ///////////////
{
	pars.analyze ();
	E.execute ( pars.prog );
}
*/
///////////////////////////////////////

bool   Parser :: am_i_in_cycle = false;
int    Parser :: break_counter = 0;

int main (int argc, char* argv[])
{
	Parser   pars (argv[1]);

	try
  	{
		pars.analyze ();
    		return 0;
  	}
  	catch (char c)
	{
		cout << "\nThe syntax analization has finished with an ERROR!\n\n";
    		cout << "unexpected symbol " << c << endl << endl;
    		return 1;
 	}
  	catch (Lex l)
  	{
		cout << "\nThe syntax analization has finished with an ERROR!\n\n";
		cout << "unexpected lexeme";
    		cout << l << endl << endl;
    		return 1;
  	}
  	catch (const char * source)
  	{
		cout << "\nThe syntax analization has finished with an ERROR!\n\n";
   		cout << source << endl << endl;
    		return 1;
  	}

}
