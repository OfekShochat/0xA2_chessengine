#include "Thread.h"
#include "node.h"
#include <iostream>
#include <string>
#include "evaluate.h"
void SearchThread::Start(Node* selected) {
    
    Node* AB = selected->select_AB();
    AB->expand();
    AB->update(evaluator::evaluate(selected->mBoard));
    selected->mParent->n += 1;
    selected->inUse = false;
}
