#include <iostream>
#include <fstream>
#include <math.h>
#include <iomanip>

using namespace std;

#define FLOAT_EPSILON 0.000001f
#define DOUBLE_EPSILON 0.0000000001

template<typename T>
class Matrix {
protected:
	T* data;

public:
	int w;
	int h;
	int elementPrintWidth;

	Matrix();
	Matrix(int _w, int _h);
	Matrix(ifstream& in);
	~Matrix();
	void Init(int _w, int _h);

	friend ostream& operator<<(ostream& os, const Matrix& mat) {
		for (int i = 0; i < mat.h; i++) {
			os << "|";
			for (int j = 0; j < mat.w; j++) {
				os << std::right << std::setprecision(2) << std::setw(mat.elementPrintWidth) << mat.data[i * mat.w + j] << "|";
			}
			os << endl;
		}
		return os;
	}

	T Get(int x, int y) {
		return data[y * w + x];
	}

	void Set(int x, int y, T val) {
		data[y * w + x] = fabs(val) < DOUBLE_EPSILON ? 0 : val;
	}

	void DivideRow(int row, T divider) {
		for (int x = 0; x < w; x++)
			Set(x, row, Get(x, row) / divider);
	}

	void SwapRow(int r1, int r2) {
		for (int i = 0; i < w; i++) {
			T tmp = Get(i, r1);
			Set(i, r1, Get(i, r2));
			Set(i, r2, tmp);
		}
	}

	void ReduceRow(int rk, int ri, int amp) {
		for (int i = 0; i < w; i++)
			Set(i, rk, Get(i, rk) + amp * Get(i, ri));
	}

	bool IsRowZero(int row) {
		for (int i = 0; i < w; i++)
			if (Get(i, row) != 0)
				return false;

		return true;
	}

	static bool CanJoin(Matrix<T>& a, Matrix<T>& b) {
		return a.h == b.h;
	}

	void Join(Matrix<T>& a, Matrix<T>& b) {
		if (CanJoin(a, b) == false) return;

		Init(a.w + b.w, a.h);

		for (int y = 0; y < a.h; y++) {

			for (int x = 0; x < a.w; x++)
				Set(x, y, a.Get(x, y));

			for (int x = 0; x < b.w; x++)
				Set(a.w + x, y, b.Get(x, y));
		}
	}

	int ForwardElimination(ostream& os, bool printState) {
		if (printState) {
			os << "== Forward Elimination Process ==" << endl;
			os << (*this) << endl;
		}

		int k_max = h;
	    for (int k = 0; k < k_max; k++)
	    {
	        // Initialize maximum value and index for pivot
	        int i_max = k;
	        int v_max = Get(k, i_max);
	 
	        /* find greater amplitude for pivot if any */
	        for (int i = k + 1; i < k_max; i++)
	            if (fabs(Get(k, i)) > v_max) {
	                v_max = Get(k, i);
	                i_max = i;
	            }
	 
	        /* if a prinicipal diagonal element  is zero,
	         * it denotes that matrix is singular, and
	         * will lead to a division-by-zero later. */
	        if (Get(k, i_max) == 0) {
	        	if (printState)
	        		os << "= End Early at k = " << k << endl;
	        	return k;
	        }
	 
	        /* Swap the greatest value row with current row */
	        if (i_max != k) {
	  			if (printState)
	  				os << "= Swap Row " << k+1 << " and " << i_max+1 << endl; 
	            SwapRow(k, i_max);
	        }
	 
	 
	        for (int i = k + 1; i < h; i++)
	        {
	            /* factor f to set current row kth element to 0,
	             * and subsequently remaining kth column to 0 */
	            T f = Get(k, i) / Get(k, k);
	            //T f = this(i, k) / this(k, k);
	 
	            /* subtract fth multiple of corresponding kth
	               row element*/
	            for (int j = k + 1; j < w; j++) {
	            	//this(i, j) -= f * this(k, j);
	                Set(j, i, Get(j, i) - f * Get(j, k));
	            }
	 
	            /* filling lower triangular matrix with zeros*/
	            //this(i, k) = 0;
	            Set(k, i, 0);
	        }
	 		
	 		if (printState)
	        	os << (*this) << endl;       //for matrix state
	    }

	    if (printState)
	   		os << (*this) << endl;          //for matrix state
	   	
	    return -1;
	}

	int ForwardElimination() { return ForwardElimination(cout, false); }
	int ForwardElimination(ostream& os) { return ForwardElimination(os, true); }

	// function to calculate the values of the unknowns
	void BackSubstitution(ostream& os, int RHSwidth)
	{
		int LHSwidth = w - RHSwidth;
	    Matrix<T> x(RHSwidth, LHSwidth); // An array to store solution
	 	x.elementPrintWidth = elementPrintWidth;

	    /* Start calculating from last equation up to the
	       first */
	    for (int i = w-1; i >= 0; i--)
	    {
	        /* start with the RHS of the equation */
	        for (int k = 0; k < RHSwidth; k++)
	        	x.Set(k, i, Get(LHSwidth + k, i));
	        	//x[i] = mat[i][N];
	 
	        /* Initialize j to i+1 since matrix is upper
	           triangular*/
	        for (int j = i + 1; j < LHSwidth; j++)
	        {
	            /* subtract all the lhs values
	             * except the coefficient of the variable
	             * whose value is being calculated */
	        	for (int k = 0; k < RHSwidth; k++)
	        		x.Set(k, i, x.Get(k, i) - Get(j, i) * x.Get(k, j));
	           // x[i] -= mat[i][j]*x[j];
	        }
	 
	        /* divide the RHS by the coefficient of the
	           unknown being calculated */
			for (int k = 0; k < RHSwidth; k++)
	        	x.Set(k, i, x.Get(k, i) / Get(i, i));
	        // x[i] = x[i]/mat[i][i];
	    }
	 
	    os << "== Solution for the system ==" << endl;
	    os << x << endl;
	}
};

// =========================
// Initialization
// =========================
template<typename T>
Matrix<T>::Matrix() {
	Init(0, 0);
}

template<typename T>
Matrix<T>::Matrix(int _w, int _h) {
	Init(_w, _h);
}

template<typename T>
Matrix<T>::Matrix(ifstream& in) {
	in >> w >> h;
	Init(w, h);
	for (int i = 0; i < w * h; i++)
		in >> data[i];
}

template<typename T>
void Matrix<T>::Init(int _w, int _h) {
	w = _w;
	h = _h;

	data = new T[w * h];

	elementPrintWidth = 4;
}

template<typename T>
Matrix<T>::~Matrix() {
	if (data != NULL)
		delete[] data;
}

// template<typename T>
// T& Matrix<T>::operator()(int y, int x) {
// 	if (x < 0 || x >= w || y < 0 || y >= h) {
// 		cout << "Access Matrix out of bound!" << endl;
// 		exit(0);
// 	}
// 	return data[y * w + x];
// }