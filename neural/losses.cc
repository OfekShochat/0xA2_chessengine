#include "losses.h"
#include <iostream>
#include <algorithm>
#include <list> 
#include <cmath> 
using namespace std;

float losses::mse(float x, float y) {
    return powf((y-x), 2);
}