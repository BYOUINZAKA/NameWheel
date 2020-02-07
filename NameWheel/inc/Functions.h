#ifndef _KODO__FUNCTIONS_H_
#define _KODO__FUNCTIONS_H_
#include <functional>
#include <string>
#include <memory>
#include "Tools.h"

namespace kodo
{
    Vector ReLU(const Vector &x);
    Vector DReLU(const Vector &x);
    Vector LeakyReLU(const Vector &x);
    Vector DLeakyReLU(const Vector &x);
    Vector Sigmoid(const Vector &x);
    Vector DSigmoid(const Vector &x);
    Vector Softmax(const Vector &x);
    Vector DSoftmax(const Vector &x);

    struct ActFunc {
        using Func = std::function<Vector(const Vector& x)>;
        Func proto, differ;
        ActFunc(Func _p, Func _d): proto(_p), differ(_d) {}
    };
    std::unique_ptr<ActFunc> CreateActFunc(const std::string &type);

    Vector MSE(const Vector &p, const Vector &y);
    Vector DMSE(const Vector &p, const Vector &y);
    Vector Sigmoid_Crossentropy(const Vector &p, const Vector &y);
    Vector DSigmoid_Crossentropy(const Vector &p, const Vector &y);
    Vector Softmax_Crossentropy(const Vector &p, const Vector &y);
    Vector DSoftmax_Crossentropy(const Vector &p, const Vector &y);

    struct LossFunc {
        using Func = std::function<Vector(const Vector& p, const Vector& y)>;
        Func proto, differ;
        LossFunc(Func _p, Func _d): proto(_p), differ(_d) {}
    };
    std::unique_ptr<LossFunc> CreateLossFunc(const std::string &type);

} // namespace kodo

#endif