#include "DelayedCollapseList.h"

void ParticleList::add(sandPart sp) {
	contents[top+1] = contents[top];
	contents[top] = sp;
}

void ParticleList::erase(int t) {
	contents[t].alive = 0;
}

sandPart& ParticleList::operator[] (const int i) {
	return contents[i];
}

void ParticleList::clear() {
	int offset = 0;
	for (int i = 0; i < top; i ++) {
		if (!contents[i].alive) offset++;
		contents[i - offset] = contents[i];
	}
	top -= offset;
}

int ParticleList::size() {
	return top-1;
}