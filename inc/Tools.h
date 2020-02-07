#ifndef _KODO__TOOLS_H_
#define _KODO__TOOLS_H_
#include <vector>
#include <ostream>
#include <iomanip>
#include <random>
#include <cstdlib>

namespace kodo
{
class MatrixMultiplyError
{
};
class MatrixAddError
{
};
using Matrix = std::vector<std::vector<double>>;
using Vector = std::vector<double>;

Matrix inline MakeMateix(std::size_t length,
                         std::size_t width,
                         double value = 0.0)
{
    return Matrix(length, Vector(width, value));
}

Matrix MakeNoiseMatrix(std::size_t length,
                       std::size_t width,
                       double max = 1.0,
                       double min = 0.0)
{
    Matrix result(std::move(MakeMateix(length, width)));
    std::mt19937 gen(rand());
    std::uniform_real_distribution<> dis(min, max);
    for (auto &&i : result)
    {
        for (auto &&j : i)
        {
            j = dis(gen);
        }
    }
    return result;
}

Vector MakeNoiseVector(std::size_t length,
                       double max = 1.0,
                       double min = 0.0)
{
    Vector result(length);
    std::mt19937 gen(time(nullptr));
    std::uniform_real_distribution<> dis(min, max);
    for (auto &&i : result)
    {
        i = dis(gen);
    }
    return result;
}

Vector &Multi(Vector *result, const kodo::Matrix &lhs, const kodo::Vector &rhs);

} // namespace kodo

kodo::Matrix operator*(const kodo::Matrix &lhs, const kodo::Matrix &rhs);
kodo::Vector operator*(const kodo::Matrix &lhs, const kodo::Vector &rhs);
kodo::Vector operator*(const kodo::Vector &lhs, const kodo::Vector &rhs);
kodo::Vector operator+(const kodo::Vector &lhs, const kodo::Vector &rhs);

std::ostream &operator<<(std::ostream &os, const kodo::Matrix &matrix);
std::ostream &operator<<(std::ostream &os, const kodo::Vector &vector);

namespace kodo
{
template <unsigned InputCount, unsigned OutputCount, class T>
class MlpNetwork;

template <unsigned N, class T>
constexpr static void Spread(T (&hiddens)[N], T arg)
{
    hiddens[N - 1] = arg;
}

template <unsigned N, class T, class... Args>
constexpr static void Spread(T (&hiddens)[N], T arg, Args... args)
{
    hiddens[N - (sizeof...(args) + 1)] = arg;
    Spread(hiddens, args...);
}

constexpr static void Spread(int (&hiddens)[])
{
}

template <int... Args>
class Hidden
{
public:
    template <unsigned InputCount, unsigned OutputCount, class T>
    friend class MlpNetwork;
    using Type = Hidden<Args...>;
    constexpr static const unsigned HIDDENS_QTY = sizeof...(Args);

    int operator[](std::size_t index)
    {
        return hiddens[index];
    }

private:
    Hidden()
    {
        Spread(hiddens, Args...);
    }
    int hiddens[HIDDENS_QTY];
};
} // namespace kodo

std::ostream &operator<<(std::ostream &os, const kodo::Matrix &matrix)
{
    for (size_t i = 0; i < matrix.size(); i++)
    {
        if (i == 0 || i == matrix.size() - 1)
            os << "[ ";
        else
            os << "| ";
        for (auto &&j : matrix[i])
        {
            os << std::showpos << std::left << std::setprecision(2) << std::setw(8) << j << std::noshowpos;
        }
        if (i == 0 || i == matrix.size() - 1)
            os << "] ";
        else
            os << "|";
        os << std::endl;
    }
    return os;
}

std::ostream &operator<<(std::ostream &os, const kodo::Vector &vector)
{
    for (size_t i = 0; i < vector.size(); i++)
    {
        if (i == 0 || i == vector.size() - 1)
            os << "[ ";
        else
            os << "| ";
        os << std::showpos << std::left << std::setprecision(2) << std::setw(8) << vector[i] << std::noshowpos;
        if (i == 0 || i == vector.size() - 1)
            os << "]";
        else
            os << "|";
        os << std::endl;
    }
    return os;
}

kodo::Vector operator*(const kodo::Matrix &lhs, const kodo::Vector &rhs)
{
    kodo::Vector result(lhs.size());
    for (size_t i = 0; i < lhs.size(); i++)
    {
        if (lhs[i].size() != rhs.size())
            throw kodo::MatrixMultiplyError();
        double sum = 0;
        for (size_t j = 0; j < rhs.size(); j++)
        {
            sum += lhs[i][j] * rhs[j];
        }
        result[i] = sum;
    }
    return result;
}

kodo::Matrix operator*(const kodo::Matrix &lhs, const kodo::Matrix &rhs)
{
    kodo::Matrix result(lhs.size(), kodo::Vector(rhs[0].size(), 0.0));
    for (size_t n = 0; n < rhs[0].size(); n++)
    {
        for (size_t i = 0; i < lhs.size(); i++)
        {
            /* code */
            if (lhs[i].size() != rhs.size())
                throw kodo::MatrixMultiplyError();
            for (size_t j = 0; j < rhs.size(); j++)
            {
                result[i][n] += lhs[i][j] * rhs[j][n];
            }
        }
    }
    return result;
}

kodo::Vector operator+(const kodo::Vector &lhs, const kodo::Vector &rhs)
{
    if (lhs.size() != rhs.size())
        throw kodo::MatrixAddError();
    kodo::Vector result(lhs.size());
    for (size_t i = 0; i < lhs.size(); i++)
    {
        result[i] = lhs[i] + rhs[i];
    }
    return result;
}

namespace kodo
{
Vector &Multi(Vector *result, const kodo::Matrix &lhs, const kodo::Vector &rhs)
{
    for (size_t i = 0; i < lhs.size(); i++)
    {
        if (lhs[i].size() != rhs.size())
            throw kodo::MatrixMultiplyError();
        double sum = 0.0;
        for (size_t j = 0; j < rhs.size(); j++)
        {
            sum += lhs[i][j] * rhs[j];
        }
        result->at(i) = sum;
    }
    return *result;
}
}; // namespace kodo

#endif