#include "VECTOR2.h"

VECTOR2::VECTOR2()
{
	x = 0;
	y = 0;
}

VECTOR2::VECTOR2(int x, int y)
{
	this->x = x;
	this->y = y;
}

VECTOR2::~VECTOR2()
{
}

VECTOR2 & VECTOR2::operator=(const VECTOR2 & vec)
{
	this->x = vec.x;
	this->y = vec.y;
	return (*this);		// 自分自身のポインタを参照で返す this-> と *this は同じ
}

int & VECTOR2::operator[](int i)
{
	if (i == 0)
	{
		return x;
	}
	else if (i == 1)
	{
		return y;
	}
	else
	{
		return x;
	}

}

bool VECTOR2::operator==(const VECTOR2 & vec) const			// 最後にconstをつける理由→"読み込み専用"で使うときに必要!
{
	return ((this->x == vec.x) && (this->y == vec.y));
}

bool VECTOR2::operator!=(const VECTOR2 & vec) const			
{
	return !((this->x == vec.x) && (this->y == vec.y));		// もともと真で返しているものの逆にすればいいだけ
															// return ((this->x != vec.x) || (this->y != vec.y));	// 2000パターンcheckするのは手間 ~(補数)は向いてない
}

bool VECTOR2::operator>=(const VECTOR2 & vec) const
{
	return ((this->x >= vec.x) && (this->y >= vec.y));		
}

bool VECTOR2::operator<(const VECTOR2 & vec)  const
{
	return ((this->x < vec.x) && (this->y < vec.y));		
}

VECTOR2 & VECTOR2::operator+=(const VECTOR2 & vec)
{
	(this->x += vec.x);
	(this->y += vec.y);
	return (*this);
}

VECTOR2 & VECTOR2::operator-=(const VECTOR2 & vec)
{
	(this->x -= vec.x);
	(this->y -= vec.y);
	return (*this);
}

VECTOR2 & VECTOR2::operator*=(int k)
{
	(this->x *= k);
	(this->y *= k);
	return (*this);
}

VECTOR2 & VECTOR2::operator/=(int k)
{
	(this->x /= k);
	(this->y /= k);
	return (*this);
}

VECTOR2 VECTOR2::operator+() const
{
	return (*this);		
}

VECTOR2 VECTOR2::operator-() const
{
	return VECTOR2(-this->x, -this->y);
}

// ベクトルの演算
// VECTOR2 + int 
VECTOR2 operator+(const VECTOR2 & u, int v)
{
	VECTOR2 vec;
	vec.x = u.x + v;
	vec.y = u.y + v;
	return vec;
}

// VECTOR2 - int 
VECTOR2 operator-(const VECTOR2 & u, int v)
{
	VECTOR2 vec;
	vec.x = u.x - v;
	vec.y = u.y - v;
	return vec;
}

// VECTOR2 + VECTOR2
VECTOR2 operator+(const VECTOR2 & u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u.x + v.x;
	vec.y = u.y + v.y;
	return vec;
}

// VECTOR2 - VECTOR2
VECTOR2 operator-(const VECTOR2 & u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u.x - v.x;
	vec.y = u.y - v.y;
	return vec;
}

// VECTOR2 * VECTOR2
VECTOR2 operator*(const VECTOR2 & u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u.x * v.x;
	vec.y = u.y * v.y;
	return vec;
}

// VECTOR2 * int
VECTOR2 operator*(const VECTOR2 & u, int v)
{
	VECTOR2 vec;
	vec.x = u.x * v;
	vec.y = u.y * v;
	return vec;
}

// int * VECTOR2
VECTOR2 operator*(int v, const VECTOR2 & u)
{
	VECTOR2 vec;
	vec.x = v * u.x;
	vec.y = v * u.y;
	return vec;
}

// VECTOR2 / int
VECTOR2 operator/(const VECTOR2 & u, int v)
{
	VECTOR2 vec;
	vec.x = u.x / v;
	vec.y = u.y / v;
	return vec;
}

// VECTOR2 % int
VECTOR2 operator%(const VECTOR2 & u, int v)
{
	VECTOR2 vec;
	vec.x = u.x % v;
	vec.y = u.y % v;
	return vec;
}

// VECTOR2 / VECTOR2
VECTOR2 operator/(const VECTOR2 & u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u.x / v.x;
	vec.y = u.y / v.y;
	return vec;
}

// VECTOR2 % VECTOR2
VECTOR2 operator%(const VECTOR2 & u, const VECTOR2 & v)
{
	VECTOR2 vec;
	vec.x = u.x % v.x;
	vec.y = u.y % v.y;
	return vec;
}
