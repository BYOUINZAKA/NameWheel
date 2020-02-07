#ifndef _KODO__MLPNETWORK_H_
#define _KODO__MLPNETWORK_H_
//#define __DEBUGING__ true

/**
 *	MIT License
 *
 *	Copyright (c) 2020 Hata.Rotom https://github.com/RotomHata
 *
 *	Permission is hereby granted, free of charge, to any person obtaining a copy
 *	of this software and associated documentation files (the "Software"), to deal
 *	in the Software without restriction, including without limitation the rights
 *	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *	copies of the Software, and to permit persons to whom the Software is
 *	furnished to do so, subject to the following conditions:
 *
 *	The above copyright notice and this permission notice shall be included in all
 *	copies or substantial portions of the Software.
 *
 *	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *	SOFTWARE.
 *
 *	@file		MlpNetwork.h
 *	@brief		A header file of the MlpNetwork class,
 *				contains its declaration and definition.
 *	@author		Hata
 *	@version	beta-1.0
 *	@date		2020-02-03
 */

#ifdef __DEBUGING__
#include <iostream>
#endif

#include "inc\Functions.h"
#include "inc\Tools.h"

namespace kodo
{
	/**
	 *	@brief		Multi-Layer Perceptron Network class.
	 *				A class that encapsulates a Multi-Layer Perceptron,
	 *				it can forward and backward.
	 *	@example	You can create a 24-input and 3-output ANN has 4 hidden layers like this:
	 *	@code
	 *				kodo::MlpNetwork<24,
	 *					3,
	 *					kodo::Hidden<12, 24, 48, 96>>
	 *					mlp("LeakyReLU", "Softmax", "crossentropy");
	 *	@endcode
	 */
	template <unsigned InputCount, unsigned OutputCount, class T>
	class MlpNetwork {
	public:
		friend class NetworkViewer;
		using HiddensType = T;
		using ActFunction = std::unique_ptr<ActFunc>;
		using LossFunction = std::unique_ptr<LossFunc>;
		constexpr static const size_t HiddenCount = HiddensType::HIDDENS_QTY;
		/**
		 * @brief		Constructor.
		 * @param		learningRate	Learningrate of the network.
		 * @param		middle_type		Type of hidden layers activation functions
		 * @param		out_type		Type of out layer activation functions
		 * @param		loss_type		Set up the target function and the loss function
		 * @exception	NotFunctionError
		 */
		MlpNetwork(double		learningRate,
				   const char*	middle_type,
				   const char*	out_type,
				   const char*	loss_type);

	private:
		double				_learningRate;
		HiddensType			_hidden;
		ActFunction			_middleFunc;
		ActFunction			_outputFunc;
		LossFunction		_lossFunc;
		std::vector<Vector> _in;
		std::vector<Vector> _out;
		std::vector<Vector> _bias;
		std::vector<Matrix> _omega;
		std::vector<Vector> _loss;

	public:
		const Vector&	Forward(const Vector &input);
		void			Backward(const Vector &real);
		std::ostream&	Print(std::ostream &os);
		double t(const Vector& real) {
			auto r = _lossFunc->proto(_out.back(), real);
			double sum = 0.0;
			for (auto &&i : r) {
				sum += i;
			}
			sum /= (double)r.size();
			return sum;
		}
	};

} // namespace kodo

template <unsigned InputCount, unsigned OutputCount, class T>
kodo::MlpNetwork<InputCount, OutputCount, T>::MlpNetwork(double learningRate,
														 const char *middle_type,
														 const char *out_type,
														 const char *loss_type):
	_hidden(),
	_in(HiddenCount + 2),
	_out(HiddenCount + 2),
	_bias(HiddenCount + 1),
	_omega(HiddenCount + 1),
	_loss(HiddenCount + 1),
	_learningRate(learningRate) {

	srand(time(nullptr));
	_middleFunc = std::move(CreateActFunc(std::string(middle_type)));
	_outputFunc = std::move(CreateActFunc(std::string(out_type)));
	_lossFunc = std::move(CreateLossFunc(std::string(loss_type)));
	_in.front().resize(InputCount);
	_in.back().resize(OutputCount);
	_out.front().resize(InputCount);
	_out.back().resize(OutputCount);
	for (size_t i = 1; i <= HiddenCount; i++) {
		_in[i].resize(_hidden[i - 1]);
		_out[i].resize(_hidden[i - 1]);
		if (i != HiddenCount) {
			_omega[i] = std::move(MakeNoiseMatrix(_hidden[i], _hidden[i - 1], 0.5, -0.5));
			_bias[i] = std::move(MakeNoiseVector(_hidden[i], 0.5, -0.5));
		}
		else {
			_omega[i] = std::move(MakeNoiseMatrix(OutputCount, _hidden[i - 1], 0.5, -0.5));
			_bias[i] = std::move(MakeNoiseVector(OutputCount, 0.5, -0.5));
		}
	}
	_omega.front() = std::move(MakeNoiseMatrix(_in[1].size(), InputCount, 0.5, -0.5));
	_bias.front().resize(_in[1].size(), 0.0);
}

template <unsigned InputCount, unsigned OutputCount, class T>
std::ostream &kodo::MlpNetwork<InputCount, OutputCount, T>::Print(std::ostream &os) {
	os << _in.front();
	for (auto &&i : _omega) {
		os << i;
	}
	os << _out.back();
	return os;
}

template <unsigned InputCount, unsigned OutputCount, class T>
const kodo::Vector &kodo::MlpNetwork<InputCount, OutputCount, T>::Forward(const kodo::Vector &input) {
	_in.front() = input;
	_out.front() = input;
	for (size_t i = 1; i <= HiddenCount; i++) {
		_in[i] = _omega[i - 1] * _out[i - 1] + _bias[i - 1];
		_out[i] = _middleFunc->proto(_in[i]);
	}
	_in.back() = _omega.back() * (*(_out.end() - 2)) + _bias.back();
	_out.back() = _outputFunc->proto(_in.back());
	return _out.back();
}

template <unsigned InputCount, unsigned OutputCount, class T>
void kodo::MlpNetwork<InputCount, OutputCount, T>::Backward(const kodo::Vector &real) {
	_loss.back() = _lossFunc->differ(_out.back(), real);
	_loss.back() &= _outputFunc->differ(_in.back());

	for (size_t i = HiddenCount; i >= 1; i--) {
		_loss[i - 1] = (~_omega[i])*_loss[i];
		_loss[i - 1] &= _middleFunc->differ(_in[i]);
	}

	for (size_t l = 0; l <= HiddenCount; l++) {
		for (size_t j = 0; j < _loss[l].size(); j++) {
			for (size_t k = 0; k < _out[l].size(); k++) {
				_omega[l][j][k] -= _learningRate * _out[l][k] * _loss[l][j];
			}
			_bias[l][j] -= _learningRate*_loss[l][j];
		}
	}
}

#endif