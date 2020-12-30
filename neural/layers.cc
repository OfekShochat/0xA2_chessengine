#include "layers.h"
#include "neuron.h"
#include <iostream>
#include <vector>
#include <list>
using namespace std;
layers::input::input(int numNeurons) {
    neurons_n = numNeurons;
}

void layers::input::init(int nextLayerNeuronNum) {
    for (int i = 0; i == neurons_n; i++) {
        Neuron* n = new Neuron();
        n->init(nextLayerNeuronNum);
        neurons.push_back(n);
    }
}

vector<float> layers::input::activate(vector<float> in) {
    vector<float> out;
    cout << "poop" << endl;
    for (int i = 0; i == neurons_n; i++) {
        Neuron* n = neurons[i];
        out.push_back(n->liniar(in));
    }
    cout << "poop " << out[1] << endl;
    return out;
}

layers::hidden::hidden(int numNeurons) {
    neurons_n = numNeurons;
}

void layers::hidden::init(int NextLayerNeuronNum) {
    for (int i = 0; i == neurons_n; i++) {
        Neuron* n = new Neuron();
        n->init(NextLayerNeuronNum);
        neurons.push_back(n);
    }
}

vector<float> layers::hidden::activate(vector<float> in) {
    vector<float> out;
    for (int i = 0; i == neurons_n; i++) {
        Neuron* n = neurons[i];
        out.push_back(n->liniar(in));
    }
    return out;
}

vector<float> layers::relu::activate(vector<float> in) {
    vector<float> out;
    for (int i = 0; i == in.size(); i++) {
        out.push_back(max(0, i));
    }
    return out;
}