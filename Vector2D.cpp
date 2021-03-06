#include "Vector2D.h"

Vector2D::Vector2D(double nX, double nY) {
    SetXY(nX, nY);
}

Vector2D::Vector2D(sf::Vector2f v) {
    SetXY(v.x, v.y);
}

void Vector2D::SetXY(double nX = 0.0, double nY = 0.0) {
    x = nX;
    y = nY;
}

void Vector2D::SetDM(double dir, double mag) {
    x = cos(dir) * mag;
    y = sin(dir) * mag;
}

double Vector2D::GetDir() {
    return atan2(y, x);
}

double Vector2D::GetMag() {
    return hypot(x, y);
}

double Vector2D::GetSqrMag() {
	return (x*x) + (y*y);
}

bool Vector2D::CheckCollision(Vector2D a, Vector2D b, float d) {
	return abs(GetSegmentDist(a,b)) < d;
}

float Vector2D::GetSegmentDist(Vector2D v, Vector2D w) {
	// Return minimum distance between line segment vw and point p
	float l2 = (v-w).GetSqrMag();  // i.e. |w-v|^2 -  avoid a sqrt
	if (l2 == 0.0) return ((*this)-v).GetMag();   // v == w case
	// Consider the line extending the segment, parameterized as v + t (w - v).
	// We find projection of point p onto the line. 
	// It falls where t = [(p-v) . (w-v)] / |w-v|^2
	float t = ((*this)-v).DotProduct(w-v) / l2;
	if (t < 0.0) return ((*this)-v).GetMag();       // Beyond the 'v' end of the segment
	else if (t > 1.0) return ((*this)-w).GetMag();  // Beyond the 'w' end of the segment
	Vector2D projection = v + (w - v)*t;  // Projection falls on the segment
	return ((*this)-projection).GetMag();
}


// Returns the dot product of this and b.
double Vector2D::DotProduct(Vector2D b) {
	return (x*b.x)+(y*b.y);
}

// Return the magnitude of b projected onto this.
double Vector2D::Project(Vector2D b) {
	return DotProduct(b)/GetMag();
}

void Vector2D::Normalize() {
	double mag = GetMag();
	x = x/mag;
	y = y/mag;
}

Vector2D Vector2D::Normal() {
	return Vector2D(y,x)/GetMag();
}


Vector2D& Vector2D::operator+=(const Vector2D& v) {
    x += v.x;
    y += v.y;
    return (*this);
}

Vector2D& Vector2D::operator-=(const Vector2D& v) {
    x -= v.x;
    y -= v.y;
    return (*this);
}

Vector2D& Vector2D::operator*=(double t) {
    x *= t;
    y *= t;
    return (*this);
}

Vector2D& Vector2D::operator/=(double t) {
    double f = 1.0 / t;
    x *= f;
    y *= f;
    return (*this);
}

Vector2D& Vector2D::operator&=(const Vector2D& v) {
    x *= v.x;
    y *= v.y;
    return (*this);
}

Vector2D Vector2D::operator-(void) const {
    return (Vector2D(-x, -y));
}

Vector2D Vector2D::operator+(const Vector2D& v) const {
    return (Vector2D(x + v.x, y + v.y));
}

Vector2D Vector2D::operator-(const Vector2D& v) const {
    return (Vector2D(x - v.x, y - v.y));
}

Vector2D Vector2D::operator*(double t) const {
    return (Vector2D(x * t, y * t));
}

Vector2D Vector2D::operator/(double t) const {
    double f = 1.0 / t;
    return (Vector2D(x * f, y * f));
}

bool Vector2D::operator==(const Vector2D& v) const {
    return ((x == v.x) && (y == v.y));
}

bool Vector2D::operator!=(const Vector2D& v) const {
    return ((x != v.x) || (y != v.y));
}

Vector2D::operator sf::Vector2f() {
    return sf::Vector2f(x, y);
}

Vector2D& Vector2D::Rotate(double angle) {
    double s = sin(angle);
    double c = cos(angle);

    double nx = c * x - s * y;
    double ny = s * x + c * y;

    x = nx;
    y = ny;

    return (*this);
}

std::ostream& operator<<(std::ostream& os, const Vector2D& v) {
    os << "(" << v.x << ", " << v.y << ")";
    return os;
}

std::istream& operator>>(std::istream& is, Vector2D& v) {
    double tX, tY;
    is >> tX >> tY;
    v.SetXY(tX, tY);
    return is;
}