#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;


class Matrix
{
public:

	Matrix(int rows, int cols) : m_Rows(rows), m_Cols(cols)
	{
		m_Mat = new int[m_Rows*m_Cols];
	}

	virtual ~Matrix() 
	{
		if (m_Mat)
			delete[] m_Mat;
	}

	int& operator()(int i, int j)
	{
		return m_Mat[m_Cols*i + j];
	}

	void readMat()
	{
		for (int i = 0; i < m_Rows; ++i)
		{
			for (int j = 0; j < m_Cols; ++j)
			{
				cin >> m_Mat[m_Cols*i + j];
			}
		}
	}

	void writeMat()
	{
		for (int i = 0; i < m_Rows; ++i)
		{
			for (int j = 0; j < m_Cols; ++j)
			{
				cout << m_Mat[m_Cols*i + j] << " ";
			}
			cout << "\n";
		}
	}

protected:

	int* m_Mat;
	int m_Rows;
	int m_Cols;
};


class Contour
{
public:

	Contour(int minR, int minC, int maxR, int maxC) : m_MinR(minR), m_MinC(minC), m_MaxR(maxR), m_MaxC(maxC) {}

	void next(int i, int j, int& iN, int& jN)
	{
		if (j == m_MinC)
		{
			if (i < m_MaxR)
			{
				iN = i + 1;
				jN = j;
			}
			else
			{
				iN = i;
				jN = j + 1;
			}
			return;
		}

		if (i == m_MaxR)
		{
			if (j < m_MaxC)
			{
				jN = j + 1;
				iN = i;
			}
			else
			{
				jN = j;
				iN = i - 1;
			}
			return;
		}

		if (j == m_MaxC)
		{
			if (i > m_MinR)
			{
				iN = i - 1;
				jN = j;
			}
			else
			{
				iN = i;
				jN = j - 1;
			}
			return;
		}

		if (i == m_MinR)
		{
			if (j > m_MinC)
			{
				jN = j - 1;
				iN = i;
			}
			else
			{
				jN = j;
				iN = i + 1;
			}
			return;
		}
	}

private:
	int m_MinR;
	int m_MinC;
	int m_MaxR;
	int m_MaxC;
};


int main() {
	/* Enter your code here. Read input from STDIN. Print output to STDOUT */

	int rows, cols, steps;
	cin >> rows >> cols >> steps;
	Matrix mat(rows, cols);
	mat.readMat();

	int nextCurrent;
	int nextNext;

	// conectivity constraint, traverse by connectivity not by scanning row and column
	int contours = min(rows, cols)/2;

	for (int c = 0; c < contours; ++c)
	{
		// starting element for the contour c = mat(c, c)
		// this is also the min index for both row and column
		// the max index (m - 1 - c, n - 1 - c)

		int sizeR = rows - 1 - c;
		int sizeC = cols - 1 - c;

		Contour con(c, c, sizeR, sizeC);

		int actualSteps = steps;
		if (sizeR + sizeC - 2 * c > 0)
			actualSteps = steps % ((sizeR + sizeC - 2*c)*2);

		for (int s = 0; s < actualSteps; ++s)
		{
			int i = c;
			int j = c;

			int iN = i;
			int jN = j;

			con.next(i, j, iN, jN);

			nextCurrent = mat(i, j);

			while (iN != c || jN != c) // contour visiting end condition
			{
				nextNext = mat(iN, jN);

				mat(iN, jN) = nextCurrent;
				nextCurrent = nextNext;
				i = iN;
				j = jN;
				con.next(i, j, iN, jN);
			}

			mat(iN, jN) = nextNext;
		}
	}

	mat.writeMat();
		
	return 0;
}
