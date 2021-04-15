#include <iostream>
#include <fstream>
#include "../core/matrix.h"

using namespace std;

#define INPUT_FILE "../standard_input/full/input1.txt"
#define OUTPUT_FILE "output.txt"
#define ELEMENT_PRINT_WIDTH 6

template<typename T>
void Gauss(Matrix<T>& a, Matrix<T>& b, ostream& out);

int main()
{
	ifstream inp(INPUT_FILE);
	ofstream out(OUTPUT_FILE);
	Matrix<double> a(inp), b(inp);

	Gauss(a, b, out);

	inp.close();
	out.close();
	return 0;
}

template<typename T>
void Gauss(Matrix<T>& a, Matrix<T>& b, ostream& out) {
	if (Matrix<T>::CanJoin(a, b)) {
		Matrix<T> c;

		c.Join(a, b);
		c.elementPrintWidth = ELEMENT_PRINT_WIDTH;
		int singular_flag = c.ForwardElimination(out);

		int res = 0;
	    if (singular_flag != -1)
	    {	 
    		bool IsAllRowBelowZero = true;
	        for (int k = singular_flag; k < c.h; k++) {
	        	if (c.IsRowZero(singular_flag) == false) {
        			IsAllRowBelowZero = false;
        			break;
	        	}
	        }

	        if (IsAllRowBelowZero == false) {
	            res = 1;
	        } else {
	        	if (singular_flag == a.w)
	        		res = 0;
	        	else
	            	res = 2;
	        }
	    } else {
	    	if (a.h < a.w)
	    		res = 2;
	    	else
		    	res = 0;
	    }

	    out << "== Row echelon form ==" << endl << c;
	    switch (res) {
	    	case 0:
	    		c.BackSubstitution(out, b.w);
	    		break;
	    	case 1:
	    		out << endl << "Inconsistent System." << endl;
	    		break;
	    	case 2:
	    		out << endl << "May have infinitely many solutions." << endl;
	    		break;
	    }
	}
}