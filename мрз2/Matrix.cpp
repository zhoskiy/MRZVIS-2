#include "pch.h"
#include "Matrix.h"
#include <random>

using namespace std;

Matrix::Matrix(const int ySize, const int xSize) :
	xSize(xSize),
	ySize(ySize)
{
	initMatrix();
}

double Matrix::random() const noexcept {
	std::uniform_real_distribution<double> unif(MIN_BOUND, MAX_BOUND);
	std::default_random_engine  engine(random_device{}());
	double random = unif(engine);
	return random;
}

void Matrix::initMatrix() {
	matrix = vector<vector<double>>(ySize, vector<double>(xSize, 0));
	for (int i = 0; i < ySize; ++i) {
		for (int j = 0; j < xSize; ++j) {
			matrix[i][j] = random();
		}
	}
}

void Matrix::setAt(const int x, const int y, double value) {
	matrix[x][y] = value;
}

double Matrix::getAt(const int x, const int y)const {
	return matrix[x][y];
}

