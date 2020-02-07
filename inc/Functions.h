#ifndef _KODO__FUNCTIONS_H_
#define _KODO__FUNCTIONS_H_
#include <functional>
#include <string>
#include <memory>
#include <cmath>
#include "Tools.h"

namespace kodo
{
class UndefinedFuncError
{
};
Vector ReLU(const Vector &x)
{
    Vector result(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        result[i] = x[i] >= 0 ? x[i] : 0;
    }
    return result;
}
Vector DReLU(const Vector &x)
{
    Vector result(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        result[i] = x[i] >= 0 ? 1 : 0;
    }
    return result;
}
Vector LeakyReLU(const Vector &x)
{
    Vector result(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        result[i] = x[i] >= 0 ? x[i] : 0.01 * x[i];
    }
    return result;
}
Vector DLeakyReLU(const Vector &x)
{
    Vector result(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        result[i] = x[i] >= 0 ? 1 : 0.01;
    }
    return result;
}
Vector Sigmoid(const Vector &x)
{
    Vector result(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        result[i] = 1 / (1 + std::exp(-x[i]));
    }
    return result;
}
Vector DSigmoid(const Vector &x)
{
    Vector result(x.size());
    for (size_t i = 0; i < x.size(); i++)
    {
        result[i] = (1 / (1 + std::exp(-x[i]))) * (1.0 - (1 / (1 + std::exp(-x[i]))));
    }
    return result;
}
Vector Softmax(const Vector &x)
{
    Vector result(x.size());
    double sum = 0;
    for (auto &&i : x)
    {
        sum += std::exp(i);
    }
    for (size_t i = 0; i < x.size(); i++)
    {
        result[i] = std::exp(x[i]) / sum;
    }
    return result;
}

struct ActFunc
{
    using Func = std::function<Vector(const Vector &)>;
    Func proto, differ;
    ActFunc(Func _p, Func _d) : proto(_p), differ(_d) {}
};

std::unique_ptr<ActFunc> CreateActFunc(const std::string &type)
{
    if (type == "relu" || type == "ReLU" || type == "RELU")
    {
        return std::make_unique<ActFunc>(ReLU, DReLU);
    }
    else if (type == "leakyrelu" || type == "LeakyReLU" || type == "LEAKYRELU")
    {
        return std::make_unique<ActFunc>(LeakyReLU, DLeakyReLU);
    }
    else if (type == "sigmoid" || type == "SIGMOID" || type == "Sigmoid")
    {
        return std::make_unique<ActFunc>(Sigmoid, DSigmoid);
    }
    else
        throw kodo::UndefinedFuncError();
}

} // namespace kodo

#endif