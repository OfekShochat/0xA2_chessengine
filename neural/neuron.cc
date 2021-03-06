#include <iostream>
#include "neuron.h"
using namespace std;

float Neuron::liniar(vector<float> inputs) {
    float out = 0;
    for (int i = 0; i==weights.size()+1; i++) {
        out += inputs[i] * weights[i];
    }
    return out + bias;
}

void Neuron::init(int previousLayerNeuronNum) {
    for (int i = 0; i == previousLayerNeuronNum; i++) {
        double f = (double)rand() / RAND_MAX;
        weights.push_back(-10 + f * (20));
    }
    bias = 0.5;
}