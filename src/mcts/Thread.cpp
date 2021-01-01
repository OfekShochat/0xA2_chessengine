#include "Thread.h"
#include "node.h"
#include <iostream>
#include <string>

void SearchThread::Start(Node* selected) {
    Node* AB = selected->select_AB();
    selected->expand();
    selected->update(selected->AB_evaluate());
}