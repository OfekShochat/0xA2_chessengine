#pragma once
#include <iostream>
#include <vector>
#include <list>
#include "neuron.h"

namespace layers {
    class input;
    class hidden;
    class relu;
} // layers

class layers::input {
public:
    input(int numNeurons);
    int neurons_n;
    vector<Neuron*> neurons;
    void init(int nextLayerNeuronNum);
    float loss(float y);
    vector<float> activate(vector<float> in);
};

class layers::hidden {
public:
    hidden(int numNeurons);
    int neurons_n;
    vector<Neuron*> neurons;
    void init(int nextLayerNeuronNum);
    float loss(float y);
    vector<float> activate(vector<float> in);
};

class layers::relu {
public:
    vector<float> activate(vector<float> in);
};