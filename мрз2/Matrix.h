#pragma once
#ifndef OKMD_MATRIX_H
#define OKMD_MATRIX_H

#include "TripleMatrix.h"

class Matrix {
public:
	Matrix(int xSize, int ySize);
	void setAt(int x, int y, double value);
	double getAt(int x, int y)const;
	const int ySize;
	const int xSize;
private:
	const double MIN_BOUND = -1.0;
	const double MAX_BOUND = 1.0;
	std::vector<std::vector<double>> matrix;
	void initMatrix();
	double random() const noexcept;
};


#endif //OKMD_MATRIX_H
