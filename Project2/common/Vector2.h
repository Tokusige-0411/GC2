#pragma once

template<class T> class Vector2Template
{
public:
	Vector2Template();
	Vector2Template(T x, T y);
	~Vector2Template();

	// ‘ã“ü‰‰Zq
	Vector2Template& operator=(const Vector2Template& vec);

	// “Yš‰‰Zq
	T& operator[](int no);

	// ’P€‰‰Zq
	Vector2Template& operator+=(const Vector2Template& vec);
	Vector2Template& operator-=(const Vector2Template& vec);
	Vector2Template& operator*=(T k);
	Vector2Template& operator/=(T k);
	Vector2Template operator+()const;
	Vector2Template operator-()const;

	Vector2Template& operator+=(T k);
	Vector2Template& operator-=(T k);

	// ”äŠr‰‰Zq
	bool operator== (const Vector2Template& vec)const;
	bool operator!= (const Vector2Template& vec)const;
	bool operator<= (const Vector2Template& vec)const;
	bool operator< (const Vector2Template& vec)const;

	T x;
	T y;
};

// Vector2 + int
template<class T>
Vector2Template<T> operator+(const Vector2Template<T>& u, const T v);

// Vector2 - int
template<class T>
Vector2Template<T> operator-(const Vector2Template<T>& u, const T v);

// Vector2 * int
template<class T>
Vector2Template<T> operator*(const Vector2Template<T>& u, const T v);

// int * Vector2
template<class T>
Vector2Template<T> operator*(const int& u, const Vector2Template<T>& v);

// Vector2 / int
template<class T>
Vector2Template<T> operator/(const Vector2Template<T>& u, const T v);

// Vector2 % int
template<class T>
Vector2Template<T> operator%(const Vector2Template<T>& u, const T v);

// Vector2 + Vector2
template<class T>
Vector2Template<T> operator+(const Vector2Template<T>& u, const Vector2Template<T>& v);

// Vector2 - Vector2
template<class T>
Vector2Template<T> operator-(const Vector2Template<T>& u, const Vector2Template<T>& v);

// Vector2 / Vector2
template<class T>
Vector2Template<T> operator/(const Vector2Template<T>& u, const Vector2Template<T>& v);

using Vector2 = Vector2Template<int>;
using Vector2Dbl = Vector2Template<double>;
using Vector2Flt = Vector2Template<float>;

#include "./detail/Vector2.h"
