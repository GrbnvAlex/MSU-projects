#include "string_list.h"

void my_handler (int signum)
{
	cerr << "Memory ERROR!!!\n";
	exit(1);
}

	Error :: Error (const char* cmt)
	{
		comment = strdup(cmt);
	}

	Error :: ~Error ()
	{
		delete [] comment;
	}

	Error :: Error (const Error& a)
	{
		comment = strdup(a.comment);
	}

	char* Error :: strdup (const char* str)
	{
		char* p;
		try {
			p = new char [strlen(str) + 1];

			strcpy(p, str);
			return p;
		}
		catch (bad_alloc) {
			p = NULL;
			throw Error("No free memory for <new>!\n");
		}
	}

char* delete_spaces(const char* s)
{
	if (s == NULL) return NULL;
	int bad_space = 0;

	const char *from = s;
	while (*(from) == ' ') { from++; bad_space++; }
	if (bad_space == strlen(s)) return NULL;

	const char* to = s + strlen(s) - 1;
	while(*(to) == ' ') { to--; bad_space++; }

	char* res;
	try {
		res = new char [strlen(s) - bad_space + 1];

		for (const char* i=from; i<=to; i++)
		res[i-from] = *(i);
		res[strlen(s) - bad_space] = '\0';

		return res;
	}
	catch (bad_alloc) {
		res = NULL;
		throw Error("No free memory for <new>!\n");
	}

	
}

	////////// STR //////////

	Str :: Str (const char *str)
	{
	    try {
	   	p = new char [strlen(str) + 1];

		 size = strlen(p);
	    	 p = delete_spaces(str);
	    }
	    catch (bad_alloc) {
			p = NULL;
			throw Error("No free memory for <new>!\n");
	    }    
	   
	}

	Str :: Str (const Str &a)
	{
	    try {
	    	p = new char [(size = a.size) + 1];

		strcpy ( p, a.p );
	    }
	    catch (bad_alloc) {
			p = NULL;
			throw Error("No free memory for <new>!\n");
	    } 
	   
	}

	Str& Str :: operator= (const Str &a)
	{
	    if ( this == &a ) return *this;
	    delete [] p;
	    try {
	    	p = new char [a.size + 1];

		p = delete_spaces(a.p);
	        size = strlen(p);
	        return *this;
	    }
	    catch (bad_alloc) {
			p = NULL;
			throw Error("No free memory for <new>!\n");
	    } 
	    
	}

	ostream & operator << (ostream & s, const Str a)
	{
		s << a.p << endl;
		return s;
	}
	
	bool operator == (const Str& one, const Str& two)
	{
		return not(strcmp(one.p, two.p));
	}

	////////// LIST //////////

	List :: List (const char* s)
	{
		Node* tmp;
		try {
			tmp = new Node;

			tmp->c     = s;
			tmp->next  = NULL;

			head      = tmp;
		}
		catch (bad_alloc) {
			tmp = NULL;
			throw Error("No free memory for <new>!\n");
	   	} 
		
	}

	List :: ~List () 
	{
		while (head != NULL) 
		{
			Node* tmp = head->next;
			delete head;
			head = tmp;
		}
	}

	
	List :: List (const List& orig)
	{
		Node *tmp_orig = orig.head;
		
		head = NULL;
		if (orig.head == NULL) head = NULL;
		else while (tmp_orig != NULL) 
		     {
		     	add_end(tmp_orig->c);
		     	tmp_orig = tmp_orig->next;
		     }
	}
	
	List& List :: operator= (const List& orig)
	{
		if (&orig == this) return *this;
		Node *tmp_orig = orig.head;
		clean();
		head = NULL;
		if (orig.head == NULL) head = NULL;
		else while (tmp_orig != NULL) 
		     {
		     	add_end(tmp_orig->c);
		     	tmp_orig = tmp_orig->next;
		     }
		return *this;
	}

	bool operator == (const List& one, const List& two)
	{
		Node* a = one.head;
		Node* b = two.head;

		while (a != NULL && b != NULL)
		{
			if (not(a->c == b->c)) return false;
			a = a->next;
			b = b->next;
		}

		if (a == NULL && b == NULL) return true;
		return false;
	}
	
	bool operator != (const List& one, const List& two)
	{
		return not (one == two);
	}

	void List :: print () 
	{
		Node* tmp = head;
		while (tmp != NULL) 
		{
			cout << tmp->c;
			tmp = tmp->next;
		}
		cout << endl;
	}


	void List :: clean () 
	{
		while (head != NULL) 
		{
			Node* tmp = head->next;
			delete head;
			head = tmp;
		}
	}
	
	void List :: add_beg (Str c1)
	{
		Node* tmp;
		try {
			tmp = new Node;

			tmp->c	  = c1;
			tmp->next = head;
			head 	  = tmp;
		}
		catch (bad_alloc) {
			tmp = NULL;
			throw Error("No free memory for <new>!\n");
	        } 
				
		
	}

	void List :: add_end (Str c1)
	{
		Node* tmp      = head;
		Node* new_node;
		try {
			new_node = new Node;

			new_node->c    = c1;
			new_node->next = NULL;

			if (head != NULL)
			{
				while (tmp->next != NULL) tmp = tmp->next;
				tmp->next = new_node;
			} else head = new_node;
		}
		catch (bad_alloc) {
			new_node = NULL;
			throw Error("No free memory for <new>!\n");
	        } 
		
		
	}	
	
	void List :: add_list_in_tail(const List& tail)
	{
		if (tail.head == NULL) return;
		Node* tmp_tail = tail.head;

		while (tmp_tail != NULL) 
	        {
	     		add_end(tmp_tail->c);
	     		tmp_tail = tmp_tail->next;
	        }
	}

	void List :: delete_node (Str c1)
	{
		if (head == NULL) return;
		Node* tmp_prev = head;
		Node* tmp_del  = head->next;
		
		if (head->c == c1) 
		{
			delete head;
			head = tmp_del;
			return;
		}

		while (tmp_del != NULL)
		{
			if (tmp_del->c == c1)
			{
				tmp_prev->next = tmp_del->next;
				delete tmp_del;
				return;
			}
			tmp_prev = tmp_prev->next;
			tmp_del  = tmp_del->next;
		}
	}

	List & operator += (List& hed, const List& tail)
	{
		hed.add_list_in_tail(tail);
	
		return hed;
	}

	const List & operator + (const List& one, const List& two)
	{
		List* tmp;
		try {
			tmp = new List;

			*tmp = one;
			tmp->add_list_in_tail(two);
		
			return *tmp;
		}
		catch (bad_alloc) {
			tmp = NULL;
			throw Error("No free memory for <new>!\n");
	        } 
		
	}

	const List & operator + (const char* s, const List& two)
	{
		List* tmp;
		try {
			tmp = new List;

			*tmp = two;
			tmp->add_beg(s);

			return *tmp;
		}
		catch (bad_alloc) {
			tmp = NULL;
			throw Error("No free memory for <new>!\n");
	        } 
		
	}

	const List & operator + (const List& one, const char* s)
	{
		List* tmp;
		try {
			tmp = new List;

			*tmp = one;
			tmp->add_end(s);

			return *tmp;
		}
		catch (bad_alloc) {
			tmp = NULL;
			throw Error("No free memory for <new>!\n");
	        } 

	}

	const List & operator - (const List& one, const char* s)
	{
		List* tmp;
		try {
			tmp = new List;

			*tmp = one;
			tmp->delete_node(s);

			return *tmp;
		}
		catch (bad_alloc) {
			tmp = NULL;
			throw Error("No free memory for <new>!\n");
	        } 
		
	}

	const Str List :: operator [] (int i)
	{
		if (i <= 0) throw Error("ERROR! Wrong Number: <=0\n");

		int j = 1;
		Node* tmp = head;

		while (tmp != NULL)
		{
			if (j++ == i) return tmp->c;
			tmp = tmp->next;
		}
		
		throw Error("ERROR! Wrong Number: too big\n");
			
	}
	
	ostream & operator << (ostream & s, const List& a)
	{
		Node* tmp = a.head;
		do {
			s << tmp->c;
			tmp = tmp->next;
		} while (tmp != NULL);
		return s;
	}

	int g (int m, List & A)
	{ 
		try { 
			A[m];
		}
	
		catch (Error& er)
		{
			cerr << er.GetCmt();
			return 1;
		}
		return 0;
	}

