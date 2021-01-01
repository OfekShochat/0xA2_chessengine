#include "net1.h"
#include <iostream>
#include <vector>

Net1::Net1() {
    hid1->init(1);
}

float Net1::forward(vector<float> data) {
    vector<float> out;
    out = hid1->activate(data);
    out = rel->activate(out);
    return tanhf(out[0]);
}