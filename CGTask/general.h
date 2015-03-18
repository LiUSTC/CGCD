#ifndef GENERAL_H 
#define GENERAL_H 
#include <vector>

class MyComplex                             //复数
{
public:
	MyComplex();
	MyComplex(const MyComplex &);
	MyComplex & operator=(const MyComplex &);

	friend bool operator==(const MyComplex &,const MyComplex &) ;
	friend bool operator!=(const MyComplex &,const MyComplex &) ;
	friend MyComplex operator*(const MyComplex &,const MyComplex &) ;

	void setReal(double _aa)
	{
		a = _aa;
	}
	void setImag(double _bb)
	{
		b = _bb;
	}
	double real()
	{
		return a;
	}
	double imag()
	{
		return b;
	}
private:
	double a;
	double b;
};                       

class MyVector3                            //三维向量
{
public:
	MyVector3(const double &_v1 = 0, const double &_v2 = 0, const double &_v3 = 0);
	MyVector3(const MyVector3 &);
	MyVector3 & operator=(const MyVector3 &);
	friend MyVector3 operator*(double,const MyVector3 &);
	friend MyVector3 operator*(const MyVector3 &, double);
	friend double operator*(const MyVector3 &, const MyVector3 &);
	friend MyVector3 normalize(const MyVector3 &);
	friend MyVector3 cross(const MyVector3 &,const MyVector3 &);
	friend double dot(const MyVector3 &,const MyVector3 &);
	friend MyVector3 operator+(const MyVector3 &,const MyVector3 &);
	friend MyVector3 operator-(const MyVector3 &,const MyVector3 &);
	friend MyVector3 operator-(const MyVector3 &);

	double x;
	double y;
	double z;
};

class MyVector2
{
public:
	MyVector2(const double &_v1 = 0, const double &_v2 = 0);
	double x;
	double y;
};

#endif