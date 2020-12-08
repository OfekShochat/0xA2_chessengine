#pragma once
#include "neural/tensorflow/Model.h"
#include "neural/tensorflow/Tensor.h"
#include <iostream>
#include <list>
#include <string>

using namespace std;

class Net {
public:
	Net(string path);

	float predict(string fen);
private:
	Model model;

	list<float> make(string fen);
};