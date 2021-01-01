#include "neural.h"
#include <iostream>
#include <vector>

class Net1 {
public:
    Net1();
    layers::hidden *hid1 = new layers::hidden(1);
    layers::relu *rel = new layers::relu();
    float forward(std::vector<float> data);
};