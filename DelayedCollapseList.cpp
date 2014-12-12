#include "DelayedCollapseList.h"
#include <list>
#include <ncurses.h>

void ParticleList::add(sandPart sp) {

}

void ParticleList::erase(int t) {
}

sandPart& ParticleList::operator[](const int i) {
	
}


void ParticleList::clear() {

}

ParticleList::ParticleList() {
	len = 0;
	head = new Node;
	tail = new Node;
	head->next = tail;
	tail->next = head;
}

ParticleList::~ParticleList() {
	clear();
	delete head;
	delete tail;
}
