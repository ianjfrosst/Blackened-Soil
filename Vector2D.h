#pragma once

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>


// Angles are in radians
class Vector2D {
public:
    double x, y;

    Vector2D(double nX = 0.0, double nY = 0.0);
    Vector2D(sf::Vector2f v);

    void SetXY(double nX, double nY);
    void SetDM(double dir, double mag);

	void Normalize();

	Vector2D Normal();

    double GetDir();
    double GetMag();
	double GetSqrMag();

	bool CheckCollision(Vector2D a, Vector2D b, float d);

	float GetSegmentDist(Vector2D, Vector2D);

	double DotProduct(Vector2D b);
	double Project(Vector2D b);


    Vector2D& operator+=(const Vector2D& v);
    Vector2D& operator-=(const Vector2D& v);
    Vector2D& operator*=(double t);
    Vector2D& operator/=(double t);
    Vector2D& operator&=(const Vector2D& v);
    Vector2D operator-(void) const;
    Vector2D operator+(const Vector2D& v) const;
    Vector2D operator-(const Vector2D& v) const;
    Vector2D operator*(double t) const;
    Vector2D operator/(double t) const;
    bool operator==(const Vector2D& v) const;
    bool operator!=(const Vector2D& v) const;
    operator sf::Vector2f();
    Vector2D& Rotate(double angle);

    friend std::ostream& operator<<(std::ostream& os, const Vector2D& v);
    friend std::istream& operator>>(std::istream& os, Vector2D& v);
};

