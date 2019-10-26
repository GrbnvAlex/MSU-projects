#include <iostream>
#include <cmath>

using namespace std;

const double PI = 3.141592653589793238463;

class shape {
public:
	virtual double area () = 0;
	virtual void set (double a = 0, double b = 0) = 0;
};

// CIRCLE //
class circle : public shape {
	double radius;
public:
		circle () : radius(0) {}
		circle (double r) : radius(r) { if (r < 0) throw "circle :: circle: Wrong input, 1-st parameter"; }
		double area () { return (PI * radius * radius); }
		void set (double a, double b) 
		{
			radius = a;
			if (a < 0) throw "circle :: set: Wrong input, 1-st parameter"; 
		}
};

// RECTANGLE //
class rectangle : public shape {
	double height, width;
public:
	rectangle () : height(0), width(0) {}
	rectangle (double a, double b) : height(a), width(b) 
	{
		if (a < 0) throw "rectangle :: rectangle: Wrong input, 1-st parameter";
		if (b < 0) throw "rectangle :: rectangle: Wrong input, 2-nd parameter";
	}
	double area () { return (height * width); }
	void set (double a, double b) 
	{
		if (a < 0) throw "rectangle :: set: Wrong input, 1-st parameter";
		if (b < 0) throw "rectangle :: set: Wrong input, 2-nd parameter";
		height = a; 
		width = b; 
	}
};

// TORUS //
class torus : public shape {
	double r1, r2;
public:
	torus () : r1(0), r2(0) {}
	torus (double a, double b) : r1(a), r2(b)
	{
		if (a < 0) throw "torus :: torus: Wrong input, 1-st parameter";
		if (b < 0) throw "torus :: torus: Wrong input, 2-nd parameter";
	}
	double area () { return (4 * PI * r1 * r2); }
	void set (double a, double b) 
	{
		if (a < 0) throw "torus :: set: Wrong input, 1-st parameter";
		if (b < 0) throw "torus :: set: Wrong input, 2-nd parameter"; 
		r1 = a; 
		r2 = b; 
	}
};

// CONE //
class cone : public shape {
	double radius, generatrix;
public:
	cone () : radius(0), generatrix(0) {}
	cone (double a, double b) : radius(a), generatrix(b)
	{
		if (a < 0) throw "cone :: cone: Wrong input, 1-st parameter";
		if (b < 0) throw "cone :: cone: Wrong input, 2-nd parameter";
	}
	double area () { return (PI * radius * generatrix + PI * radius * radius ); }
	void set (double a, double b) 
	{ 
		if (a < 0) throw "cone :: set: Wrong input, 1-st parameter";
		if (b < 0) throw "cone :: set: Wrong input, 2-nd parameter";
		radius = a; 
		generatrix = b; 
	}
};

// CYLINDER //
class cylinder : public shape {
	double radius, height;
public:
	cylinder () : radius (0), height (0) {}
	cylinder (double a, double b) : radius(a), height(b)
	{
		if (a < 0) throw "cylinder :: cylinder: Wrong input, 1-st parameter";
		if (b < 0) throw "cylinder :: cylinder: Wrong input, 2-nd parameter";
	}
	double area () { return ( 2 * PI * radius * height + 2 * PI * radius * radius ); }
	void set (double a, double b) 
	{ 
		if (a < 0) throw "cylinder :: set: Wrong input, 1-st parameter";
		if (b < 0) throw "cylinder :: set: Wrong input, 2-nd parameter";
		radius = a; 
		height = b; 
	}
};


// PERFECT POLYGON //
class per_polygon : public shape {
	double side;
	int side_num;
public:
	per_polygon (int num, double s = 0) : side_num(num), side(s) 
	{
		if (num <= 2) throw "per_polygon :: per_polygon: Wrong input 1-st parameter";
		if (s < 0) throw "per_polygon :: per_polygon: Wrong input, 2-nd parameter";
	}
	double area () { return (side_num * side * side / 4 / tan(PI / side_num)); }
	void set (double a, double b) 
	{ 
		if (a < 0) throw "per_polygon :: set: Wrong input, 1-st parameter";
		side = a; 
	}
};

void print_area (shape* p) { cout << p -> area () << endl; }

double sum_area (shape** p, int n)
{
	double sum = 0;
	for (int i=0; i<n; i++) sum += p[i] -> area ();
	return sum;
}

int main () {

	const int size = 9;
	int i = 0;
	shape* p [size];
	try {
		p[i++] = new rectangle (2, 7);
		p[i++] = new circle (1);
		p[i++] = new per_polygon (3, 15);
		p[i++] = new cone (2, 7);
		p[i++] = new cylinder (22, 8.12);
		p[i++] = new torus (13.37, 26.74);
		p[i++] = new per_polygon (3, 12);
		p[i++] = new cone (32.9, 76.11);
		p[i++] = new rectangle (9, 2.777);
	}
	catch (const char* str) { 
		for (int j=0; j<i; j++) delete p[j];
		cout << str << endl;
		return 1; 
	}
	catch (bad_alloc) {
		cout << "Can't get memory for new" << endl;
		return 1;
	}

	for (i=0; i<size; i++) print_area(p[i]);
	cout << "------------------------\nSUM = " << sum_area(p, size) << endl;
	
	for (i=0; i<size; i++) delete p[i];
	
	
	return 0;
}
