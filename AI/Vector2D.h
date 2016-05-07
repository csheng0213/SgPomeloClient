#ifndef _VECTOR2D_H_
#define _VECTOR2D_H_

#include <cmath>
#include <limits>

const float PI = 3.1415926535897932384626433832795f;

class Vector2D
{
public:
	Vector2D(const float x = 0.0f, const float y = 0.0f)
		: x(x), y(y)
	{}

public: // useful interface
	inline void setOrigin()
	{
		x = y = 0.0f;
	}

	// length
	const float len() const
	{
		return std::sqrt(x * x + y * y);
	}

	// square of length
	const float lenSq() const
	{
		return x * x + y * y;
	}

	// distance
	const float dist(const Vector2D &v) const
	{
		return std::sqrt((x - v.x) * (x - v.x) + (y - v.y) * (y - v.y));
	}
	
	const float distSq(const Vector2D &v) const
	{
		return (x - v.x) * (x - v.x) + (y - v.y) * (y - v.y);
	}

	// normalize self
	void norm()
	{
        auto len = this->len();

        if (len > std::numeric_limits<float>::epsilon())
        {
            this->x /= len;
            this->y /= len;
        }
	}

	// normalize
	const Vector2D getNorm() const
	{
        auto vec = *this;
        auto len = vec.len();

        if (len > std::numeric_limits<float>::epsilon())
        {
            vec.x /= len;
            vec.y /= len;
        }
        return vec;
	}

	void reverse()
	{
		x = -x;
		y = -y;
	}

	const Vector2D getReverse() const
	{
		return Vector2D(-x, -y);
	}

	void rotate(const float v)
	{
		float agl = getAngle() + v;
		float l = len();
		x = l * std::sin(agl);
		y = l * std::cos(agl);
	}

	const Vector2D getRotate(const float v)
	{
		float agl = getAngle() + v;
		float l = len();
		return Vector2D(l * std::sin(agl), l * std::cos(agl));
	}

	const float getAngle() const
	{
		return std::atan2(y, x);
	}
	
public:	// operators
	const bool operator == (const Vector2D &v) const
	{
		return x == v.x && y == v.y;
	}

	Vector2D & operator = (const Vector2D &v)
	{
		x = v.x;
		y = v.y;
		return *this;
	}

    const Vector2D operator - () const
    {
        return Vector2D(-x, -y);
    }
	
	const Vector2D operator + (const Vector2D &v) const
	{
		return Vector2D(x + v.x, y + v.y);
	}

	const Vector2D operator - (const Vector2D &v) const
	{
		return Vector2D(x - v.x, y - v.y);
	}

	// multiply with right number
	const Vector2D operator * (const float v) const
	{
		return Vector2D(x * v, y * v);
	}

	const float operator * (const Vector2D &v) const
	{
		return x * v.x + y * v.y;
	}

	const Vector2D operator / (const float v) const
	{
		return Vector2D(x / v, y / v);
	}

	Vector2D & operator += (const Vector2D &v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	Vector2D & operator -= (const Vector2D &v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	Vector2D & operator *= (const float v)
	{
		 x *= v;
		 y *= v;
		 return *this;
	}

	Vector2D & operator /= (const float v)
	{
		x /= v;
		y /= v;
		return *this;
	}

public:
	float x;
	float y;
};

// multiply with left number
inline const Vector2D operator * (const float f, const Vector2D &v)
{
	return Vector2D(f * v.x, f * v.y);
}

//typedef Vector2D Point2D;

#endif // _VECTOR2D_H_