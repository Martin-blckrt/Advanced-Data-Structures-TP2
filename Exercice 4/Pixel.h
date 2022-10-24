#pragma once

#include <iostream>
using namespace std;


class Pixel
{
public:
	Pixel();
	Pixel(double, double, double);
	~Pixel();

	double getRed() const;
	double getGreen() const;
	double getBlue() const;

	void setRed(double);
	void setGreen(double);
	void setBlue(double);

	friend ostream& operator<<(ostream& out, const Pixel& pixel);
private:
	double red;
	double green;
	double blue;
};
