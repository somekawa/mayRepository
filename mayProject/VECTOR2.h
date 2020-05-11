#pragma once
class VECTOR2
{
public:
	VECTOR2();
	VECTOR2(int x, int y);

	~VECTOR2();

	int x;
	int y;

	//������Z�q
	VECTOR2& operator = (const VECTOR2& vec);

	//�Y�������Z�q
	int& operator[](int i);
	//��r���Z�q
	bool operator==(const VECTOR2& vec) const;
	bool operator!=(const VECTOR2& vec) const;
	bool operator>=(const VECTOR2& vec) const;
	bool operator<(const VECTOR2& vec)  const;

	//�P�����Z�q
	VECTOR2& operator+=(const VECTOR2& vec);
	VECTOR2& operator-=(const VECTOR2& vec);
	VECTOR2& operator*=(int k);
	VECTOR2& operator/=(int k);
	VECTOR2 operator+()const;
	VECTOR2 operator-()const;
};

//�x�N�g���ƃx�N�g���̉��Z
//Vector2+Vector2
VECTOR2 operator+(const VECTOR2& u, const VECTOR2& v);
//Vector2-Vector2
VECTOR2 operator-(const VECTOR2& u, const VECTOR2& v);
//Vector2*Vector2
VECTOR2 operator*(const VECTOR2& u, const VECTOR2& v);
//int*Vector2
VECTOR2 operator*(int v, const  VECTOR2& u);
//Vector2*int
VECTOR2 operator*(const VECTOR2& u, int v);
//Vector2+int
VECTOR2 operator+(const VECTOR2& u, int v);
//Vector2/int
VECTOR2 operator/(const VECTOR2& u, int v);
//Vector2%int
VECTOR2 operator%(const VECTOR2& u, int v);
//Vector2/Vector2
VECTOR2 operator/(const VECTOR2& u, const VECTOR2& v);
//VECTOR2%VECTOR2
VECTOR2 operator%(const VECTOR2& u, const VECTOR2& v);

