#include "MlpNetwork.h"
#include <iostream>
using namespace kodo;

int main()
{
    MlpNetwork<2,
               3,
               Hidden<1, 2, 3>>
        mlp("ReLU", "Sigmoid");
        std::cout <<"h¹þ¹þ";
}