#pragma once
class Vector2
{
public:
	Vector2();
	Vector2(int x, int y);
	~Vector2();

	// ‘ã“ü‰‰Zq
	Vector2& operator=(const Vector2& vec);

	// “Yš‰‰Zq
	int& operator[](int no);

	// ’P€‰‰Zq
	Vector2& operator+=(const Vector2& vec);
	Vector2& operator-=(const Vector2& vec);
	Vector2& operator*=(int k);
	Vector2& operator/=(int k);
	Vector2 operator+()const;
	Vector2 operator-()const;

	Vector2& operator+=(int k);
	Vector2& operator-=(int k);

	// ”äŠr‰‰Zq
	bool operator== (const Vector2& vec)const;
	bool operator!= (const Vector2& vec)const;
	bool operator<= (const Vector2& vec)const;
	bool operator< (const Vector2& vec)const;

	int x;
	int y;
};

// Vector2 + int
Vector2 operator+(const Vector2& u, const int v);

// Vector2 - int
Vector2 operator-(const Vector2& u, const int v);

// Vector2 * int
Vector2 operator*(const Vector2& u, const int v);

// int * Vector2
Vector2 operator*(const int& u, const Vector2& v);

// Vector2 / int
Vector2 operator/(const Vector2& u, const int v);

// Vector2 % int
Vector2 operator%(const Vector2& u, const int v);

// Vector2 + Vector2
Vector2 operator+(const Vector2& u, const Vector2& v);

// Vector2 - Vector2
Vector2 operator-(const Vector2& u, const Vector2& v);
