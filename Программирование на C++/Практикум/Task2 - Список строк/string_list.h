#include <iostream>
#include <cstring>
#include <csignal>
#include "stdlib.h"

using namespace std;

void my_handler (int signum);

class Error {
	char* comment;
public:
	Error (const char* cmt);
	~Error ();
	Error (const Error& a);
	const char* GetCmt() const { return comment; }
private:
	static char* strdup (const char* str);
};


char* delete_spaces(const char* s);

class Str {
public:
	char *p;
	int size;

  	Str (const char *str = "\0");
  	Str (const Str &a);
  	~Str () { delete[] p; }
  	Str& operator= (const Str &a);
	friend ostream & operator << (ostream & s, const Str a);
  	friend bool operator == (const Str& one, const Str& two);
};

class Node : public Str {
public:
	Str c;
	Node *next;
};

class List : public Node {
public:	Node *head;
	
public:
	List () { head = NULL; }
	List (const char* s); 
   	~List (); 
	List (const List& orig); 
	List& operator = (const List& orig);
	friend bool operator == (const List& one, const List& two);
	friend bool operator != (const List& one, const List& two);

	void print (); 
	void clean (); 
	void add_beg (Str c1); 
	void add_end (Str c1); 
	void add_list_in_tail (const List& tail);
	void delete_node (Str c1);

	friend List & operator += (List& hed, const List& tail);
	friend const List & operator + (const List& one, const List& two);
	friend const List & operator + (const char* s, const List& two);
	friend const List & operator + (const List& one, const char* s);
	friend const List & operator - (const List& one, const char* s);
	
	const Str operator [] (int i);
	friend ostream & operator << (ostream & s, const List& a);	
};

int g (int m, List & A);

