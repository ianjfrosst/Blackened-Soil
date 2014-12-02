#include "GameObject.h"

void GameObject::update() {
    vel += acc;
    pos += vel;

}