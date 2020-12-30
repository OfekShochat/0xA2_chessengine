#include "net1.h"
#include <iostream>
#include <vector>

Net1::Net1() {
    in->init(1);
}

float Net1::forward(vector<float> data) {
    vector<float> out;
    
    out = in->activate(data);

    return tanhf(out[0]);
}