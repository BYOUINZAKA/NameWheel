#ifndef _KODO__MLPNETWORK_H_
#define _KODO__MLPNETWORK_H_
//#define __DEBUGING__ true

#ifdef __DEBUGING__
#include <iostream>
#endif

#include "inc\Tools.h"
#include "inc\Functions.h"

namespace kodo
{
template <unsigned InputCount, unsigned OutputCount, class T>
class MlpNetwork
{
public:
    friend class NetworkViewer;
    using HiddensType = T;
    constexpr static const size_t HiddenCount = HiddensType::HIDDENS_QTY;

private:
    HiddensType _hidden;
    std::unique_ptr<ActFunc> _middleFunc, _outputFunc;
    std::vector<Vector> _in;
    std::vector<Vector> _out;
    std::vector<Vector> _bias;
    std::vector<Matrix> _omega;
    Vector _real;

public:
    MlpNetwork(const std::string &middle_type,
               const std::string &out_type);
    const Vector &Forward(const Vector &input);
    void Backward(const Vector &value);
    std::ostream &Print(std::ostream &os);
};

} // namespace kodo

template <unsigned InputCount, unsigned OutputCount, class T>
kodo::MlpNetwork<InputCount, OutputCount, T>::MlpNetwork(const std::string &middle_type,
                                                         const std::string &out_type) : _hidden(),
                                                                                        _in(HiddenCount + 2),
                                                                                        _out(HiddenCount + 2),
                                                                                        _bias(HiddenCount + 1),
                                                                                        _omega(HiddenCount + 1),
                                                                                        _real(OutputCount)
{
    srand(time(nullptr));
    _middleFunc = std::move(CreateActFunc(middle_type));
    _outputFunc = std::move(CreateActFunc(out_type));
    _in.front().resize(InputCount);
    _in.back().resize(OutputCount);
    _out.front().resize(InputCount);
    _out.back().resize(OutputCount);
    for (size_t i = 1; i <= HiddenCount; i++)
    {
        _in[i].resize(_hidden[i - 1]);
        _out[i].resize(_hidden[i - 1]);
        if (i != HiddenCount)
        {
            _omega[i] = std::move(MakeNoiseMatrix(_hidden[i], _hidden[i - 1], 0.5, -0.5));
            _bias[i] = std::move(MakeNoiseVector(_hidden[i], 0.5, -0.5));
        }
        else
        {
            _omega[i] = std::move(MakeNoiseMatrix(OutputCount, _hidden[i - 1], 0.5, -0.5));
            _bias[i] = std::move(MakeNoiseVector(OutputCount, 0.5, -0.5));
        }
    }
    _omega.front() = std::move(MakeNoiseMatrix(_in[1].size(), InputCount, 0.5, -0.5));
    _bias.front().resize(_in[1].size(), 0.0);
}

template <unsigned InputCount, unsigned OutputCount, class T>
std::ostream &kodo::MlpNetwork<InputCount, OutputCount, T>::Print(std::ostream &os)
{
    os << _in.front();
    for (auto &&i : _omega)
    {
        os << i;
    }
    os << _out.back();
    return os;
}

template <unsigned InputCount, unsigned OutputCount, class T>
const kodo::Vector &kodo::MlpNetwork<InputCount, OutputCount, T>::Forward(const kodo::Vector &input)
{
    _in.front() = input;
    _out.front() = input;
    for (size_t i = 1; i <= HiddenCount; i++)
    {
        _in[i] = _omega[i - 1] * _out[i - 1] + _bias[i - 1];
        _out[i] = std::move(_middleFunc->proto(_in[i]));
    }
    _in.back() = _omega.back() * (*(_out.end() - 2)) + _bias.back();
    _out.back() = std::move(_middleFunc->proto(_in.back()));
    return _out.back();
}

template <unsigned InputCount, unsigned OutputCount, class T>
void kodo::MlpNetwork<InputCount, OutputCount, T>::Backward(const kodo::Vector &value)
{
    _real = value;
}

#endif