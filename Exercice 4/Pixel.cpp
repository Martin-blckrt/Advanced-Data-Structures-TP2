#include "Pixel.h"
#include <iostream>

using namespace std;

Pixel::Pixel() : red(0), green(0), blue(0) {}
Pixel::Pixel(double r, double g, double b) : red(r), green(g), blue(b) {}
Pixel::~Pixel(){}

double Pixel::getRed() const
{
	return red;
}
double Pixel::getGreen() const
{
	return green;
}
double Pixel::getBlue() const
{
	return blue;
}

void Pixel::setRed(double r)
{
	red = r;
}
void Pixel::setGreen(double g)
{
	green = g;
}
void Pixel::setBlue(double b)
{
	blue = b;
}

ostream& operator<<(ostream& out, const Pixel& pixel) // Surchage de l'opérateur d'output
{
	out << "(" << pixel.getRed() << "," << pixel.getGreen() << "," << pixel.getRed() << ")";
	return out;
}

bool Pixel::operator==(Pixel const& test) const // Surchage de l'opérateur d'égalité pour comparer les pixels
{
	if (red == test.getRed() && green == test.getGreen() && blue == test.getBlue())
	{
		return true;
	}
	return false;
}
