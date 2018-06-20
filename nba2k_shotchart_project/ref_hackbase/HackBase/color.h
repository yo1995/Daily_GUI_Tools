#ifndef __COLOR_H__
#define __COLOR_H__

#pragma warning (push)
#pragma warning (disable: 4244)
class Color {
public:
	int a, r, g, b;

	Color() { Color(0, 0, 0, 0); }

	Color(int a, int r, int g, int b) {
		this->a = a;
		this->r = r;
		this->g = g;
		this->b = b;
	}

	void SetColor(int a, int r, int g, int b) {
		this->a = a;
		this->r = r;
		this->g = g;
		this->b = b;
	}

	Color operator+(const Color& c) const
	{
		return Color(a + c.a, r + c.r, g + c.g, b + c.b);
	}
	Color& operator+=(const Color& c)
	{
		a += c.a;
		r += c.r;
		g += c.g;
		b += c.b;
		return *this;
	}
	Color operator*(float s) const
	{
		return Color(a * s, r * s, g * s, b * s);
	}
	Color& operator*=(float s)
	{
		a *= s;
		r *= s;
		g *= s;
		b *= s;
		return *this;
	}
	Color operator/(float s) const
	{
		return Color(a / s, r / s, g / s, b / s);
	}
	Color& operator/=(float s)
	{
		a /= s;
		r /= s;
		g /= s;
		b /= s;
		return *this;
	}
};
#pragma warning (pop)

#endif