#include "pch.h"
#include "Controller.h"
#include <thread>
#include <random>
#include <iostream>

using namespace std;


Controller::Controller(int p, int q, int m, int count) :
	p(p),
	q(q),
	m(m),
	threadCount(count),
	lavg(0.0) {
	a = new Matrix(p, m);
	b = new Matrix(m, q);
	e = new Matrix(1, m);
	g = new Matrix(p, q);
	strangeD = new Matrix(p, q);
	strangeF = new Matrix(p, q);
	maxStrange = new Matrix(p, q);
	f = new TripleMatrix(p, q, m);
	d = new TripleMatrix(p, q, m);
	tacts = 0;
}

void Controller::getTripleMatrixProduct() {
	vector<thread> threads;
	int i = 0, j = 0;
	bool mat = true;
	while (i < p || j < p) {
		bool whatTact = false;
		for (int k = 0; k < threadCount; ++k) {
			if (mat && i < p) {
				threads.emplace_back(&Controller::calcD, this, i);
				i++;
			}
			else if (!mat && j < p) {
				threads.emplace_back(&Controller::calcF, this, j);
				j++;
				whatTact = true;
			}
			mat = !mat;
		}
		if (whatTact) {
			tacts += m * q * 10;
			lavg += m * q * 10;
		}
		else {
			tacts += m * q;
			lavg += m * q;
		}
		for (auto &t : threads)t.join();
		threads.clear();
	}
}

void Controller::calcD(const int index) {
	int maxi, maxj, maxk;
	d->getSize(maxi, maxj, maxk);
	for (int j = 0; j < maxj; ++j) {
		for (int k = 0; k < maxk; ++k) {
			d->setAt(index, j, k, a->getAt(index, k) * b->getAt(k, j));
		}
	}
}

void Controller::calcF(const int index) {
	int maxi, maxj, maxk;
	d->getSize(maxi, maxj, maxk);
	for (int j = 0; j < maxj; ++j) {
		for (int k = 0; k < maxk; ++k) {
			double result = 0;
			result += supAB(index, j, k) * (2 * e->getAt(0, k) - 1) * e->getAt(0, k);
			result += supBA(index, j, k) * (1 + (4 * (supAB(index, j, k)) - 2)*e->getAt(0, k)) * (1 - e->getAt(0, k));
			f->setAt(index, j, k, result);
		}
	}
}

double Controller::supAB(int i, int j, int k) {
	double result = 0;
	double aik = 1 - a->getAt(i, k);
	double bkj = b->getAt(k, j);
	if (aik > bkj) {
		result = bkj / aik;
	}
	else if (aik <= bkj) {
		result = 1.0;
	}
	return result;
}

double Controller::supBA(int i, int j, int k) {
	double result = 0;
	double aik = a->getAt(i, k);
	double bkj = 1 - b->getAt(k, j);
	if (aik < bkj) {
		result = aik / bkj;
	}
	else if (aik <= bkj) {
		result = 1.0;
	}
	return result;
}

void Controller::calcStrange() {
	vector<thread> threads;
	int i = 0, j = 0;
	bool mat = true;
	while (i < p || j < p) {
		for (int k = 0; k < threadCount; ++k) {
			if (mat && i < p) {
				threads.emplace_back([this, i]() {
					for (int j = 0; j < q; ++j) {
						double res = 1;
						for (int k = 0; k < m; ++k) {
							res *= f->getAt(i, j, k);
						}
						strangeF->setAt(i, j, res);
					}
				});
				i++;
			}
			else if (!mat && j < p) {
				threads.emplace_back([this, j]() {
					for (int z = 0; z < q; ++z) {
						double res = 1;
						for (int k = 0; k < m; ++k) {
							res *= (1 - d->getAt(j, z, k));
						}
						strangeD->setAt(j, z, 1 - res);
					}
				});
				j++;
			}
			mat = !mat;
		}
		tacts += q * m;
		lavg += q * m;
		for (auto &t : threads)t.join();
		threads.clear();
	}
}

void Controller::calcMaxStrange() {
	for (int i = 0; i < p; ++i) {
		for (int j = 0; j < m; ++j) {
			maxStrange->setAt(i, j, max(0.0, strangeD->getAt(i, j) + strangeF->getAt(i, j) - 1));
			tacts++;
		}
	}
}

Matrix Controller::run() {
	Matrix res(p, q);
	getTripleMatrixProduct();
	calcStrange();
	calcMaxStrange();
	for (int i = 0; i < p; ++i) {
		for (int j = 0; j < m; ++j) {
			double result = 0;
			result += strangeF->getAt(i, j)*(3 * g->getAt(i, j) - 2)*g->getAt(i, j);
			result += (strangeD->getAt(i, j) + (4 * (maxStrange->getAt(i, j)) - 3 * strangeD->getAt(i, j))*g->getAt(i, j))
				* (1 - g->getAt(i, j));
			res.setAt(i, j, result);
			tacts += 11;
		}
	}
	output();
	cout << "\n Matrix C\n";
	printMatrix(res);
	cout << "\n Tacts :" << tacts << "\n";
	cout << "\n Lavg :" << lavg << "\n";
	return res;
}

void Controller::printMatrix(const Matrix &matrix) {
	for (int i = 0; i < matrix.ySize; ++i) {
		for (int j = 0; j < matrix.xSize; ++j) {
			printf("%5.3f\t", matrix.getAt(i, j));
		}
		std::cout << "\n";
	}
}

void Controller::output() {
	cout << "p :" << p << ", q :" << q << ", m :" << m << ", Threads :" << threadCount << "\n";
	cout << "\n Matrix A\n";
	printMatrix(*a);
	cout << "\n Matrix B\n";
	printMatrix(*b);
	cout << "\n Matrix E\n";
	printMatrix(*e);
	cout << "\n Matrix G\n";
	printMatrix(*g);
}


