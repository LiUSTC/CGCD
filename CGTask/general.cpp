#include "stdafx.h"

MyComplex::MyComplex():a(0),b(0){ };                                                            //复数的一些函数
MyComplex::MyComplex(const MyComplex & c) : a(c.a), b(c.b){ };
MyComplex & MyComplex::operator=(const MyComplex & c) {a=c.a; b=c.b; return *this;}

MyVector2::MyVector2(const double &_v1, const double &_v2) : x(_v1), y(_v2){};

MyComplex operator*(const MyComplex &_c1,const MyComplex &_c2)
{
	MyComplex result;
	result.a = _c1.a * _c2.a - _c1.b * _c2.b;
	result.b = _c1.b * _c2.a + _c1.a * _c2.b;
	return result;
}

bool operator==(const MyComplex &_c1,const MyComplex &_c2) {return ((_c1.a == _c2.a) && (_c1.b == _c2.b));}
bool operator!=(const MyComplex &_c1,const MyComplex &_c2) {return !(_c1 == _c2);}

MyVector3::MyVector3(const double &_v1, const double &_v2, const double &_v3) : x(_v1), y(_v2), z(_v3) {};                    //向量的一些函数
MyVector3::MyVector3(const MyVector3 &_c) : x(_c.x), y(_c.y), z(_c.z) {};
MyVector3 & MyVector3::operator=(const MyVector3 &_c)
{
	x = _c.x;
	y = _c.y;
	z = _c.z;
	return *this;
}
MyVector3 normalize(const MyVector3 &_c)
{
	return MyVector3(_c.x / sqrt(_c.x * _c.x + _c.y * _c.y + _c.z * _c.z), _c.y / sqrt(_c.x * _c.x + _c.y * _c.y + _c.z * _c.z), _c.z / sqrt(_c.x * _c.x + _c.y * _c.y + _c.z * _c.z));
}

MyVector3 operator*(double _k, const MyVector3 &_c)
{
	return MyVector3(_k * _c.x, _k * _c.y, _k * _c.z);
}

MyVector3 operator*(const MyVector3 &_c, double _k)
{
	return MyVector3(_k * _c.x, _k * _c.y, _k * _c.z);
}

double operator*(const MyVector3 &_c1, const MyVector3 &_c2)
{
	return (_c1.x * _c2.x + _c1.y * _c2.y + _c1.z * _c2.z);
}

MyVector3 cross(const MyVector3 &_c1,const MyVector3 &_c2)
{
	return MyVector3(_c1.y * _c2.z - _c1.z * _c2.y, _c1.z * _c2.x - _c1.x * _c2.z, _c1.x * _c2.y - _c1.y * _c2.x);
}

double dot(const MyVector3 &_c1,const MyVector3 &_c2)
{
	return _c1.x * _c2.x + _c1.y * _c2.y + _c1.z * _c2.z;
}

MyVector3 operator+(const MyVector3 &_c1,const MyVector3 &_c2)
{
	return MyVector3(_c1.x + _c2.x, _c1.y + _c2.y, _c1.z + _c2.z);
}

MyVector3 operator-(const MyVector3 &_c1,const MyVector3 &_c2)
{
	return MyVector3(_c1.x - _c2.x, _c1.y - _c2.y, _c1.z - _c2.z);
}

MyVector3 operator-(const MyVector3 & _c)
{
	return MyVector3(-_c.x, -_c.y, -_c.z);
}