#include "neural.h"
#include "net1.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <cstdio>

using namespace std;
int main() {
    Net1* net = new Net1();
    vector<float> in{3.0};
    float d = net->forward(in);
    cout << d << endl;
}