#include "stdafx.h"
#include ".\inc\Tools.h"

namespace kodo
{
	Matrix inline MakeMateix(std::size_t length,
							 std::size_t width,
							 double value) {
		return Matrix(length, Vector(width, value));
	}

	Matrix MakeNoiseMatrix(std::size_t length,
						   std::size_t width,
						   double max,
						   double min) {
		Matrix result(std::move(MakeMateix(length, width)));
		std::mt19937 gen(rand());
		std::uniform_real_distribution<> dis(min, max);
		for (auto &&i : result) {
			for (auto &&j : i) {
				j = dis(gen);
			}
		}
		return std::move(result);
	}

	Vector MakeNoiseVector(std::size_t length,
						   double max,
						   double min) {
		Vector result(length);
		std::mt19937 gen(time(nullptr));
		std::uniform_real_distribution<> dis(min, max);
		for (auto &&i : result) {
			i = dis(gen);
		}
		return std::move(result);
	}
}

std::ostream &operator<<(std::ostream &os, const kodo::Matrix &matrix) {
	if (matrix.size() == 1) {
		os << "£¨ ";
		for (auto &&j : matrix[0]) {
			os << std::showpos << std::left << std::setprecision(2) << std::setw(8) << j << std::noshowpos;
		}
		os << "£©" << std::endl;
		return os;
	}
	for (size_t i = 0; i < matrix.size(); i++) {
		if (i == 0)
			os << "¨q  ";
		else if (i == matrix.size() - 1)
			os << "¨t  ";
		else
			os << "©¦  ";
		for (auto &&j : matrix[i]) {
			os << std::showpos << std::left << std::setprecision(2) << std::setw(8) << j << std::noshowpos;
		}
		if (i == 0)
			os << "¨r";
		else if (i == matrix.size() - 1)
			os << "¨s";
		else
			os << "©¦";
		os << std::endl;
	}
	return os;
}

std::ostream &operator<<(std::ostream &os, const kodo::Vector &vector) {
	if (vector.size() == 1) {
		os << "£¨ ";
		os << std::showpos << std::left << std::setprecision(2)
			<< std::setw(8) << vector[0] << std::noshowpos;
		os << "£©" << std::endl;
		return os;
	}
	for (size_t i = 0; i < vector.size(); i++) {
		if (i == 0)
			os << "¨q  ";
		else if (i == vector.size() - 1)
			os << "¨t  ";
		else
			os << "©¦  ";
		os << std::showpos << std::left << std::setprecision(2) << std::setw(8) << vector[i] << std::noshowpos;
		if (i == 0)
			os << "¨r";
		else if (i == vector.size() - 1)
			os << "¨s";
		else
			os << "©¦";
		os << std::endl;
	}
	return os;
}

kodo::Vector operator*(const kodo::Matrix &lhs, const kodo::Vector &rhs) {
	kodo::Vector result(lhs.size());
	for (size_t i = 0; i < lhs.size(); i++) {
		if (lhs[i].size() != rhs.size())
			throw std::logic_error("Matrix Multiply Error.");
		double sum = 0;
		for (size_t j = 0; j < rhs.size(); j++) {
			sum += lhs[i][j] * rhs[j];
		}
		result[i] = sum;
	}
	return std::move(result);
}

kodo::Matrix operator*(const kodo::Matrix &lhs, const kodo::Matrix &rhs) {
	kodo::Matrix result(lhs.size(), kodo::Vector(rhs[0].size(), 0.0));
	for (size_t n = 0; n < rhs[0].size(); n++) {
		for (size_t i = 0; i < lhs.size(); i++) {
			if (lhs[i].size() != rhs.size())
				throw std::logic_error("Matrix Multiply Error.");
			for (size_t j = 0; j < rhs.size(); j++) {
				result[i][n] += lhs[i][j] * rhs[j][n];
			}
		}
	}
	return std::move(result);
}

kodo::Vector operator+(const kodo::Vector &lhs, const kodo::Vector &rhs) {
	if (lhs.size() != rhs.size())
		throw std::logic_error("Matrix Add Error.");
	kodo::Vector result(lhs.size());
	for (size_t i = 0; i < lhs.size(); i++) {
		result[i] = lhs[i] + rhs[i];
	}
	return std::move(result);
}

kodo::Vector& operator&=(kodo::Vector &lhs, const kodo::Vector &rhs) {
	for (size_t i = 0; i < lhs.size(); i++) {
		lhs[i] *= rhs[i];
	}
	return lhs;
}

kodo::Matrix operator~(const kodo::Matrix& matrix) {
	kodo::Matrix result(matrix[0].size(), kodo::Vector(matrix.size()));
	for (size_t i = 0; i < matrix.size(); i++) {
		for (size_t j = 0; j < matrix[i].size(); j++) {
			result[j][i] = matrix[i][j];
		}
	}
	return std::move(result);
}

namespace kodo
{
	Vector &Multi(Vector *result, const kodo::Matrix &lhs, const kodo::Vector &rhs) {
		for (size_t i = 0; i < lhs.size(); i++) {
			if (lhs[i].size() != rhs.size())
				throw std::logic_error("Matrix Multiply Error.");
			double sum = 0.0;
			for (size_t j = 0; j < rhs.size(); j++) {
				sum += lhs[i][j] * rhs[j];
			}
			result->at(i) = sum;
		}
		return *result;
	}
}; // namespace kodo