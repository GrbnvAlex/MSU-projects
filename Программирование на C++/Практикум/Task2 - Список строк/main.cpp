#include "string_list.h"

int main () {
	
	signal(SIGSEGV, my_handler);

	int k = 1;

	// <<< Test #1 >>> Constructors & list (char*)

	cout << "\n<<< TEST #1 >>>\n\n";

	
	List A("cat");
	List B;
	List C = "cat";
	List E, F, G, H, I, J, K, L, M;
	
	
	A==C? cerr << "Passed " : cerr << "Didn't pass "; cerr << "1." << k++ << endl; // 1.1

	
	List D = A = B;


	A==B? cerr << "Passed " : cerr << "Didn't pass "; cerr << "1." << k++ << endl; // 1.2
	D==A? cerr << "Passed " : cerr << "Didn't pass "; cerr << "1." << k++ << endl; // 1.3
	D==B? cerr << "Passed " : cerr << "Didn't pass "; cerr << "1." << k++ << endl; // 1.4

	
	A = "     cat";
	B = "cat   ";
	E = ("     cat      ");
	D = "     c    at     ";
	
	

	A==B? cerr << "Passed " : cerr << "Didn't pass "; cerr << "1." << k++ << endl; // 1.5
	A==C? cerr << "Passed " : cerr << "Didn't pass "; cerr << "1." << k++ << endl; // 1.6
   	D!=A? cerr << "Passed " : cerr << "Didn't pass "; cerr << "1." << k++ << endl; // 1.7
	E==A? cerr << "Passed " : cerr << "Didn't pass "; cerr << "1." << k++ << endl; // 1.8

	// <<< Test #2 >>> list + list	
	
	k = 1;

	cout << "\n<<< TEST #2 >>>\n\n";

	
	A = "  cat  "; C = A;
	B = "ApPlE  "; D = B;
	B = B + A;
	
	

	C==A? cerr << "Passed " : cerr << "Didn't pass "; cerr << "2." << k++ << endl; // 2.1	
	B!=A? cerr << "Passed " : cerr << "Didn't pass "; cerr << "2." << k++ << endl; // 2.2
	B!=D? cerr << "Passed " : cerr << "Didn't pass "; cerr << "2." << k++ << endl; // 2.3

	
	A = "  CMELOSTb  "; B = "XUTPOCTb  ";
	C = "CMELOSTb"    ; D = "     XUTPOCTb     ";

	G = A; H = B;
	I = C; J = D;

	E = A + B; F = C + D;
	
	

	E==F? cerr << "Passed " : cerr << "Didn't pass "; cerr << "2." << k++ << endl; // 2.4
	A==G? cerr << "Passed " : cerr << "Didn't pass "; cerr << "2." << k++ << endl; // 2.5
	B==H? cerr << "Passed " : cerr << "Didn't pass "; cerr << "2." << k++ << endl; // 2.6
	C==I? cerr << "Passed " : cerr << "Didn't pass "; cerr << "2." << k++ << endl; // 2.7
	D==J? cerr << "Passed " : cerr << "Didn't pass "; cerr << "2." << k++ << endl; // 2.8

	// <<< Test #3 >>> list += list

	k = 1;

	cout << "\n<<< TEST #3 >>>\n\n";

	
	A = "Apple"; B = "Pen     "; E = B;
	A += B;

	C = "Pen     "; D = "     Pineapple     "; F = D;
	C += D; G = A; H = C;

	C += A;
	
	

	E==B? cerr << "Passed " : cerr << "Didn't pass "; cerr << "3." << k++ << endl; // 3.1
	F==D? cerr << "Passed " : cerr << "Didn't pass "; cerr << "3." << k++ << endl; // 3.2
	G==A? cerr << "Passed " : cerr << "Didn't pass "; cerr << "3." << k++ << endl; // 3.3
    C==(H+A)? cerr << "Passed " : cerr << "Didn't pass "; cerr << "3." << k++ << endl; // 3.4

	// <<< Test #4 >>> char* + list, list + char*

	k = 1;

	cout << "\n<<< TEST #4 >>>\n\n";

	
	A = "   Lom    ";
	A = A + "     Priem";

	B = "    Lom  ";
	C = "Priem           ";
	B += C;

	D = "   Priem";
	D = "  Lom" + D;
	
	

	A==B? cerr << "Passed " : cerr << "Didn't pass "; cerr << "4." << k++ << endl; // 4.1
	A==D? cerr << "Passed " : cerr << "Didn't pass "; cerr << "4." << k++ << endl; // 4.2

	// <<< Test #5 >>> list - char*

	k = 1;

	cout << "\n<<< TEST #5 >>>\n\n";
	
	
	A = "Dan";
	A = A + "Horffd";
	A = "Jeronimo" + A;

	B = A;
	A = A - "Dan";

	C = "Jeronimo"; C += "Horffd";
	
	

	A==C? cerr << "Passed " : cerr << "Didn't pass "; cerr << "5." << k++ << endl; // 5.1
	A!=B? cerr << "Passed " : cerr << "Didn't pass "; cerr << "5." << k++ << endl; // 5.2	

	
	A = "Dan";
	A = A + "Horffd";
	A = "Jeronimo" + A;

	B = A;
	A = A - "Jeronimo";

	C = "Dan"; C += "Horffd";
	

	A==C? cerr << "Passed " : cerr << "Didn't pass "; cerr << "5." << k++ << endl; // 5.3
	A!=B? cerr << "Passed " : cerr << "Didn't pass "; cerr << "5." << k++ << endl; // 5.4

	
	A = "Dan";
	A = A + "Horffd";
	A = "Jeronimo" + A;

	B = A;
	A = A - "Horffd";

	C = "Jeronimo"; C += "Dan";
	

	A==C? cerr << "Passed " : cerr << "Didn't pass "; cerr << "5." << k++ << endl; // 5.5
	A!=B? cerr << "Passed " : cerr << "Didn't pass "; cerr << "5." << k++ << endl; // 5.6	

	// <<< Test #6 >>> list[i]

	k = 1;

	cout << "\n<<< TEST #6 >>>\n\n";

	Str s1 = "Jeronimo";

	
	A = "Dan"; A = A + "Horffd"; A = "Jeronimo" + A;
	
	
	
	g(-1, A)==1? cerr << "Passed " : cerr << "Didn't pass "; cerr << "6." << k++ << endl; //6.1
	g(0, A)==1? cerr << "Passed " : cerr << "Didn't pass "; cerr << "6." << k++ << endl; // 6.2
	g(1, A)==0? cerr << "Passed " : cerr << "Didn't pass "; cerr << "6." << k++ << endl; // 6.3
	g(2, A)==0? cerr << "Passed " : cerr << "Didn't pass "; cerr << "6." << k++ << endl; // 6.4
	g(3, A)==0? cerr << "Passed " : cerr << "Didn't pass "; cerr << "6." << k++ << endl; // 6.5
	g(4, A)==1? cerr << "Passed " : cerr << "Didn't pass "; cerr << "6." << k++ << endl; // 6.6
	

	A[1]==s1? cerr << "Passed " : cerr << "Didn't pass "; cerr << "6." << k++ << endl; // 6.7

	
	A = "Dan"; A = A + "Horffd"; A = "Jeronimo" + A;
	s1 = "Dan";
	
	 
	A[2]==s1? cerr << "Passed " : cerr << "Didn't pass "; cerr << "6." << k++ << endl; // 6.8

	
	A = "Dan"; A = A + "Horffd"; A = "Jeronimo" + A;
	s1 = "Horffd";
	
	A[3]==s1? cerr << "Passed " : cerr << "Didn't pass "; cerr << "6." << k++ << endl; // 6.9

	// <<< Test #7 >>> to char*

	k = 1;

	cout << "\n<<< TEST #7 >>>\n\n";

	A = "4 balls";
	A = "3 big cats" + A; A = "2 boys" + A; A = "1 large animal" + A;
	A = A + "5 lifes";
	
	cout << A << endl;

	return 0;
}
