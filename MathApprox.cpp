#include <cmath>

// Arrays of values that would be expensive to calculate on the fly, and whose precision is not of significance.

// When given the X coordinate of a point on a unit circle, gives the y.
// Element 100 is the center of the circle, and is 1.
// Elements 0 and 200 are the edges, and are 0.
double root2minusN[201];

// Gives the absolute Y-coordinate of a point on a unit circle with x coordinate x/maxX + 1;
// maxX represents the diameter, not the radius.
double getUCYofX(int x, int maxX) {
	return root2minusN[x/maxX * 200];
}

void calcApprox() {
	double sqrt2 = sqrt(2);
	for (int i = 0; i < 201; i++) {
		root2minusN[i] = sqrt(((i-100)/100)*((i-100)/100) - 1); 
	}
}