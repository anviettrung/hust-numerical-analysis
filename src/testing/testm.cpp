#include <iostream>
#include <fstream>
#include "../core/matrix.h"

using namespace std;

int main()
{
	ifstream inp("input.txt");
	ofstream out("output.txt");
	Matrix<double> a(inp);

	a.ForwardElimination();
	out << a;

	// if (Matrix<float>::CanJoin(a, b)) {
	// 	Matrix<float> c;
	// 	c.Join(a, b);
	// 	c.ForwardElimination();
	// 	out << c;
	// }

	inp.close();
	out.close();
	return 0;
}