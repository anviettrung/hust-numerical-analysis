#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

template<typename T>
class Matrix {
private:
	T* data;

public:
	int w;
	int h;

	Matrix() {}
	Matrix(int _w, int _h);
	Matrix(ifstream& in);
	~Matrix();

	void Init(int _w, int _h);

	friend ostream& operator<<(ostream& os, const Matrix& mat) {
		for (int i = 0; i < mat.h; i++) {
			for (int j = 0; j < mat.w; j++)
				os << mat.data[i * mat.w + j] << " ";
			os << endl;
		}
		return os;}

	T& operator()(int y, int x);

	T Get(int x, int y) {
		return data[y * w + x];
	}

	void Set(int x, int y, T val) {
		data[y * w + x] = val;
	}

	void DivideRow(int row, T divider) {
		for (int x = 0; x < w; x++)
			Set(x, row, Get(x, row) / divider);
			//this(row, x) /= divider;
	}

	static bool CanJoin(Matrix<T>& a, Matrix<T>& b) {
		return a.h == b.h;
	}

	void Join(Matrix<T>& a, Matrix<T>& b) {
		if (CanJoin(a, b) == false) return;
		Init(a.w + b.w, a.h);

		for (int y = 0; y < a.h; y++) {
			for (int x = 0; x < a.w; x++)
				//this(y, x) = a(y, x);
				Set(x, y, a.Get(x, y));
			for (int x = 0; x < b.w; x++)
				//this(y, a->w + x) = b(y, x);
				Set(a.w + x, y, b.Get(x, y));
		}
	}

	void SwapRow(int r1, int r2) {
		for (int i = 0; i < w; i++) {
			T tmp = Get(i, r1);
			//(r1, i) = (r2, i); //Get(r2, i);
			Set(i, r1, Get(i, r2));
			//(r2, i) = tmp;
			Set(i, r2, tmp);
		}
	}

	void ReduceRow(int rk, int ri, int amp) {
		for (int i = 0; i < w; i++)
			//this(rk, i) += amp * this(ri, i);
			Set(i, rk, Get(i, rk) + amp * Get(i, ri));
	}

	int ForwardElimination() {
	    for (int k = 0; k < h; k++)
	    {
	        // Initialize maximum value and index for pivot
	        int i_max = k;
	        int v_max = Get(k, i_max);
	 
	        /* find greater amplitude for pivot if any */
	        for (int i = k + 1; i < h; i++)
	            if (fabs(Get(k, i)) > v_max) {
	                v_max = Get(k, i);
	                i_max = i;
	            }
	 
	        /* if a prinicipal diagonal element  is zero,
	         * it denotes that matrix is singular, and
	         * will lead to a division-by-zero later. */
	        if (Get(k, i_max) == 0) {
	            return k; // Matrix is singular
	        }
	 
	        /* Swap the greatest value row with current row */
	        if (i_max != k)
	            SwapRow(k, i_max);
	 
	 
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
	 
	        //print(mat);        //for matrix state
	    }
	    //print(mat);            //for matrix state
	    return -1;
	}
};

// =========================
// Initialization
// =========================
template<typename T>
Matrix<T>::Matrix(int _w, int _h) {
	Init(_w, _h);
}

template<typename T>
Matrix<T>::Matrix(ifstream& in) {
	in >> w >> h;
	data = new T[w * h];
	for (int i = 0; i < w * h; i++)
		in >> data[i];
}

template<typename T>
Matrix<T>::~Matrix() {
	if (data != NULL)
		delete[] data;
}

template<typename T>
void Matrix<T>::Init(int _w, int _h) {
	w = _w;
	h = _h;

	data = new T[w * h];
}

template<typename T>
T& Matrix<T>::operator()(int y, int x) {
	if (x < 0 || x >= w || y < 0 || y >= h) {
		cout << "Access Matrix out of bound!" << endl;
		exit(0);
	}
	return data[y * w + x];
}