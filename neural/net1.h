#include "neural.h"
#include <iostream>
#include <vector>

class Net1 {
public:
    Net1();
    layers::input *in = new layers::input(1);
    layers::relu *rel = new layers::relu();
    float forward(std::vector<float> data);
};