#include "stdafx.h"
#include ".\inc\Functions.h"
#include <cmath>

namespace kodo
{
    Vector ReLU(const Vector &x) {
        Vector result(x.size());
        for (size_t i = 0; i < x.size(); i++) {
            result[i] = x[i] >= 0 ? x[i] : 0;
        }
        return std::move(result);
    }
    Vector DReLU(const Vector &x) {
        Vector result(x.size());
        for (size_t i = 0; i < x.size(); i++) {
            result[i] = x[i] >= 0 ? 1 : 0;
        }
        return std::move(result);
    }
    Vector LeakyReLU(const Vector &x) {
        Vector result(x.size());
        for (size_t i = 0; i < x.size(); i++) {
            result[i] = x[i] >= 0 ? x[i] : 0.01 * x[i];
        }
        return std::move(result);
    }
    Vector DLeakyReLU(const Vector &x) {
        Vector result(x.size());
        for (size_t i = 0; i < x.size(); i++) {
            result[i] = x[i] >= 0 ? 1 : 0.01;
        }
        return std::move(result);
    }
    Vector Sigmoid(const Vector &x) {
        Vector result(x.size());
        for (size_t i = 0; i < x.size(); i++) {
            result[i] = 1.0 / (1.0 + std::exp(-x[i]));
        }
        return std::move(result);
    }
    Vector DSigmoid(const Vector &x) {
        Vector result(x.size());
        for (size_t i = 0; i < x.size(); i++) {
            result[i] = (1.0 / (1.0 + std::exp(-x[i]))) * (1.0 - (1.0 / (1.0 + std::exp(-x[i]))));
        }
        return std::move(result);
    }
    Vector Softmax(const Vector &x) {
        Vector result(x.size());
        double sum = 0;
        for (auto &&i : x) {
            sum += std::exp(i);
        }
        for (size_t i = 0; i < x.size(); i++) {
            result[i] = std::exp(x[i]) / sum;
        }
        return std::move(result);
    }
    Vector DSoftmax(const Vector &x) {
        return Vector(x.size(), 1);
    }
    Vector MSE(const Vector &p, const Vector &y) {
        Vector result(p.size());
        for (size_t i = 0; i < p.size(); i++) {
            result[i] = std::pow(p[i] - y[i], 2) / 2.0;
        }
        return std::move(result);
    }
    Vector DMSE(const Vector &p, const Vector &y) {
        Vector result(p.size());
        for (size_t i = 0; i < p.size(); i++) {
            result[i] = p[i] - y[i];
        }
        return std::move(result);
    }
    Vector Sigmoid_Crossentropy(const Vector &p, const Vector &y) {
        Vector result(p.size());
        for (size_t i = 0; i < p.size(); i++) {
            result[i] = -y[i] * std::log(p[i]) - (1 - y[i])*std::log(1 - p[i]);
        }
        return std::move(result);
    }
    Vector DSigmoid_Crossentropy(const Vector &p, const Vector &y) {
        Vector result(p.size());
        for (size_t i = 0; i < p.size(); i++) {
            result[i] = -y[i] / p[i] + (1 - y[i]) / (1 - p[i]);
        }
        return std::move(result);
    }
    Vector Softmax_Crossentropy(const Vector &p, const Vector &y) {
        Vector result(p.size());
        for (size_t i = 0; i < p.size(); i++) {
            result[i] = -y[i] * std::log(p[i]);
        }
        return std::move(result);
    }
    Vector DSoftmax_Crossentropy(const Vector &p, const Vector &y) {
        Vector result(p.size());
        for (size_t i = 0; i < p.size(); i++) {
            result[i] = p[i] - y[i];
        }
        return std::move(result);
    }
    std::unique_ptr<ActFunc> CreateActFunc(const std::string &type) {
        if (type == "relu" || type == "ReLU" || type == "RELU") {
            return std::make_unique<ActFunc>(ReLU, DReLU);
        }
        else if (type == "leakyrelu" || type == "LeakyReLU" || type == "LEAKYRELU") {
            return std::make_unique<ActFunc>(LeakyReLU, DLeakyReLU);
        }
        else if (type == "sigmoid" || type == "SIGMOID" || type == "Sigmoid") {
            return std::make_unique<ActFunc>(Sigmoid, DSigmoid);
        }
        else if (type == "softmax" || type == "SOFTMAX" || type == "Softmax") {
            return std::make_unique<ActFunc>(Softmax, DSoftmax);
        }
        else
            throw std::logic_error(type + "is not an ActFunc.");
    }

    std::unique_ptr<LossFunc> CreateLossFunc(const std::string &type) {
        if (type == "mse" || type == "MSE") {
            return std::make_unique<LossFunc>(MSE, DMSE);
        }
        else if (type == "binary_crossentropy") {
            return std::make_unique<LossFunc>(Sigmoid_Crossentropy, DSigmoid_Crossentropy);
        }
        else if (type == "categorical_crossentropy") {
            return std::make_unique<LossFunc>(Softmax_Crossentropy, DSoftmax_Crossentropy);
        }
        else throw std::logic_error(type + "is not an LossFunc.");
    }
}