#pragma once
#include <iostream>
#include <list>
#include "losses.h"
#include <vector>
using namespace std;

class Neuron {
public:
    float liniar(vector<float> inputs);
    void init(int nextLayer_NeuronNum);
    std::vector<double> weights;
    float bias;
};