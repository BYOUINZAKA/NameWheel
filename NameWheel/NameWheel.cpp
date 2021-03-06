// NameWheel.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "inc\svpng.inc"
#include "MlpNetwork.h"
#include <iostream>
#include <cstdio>

#define W 628
#define H 628
unsigned char img[W * H * 3];

using namespace kodo;

int Part(const Vector &v) {
    if (v[1] >= std::sin(v[0])) {
        if (v[0] <= std::sin(v[1])) return 1;
        else return 0;
    }
    else {
        if (v[0] <= std::sin(v[1])) return 2;
        else return 3;
    }
}

std::size_t Max(const Vector& v) {
    std::size_t count = 0;
    double max = 0;
    for (size_t i = 0; i < v.size(); i++) {
        if (v[i] > max) {
            count = i;
            max = v[i];
        }
    }
    return count;
}

int main(int argv, char* argc[]) {

    std::cout << u8"我十点" << std::endl;
    /*
    MlpNetwork<2,
        4,
        HiddenGroup<2, 4>>
        mlp(0.000001, "leakyrelu", "softmax", "categorical_crossentropy");

    srand(time(0));
    std::mt19937 gen(rand());
    std::uniform_real_distribution<> dis(-std::_Pi, std::_Pi);
    Vector input(2), result(4, 0);

    for (size_t i = 0; i < 100000; i++) {
        input[0] = dis(gen);
        input[1] = dis(gen);
        int index = Part(input);
        result[index] = 1;
        mlp.Forward(input);
        mlp.Backward(result);
        if (i % 10000 == 0) {
            auto d = mlp.t(result);
            if (d <= 0.00001) break;
            else std::cout << d << std::endl;
        }
        result[index] = 0;
    }

    unsigned char* p = img;
    for (int y = 0; y < H; y++)
        for (int x = 0; x < W; x++, p += 3) {
            input[0] = (x - 314) / 100.0;
            input[1] = (y - 314) / 100.0;
            switch (Max(mlp.Forward(input))) {
            case 0:
                p[0] = p[1] = p[2] = 0;
                break;
            case 1:
                p[0] = p[1] = p[2] = 50;
                break;
            case 2:
                p[0] = p[1] = p[2] = 100;
                break;
            case 3:
                p[0] = p[1] = p[2] = 150;
                break;
            }
        }

    svpng(fopen("sin3.png", "wb"), W, H, img, 0);
    */

    system("pause");
    return 0;
}

