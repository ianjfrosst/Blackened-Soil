#pragma once

#include "SandExtras.h"
#include <iterator>

class ParticleList {
private:
    struct Node {
        Node *next;
        Node *prev;
        sandPart data;
    };

    int len;
    Node *head;
    Node *tail;

public:
    ParticleList();
    ~ParticleList();


    int size() const {return len;}
    bool empty() const {return len == 0;}

    void add(sandPart sp);
    void erase(int i);
    void clear();

    sandPart &operator[](int const i);
};