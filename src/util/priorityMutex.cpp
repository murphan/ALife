//
// Created by Emmet on 5/7/2023.
//
#include "priorityMutex.h"

PriorityMutex::PriorityMutex(): mutex(), lowPriorityMutex(), nextAccessMutex() {}
