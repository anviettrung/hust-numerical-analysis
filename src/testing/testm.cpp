#include <iostream>
#include <fstream>
#include "../core/matrix.h"

using namespace std;

#define INPUT_FILE "../standard_input/full_rows/input2.txt"
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

		int* index = new int[a.h];
		for (int u = 0; u < a.h; u++)
			index[u] = -1;
		c.ForwardElimination(index, out);

		int correct = 0, incorrect = 0;
		for (int i = 0; i < c.h; i++) {
			cout << index[i] << endl;
			if (0 <= index[i] && index[i] < a.w)
				correct++;
			else if (index[i] >= a.w)
				incorrect++;
		}

		out << "== Row echelon form ==" << endl << c;
		if (incorrect > 0)
			out << endl << "Inconsistent System." << endl;
		else if (correct == a.w)
			c.BackSubstitution(out, b.w);
		else
			out << endl << "Infinite solutions depend on " << a.w - correct << " parameters." << endl;
	}
}