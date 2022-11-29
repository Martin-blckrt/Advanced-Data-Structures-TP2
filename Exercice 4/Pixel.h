#pragma once

#include <iostream>
using namespace std;


class Pixel
{
public:
	Pixel();
	Pixel(double, double, double);
	~Pixel();

	// Getters
	double getRed() const;
	double getGreen() const;
	double getBlue() const;

	// Setters
	void setRed(double);
	void setGreen(double);
	void setBlue(double);

	// Surcharge d'opérateurs
	friend ostream& operator<<(ostream& out, const Pixel& pixel);
	bool operator==(Pixel const& test) const;

private:
	double red;
	double green;
	double blue;
};
