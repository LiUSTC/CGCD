#include "stdafx.h"
#include <math.h>
#include <stack>
#include <queue>
#include <vector>
#include <time.h>

/*-------------------------------以下为各个功能的实现函数-------------------------------*/
void DrawLine(HDC _hdc, POINT _ptBeg, POINT _ptEnd, COLORREF _color)   //画直线程序，使用Bresenham画线算法
{
	int _xBeg = _ptBeg.x;
	int _yBeg = _ptBeg.y;
	int _xEnd = _ptEnd.x;
	int _yEnd = _ptEnd.y;
	int _i;
	int _x, _y, _dx, _dy;
	int _e,_s;

	_dx = _xEnd - _xBeg; _dy = _yEnd - _yBeg;
	if(abs(_dx) >= abs(_dy))                      //斜率绝对值小于等于1，递增_x
	{
		if (!(_xBeg <= _xEnd))                   //我们希望直线起点的横坐标值小于终点的横坐标值
		{
		myswap(_xBeg, _xEnd);
		myswap(_yBeg, _yEnd);
		}

		_dx = _xEnd - _xBeg; _dy = _yEnd - _yBeg;
		_x = _xBeg; _y = _yBeg;
		_e = -_dx;
		_s = (_dy == 0) ? 1 : (_dy / abs(_dy));

		for(_i = 0; _i <= _dx; _i++)
		{
			SetPixel(_hdc, _x, _y, _color);
			_x = _x + 1;
			_e = _e + 2 * _s * _dy;
			if(_e >= 0)
			{
				_y = _y + _s;
				_e = _e - 2 * _dx;
			}
		}
	}
	else                                      //斜率绝对值大于1，递增_y
	{
		if (!(_yBeg <= _yEnd))                   //我们希望直线起点的纵坐标值小于终点的纵坐标值
		{
		myswap(_xBeg, _xEnd);
		myswap(_yBeg, _yEnd);
		}

		_dx = _xEnd - _xBeg; _dy = _yEnd - _yBeg;
		_x = _xBeg; _y = _yBeg;
		_e = -_dy;
		_s = (_dx == 0) ? 1 : (_dx / abs(_dx));

		for(_i = 0; _i <= _dy; _i++)
		{
			SetPixel(_hdc, _x, _y, _color);
			_y = _y + 1;
			_e = _e + 2 * _s * _dx;
			if(_e >= 0)
			{
				_x = _x + _s;
				_e = _e - 2 * _dy;
			}
		}
	}
}

void DrawVirtualLine(HDC _hdc, POINT _ptBeg, POINT _ptEnd, COLORREF _color)   //画虚线函数
{
	int _xBeg = _ptBeg.x;
	int _yBeg = _ptBeg.y;
	int _xEnd = _ptEnd.x;
	int _yEnd = _ptEnd.y;
	int _i;
	int _flag = 1;
	int _j = 0;
	int _x, _y, _dx, _dy;
	int _e,_s;

	_dx = _xEnd - _xBeg; _dy = _yEnd - _yBeg;
	if(abs(_dx) >= abs(_dy))                      //斜率绝对值小于等于1，递增x
	{
		if (!(_xBeg <= _xEnd))                   //我们希望直线起点的横坐标值小于终点的横坐标值
		{
		myswap(_xBeg, _xEnd);
		myswap(_yBeg, _yEnd);
		}

		_dx = _xEnd - _xBeg; _dy = _yEnd - _yBeg;
		_x = _xBeg; _y = _yBeg;
		_e = -_dx;
		_s = (_dy == 0) ? 1 : (_dy / abs(_dy));

		for(_i = 0; _i <= _dx; _i++)
		{
			_j = _j + 1;
			if (_j%5 == 0)
				_flag = _flag * (-1);
			if (_flag == 1)
				SetPixel(_hdc, _x, _y, _color);
			_x = _x + 1;
			_e = _e + 2 * _s * _dy;
			if(_e >= 0)
			{
				_y = _y + _s;
				_e = _e - 2 * _dx;
			}
		}
	}
	else                                      //斜率绝对值大于1，递增y
	{
		if (!(_yBeg <= _yEnd))                   //我们希望直线起点的纵坐标值小于终点的纵坐标值
		{
		myswap(_xBeg, _xEnd);
		myswap(_yBeg, _yEnd);
		}

		_dx = _xEnd - _xBeg; _dy = _yEnd - _yBeg;
		_x = _xBeg; _y = _yBeg;
		_e = -_dy;
		_s = (_dx == 0) ? 1 : (_dx / abs(_dx));

		for(_i = 0; _i <= _dy; _i++)
		{
			_j = _j + 1;
			if (_j%5 == 0)
				_flag = _flag * (-1);
			if (_flag == 1)
				SetPixel(_hdc, _x, _y, _color);
			_y = _y + 1;
			_e = _e + 2 * _s * _dx;
			if(_e >= 0)
			{
				_x = _x + _s;
				_e = _e - 2 * _dy;
			}
		}
	}
}

void DrawEllipse(HDC _hdc, POINT _ptCen, int _a, int _b, COLORREF _color)         //画椭圆函数，使用中点椭圆算法
{
	int _xCen = _ptCen.x;
	int _yCen = _ptCen.y;
	int _x, _y;
	long double _d1, _d2;

	_x = 0, _y = _b;                        //先画第一象限部分，设圆心为（0，0），方便计算，画点时按椭圆中心平移
	_d1 = (double)_b * _b + (double)_a * _a * (_b - 0.5) * (_b - 0.5) - (double)_a * _a * _b * _b;
	SetPixel(_hdc, _x + _xCen, _y + _yCen, _color);             //按椭圆中心平移，根据对称性画其余象限
	SetPixel(_hdc, _x + _xCen, 2 * _yCen - (_y + _yCen), _color);
	SetPixel(_hdc, 2 * _xCen - (_x + _xCen), _y + _yCen, _color);
	SetPixel(_hdc, 2 * _xCen - (_x + _xCen), 2 * _yCen - (_y + _yCen), _color);
	while ((double)_b * _b * (_x + 1) < (double)_a * _a * (_y - 0.5))     //切线斜率大于-1，比较右点与右上点
	{
		if (_d1 < 0)
		{
			_d1 = _d1 + (double)_b * _b * (2 * _x + 3);
			_x = _x + 1;
		}
		else
		{
			_d1 = _d1 + ((double)_b * _b * (2 * _x + 3) + (double)_a * _a * (-2 * _y + 2));
			_x = _x + 1;
			_y = _y - 1;
		}
		SetPixel(_hdc, _x + _xCen, _y + _yCen, _color);             //按椭圆中心平移
		SetPixel(_hdc, _x + _xCen, 2 * _yCen - (_y + _yCen), _color);
		SetPixel(_hdc, 2 * _xCen - (_x + _xCen), _y + _yCen, _color);
		SetPixel(_hdc, 2 * _xCen - (_x + _xCen), 2 * _yCen - (_y + _yCen), _color);
	}

	_d2 = (double)_b * _b * (_x + 0.5) * (_x + 0.5) + (double)_a * _a * (_y - 1) * (_y - 1) - (double)_a * _a * _b * _b;
	while (_y > 0)                                 //切线斜率小于-1，比较下点与右下点
	{
		if (_d2 < 0)
		{
			_d2 = _d2 + (double)_b * _b * (2 * _x + 2) + (double)_a * _a * (-2 * _y + 3);
			_x = _x + 1;
			_y = _y - 1;
		}
		else
		{
			_d2 = _d2 + (double)_a * _a * (-2 * _y + 3);
			_y = _y - 1;
		}
		SetPixel(_hdc, _x + _xCen, _y + _yCen, _color);             //按椭圆中心平移
		SetPixel(_hdc, _x + _xCen, 2 * _yCen - (_y + _yCen), _color);
		SetPixel(_hdc, 2 * _xCen - (_x + _xCen), _y + _yCen, _color);
		SetPixel(_hdc, 2 * _xCen - (_x + _xCen), 2 * _yCen - (_y + _yCen), _color);
	}
	_x = _a, _y = 0;
	SetPixel(_hdc, _x + _xCen, _y + _yCen, _color);             //按椭圆中心平移
	SetPixel(_hdc, _x + _xCen, 2 * _yCen - (_y + _yCen), _color);
	SetPixel(_hdc, 2 * _xCen - (_x + _xCen), _y + _yCen, _color);
		SetPixel(_hdc, 2 * _xCen - (_x + _xCen), 2 * _yCen - (_y + _yCen), _color);
}

void DrawVirtualRect(HDC _hdc, POINT _ptBeg, POINT _ptEnd, COLORREF _color)        //画虚线框函数
{
	POINT _ptRT,_ptLB;   //RightTop和LeftBottom
	_ptRT.x = _ptEnd.x; _ptRT.y = _ptBeg.y;
	_ptLB.x = _ptBeg.x; _ptLB.y = _ptEnd.y;
	DrawVirtualLine(_hdc,_ptBeg,_ptRT,_color);
	DrawVirtualLine(_hdc,_ptBeg,_ptLB,_color);
	DrawVirtualLine(_hdc,_ptRT,_ptEnd,_color);
	DrawVirtualLine(_hdc,_ptLB,_ptEnd,_color);
}

void DrawFill(HDC _hdc, POINT _ptSet, COLORREF _color, RECT _rect)                      //区域填充算法，采用种子填充算法
{
	POINT _pt = _ptSet;
	std::stack<POINT> _pts;
	_pts.push(_pt);
	while(!_pts.empty())
	{
		_pt = _pts.top();
		_pts.pop();
		if((_pt.x >= _rect.left) && (_pt.x <= _rect.right) && (_pt.y <= _rect.bottom) && (_pt.y >= _rect.top))
		{
			COLORREF _pt_color = GetPixel(_hdc, _pt.x, _pt.y);
			if (_pt_color == GetBkColor(_hdc))
			{
				SetPixel(_hdc, _pt.x, _pt.y, _color);
			}
			_pt.x = _pt.x - 1;
			_pt_color = GetPixel(_hdc, _pt.x, _pt.y);
			if (_pt_color == GetBkColor(_hdc))
			{
					_pts.push(_pt);	
			}
			_pt.x = _pt.x + 2;
			_pt_color = GetPixel(_hdc, _pt.x, _pt.y);
			if (_pt_color == GetBkColor(_hdc))
			{
				_pts.push(_pt);
			}
			_pt.x = _pt.x - 1; _pt.y = _pt.y + 1;
			_pt_color = GetPixel(_hdc, _pt.x, _pt.y);
			if (_pt_color == GetBkColor(_hdc))
			{
				_pts.push(_pt);
			}
			_pt.y = _pt.y - 2;
			_pt_color = GetPixel(_hdc, _pt.x, _pt.y);
			if (_pt_color == GetBkColor(_hdc))
			{
				_pts.push(_pt);
			}
		}
	}
}

void DrawBezier(HDC _hdc, std::vector<POINT> _vertex, int _vcnt, COLORREF _color)
{
	POINT _pre, _next;
	int _k;
	double _i, _t;
	double _n = _vcnt;
	double _dt = 1 / _n;
	double _a0, _a1, _a2, _a3;
	double _b0, _b1, _b2, _b3;
	for (_k = 0; _k < _vcnt - 1; _k++)
	{
		DrawVirtualLine(_hdc, _vertex[_k], _vertex[_k + 1], 26642);
	}
	if (_vcnt > 3)
	{
		for (_k = 0; _k < _vcnt - 3; _k += 3)
		{
			_a0 = _vertex[_k].x;
			_a1 = -3 * _vertex[_k].x + 3 * _vertex[_k + 1].x;
			_a2 = 3 * _vertex[_k].x - 6 * _vertex[_k + 1].x + 3 * _vertex[_k + 2].x;
			_a3 = -_vertex[_k].x + 3 * _vertex[_k + 1].x - 3 * _vertex[_k + 2].x + _vertex[_k + 3].x;
			_b0 = _vertex[_k].y;
			_b1 = -3 * _vertex[_k].y + 3 * _vertex[_k + 1].y;
			_b2 = 3 * _vertex[_k].y - 6 * _vertex[_k + 1].y + 3 * _vertex[_k + 2].y;
			_b3 = -_vertex[_k].y + 3 * _vertex[_k + 1].y - 3 * _vertex[_k + 2].y + _vertex[_k + 3].y;
			for (_i = 0; _i < _n; _i += (1 / _n))
			{
				_t = _i * _dt;
				_next.x =(_a0 + _a1 * _t + _a2 * _t * _t + _a3 * _t * _t * _t);
				_next.y =(_b0 + _b1 * _t + _b2 * _t * _t + _b3 * _t * _t * _t);
				if (_i == 0)
				{
					_pre = _next;
				}
				if ((_pre.x != _next.x) || (_pre.y != _next.y))
				{
					DrawLine(_hdc, _pre, _next, _color);
				}
				_pre = _next;
			}
		}
	}
}

void DrawBSpline(HDC _hdc, std::vector<POINT> _vertex, int _vcnt, COLORREF _color)
{
	POINT _pre, _next;
	int _k;
	double _i, _t;
	double _n = _vcnt;
	double _dt = 1 / _n;
	double _a0, _a1, _a2, _a3;
	double _b0, _b1, _b2, _b3;
	for (_k = 0; _k < _vcnt - 1; _k++)
	{
		DrawVirtualLine(_hdc, _vertex[_k], _vertex[_k + 1], 26642);
	}
	if (_vcnt > 3)
	{
		for (_k = 0; _k < _vcnt - 3; _k += 1)
		{
			_a0 = (_vertex[_k].x + 4 * _vertex[_k + 1].x + _vertex[_k + 2].x) / 6;
			_a1 = (_vertex[_k + 2].x - _vertex[_k].x) / 2;
			_a2 = (_vertex[_k].x - 2 * _vertex[_k + 1].x + _vertex[_k + 2].x) / 2;
			_a3 = -(_vertex[_k].x - 3 * _vertex[_k + 1].x + 3 * _vertex[_k + 2].x - _vertex[_k + 3].x) / 6;
			_b0 = (_vertex[_k].y + 4 * _vertex[_k + 1].y + _vertex[_k + 2].y) / 6;
			_b1 = (_vertex[_k + 2].y - _vertex[_k].y) / 2;
			_b2 = (_vertex[_k].y - 2 * _vertex[_k + 1].y + _vertex[_k + 2].y) / 2;
			_b3 = -(_vertex[_k].y - 3 * _vertex[_k + 1].y + 3 * _vertex[_k + 2].y - _vertex[_k + 3].y) / 6;
			for (_i = 0; _i < _n; _i += (1 / _n))
			{
				_t = _i * _dt;
				_next.x =(_a0 + _a1 * _t + _a2 * _t * _t + _a3 * _t * _t * _t);
				_next.y =(_b0 + _b1 * _t + _b2 * _t * _t + _b3 * _t * _t * _t);
				if (_i == 0)
				{
					_pre = _next;
				}
				if ((_pre.x != _next.x) || (_pre.y != _next.y))
				{
					DrawLine(_hdc, _pre, _next, _color);
				}
				_pre = _next;
			}
		}
	}
}

void DrawKoch(HDC _hdc, RECT _rect,int _KochTime, COLORREF _color)
{
	std::queue<POINT> _ptCtrl;
	std::queue<POINT> _ptTemp;
	int _count = 0;
	POINT _lstart, _lend, _l3rd, _l3rd2, _l3rd3;
	_lstart.x = _rect.left + 20;_lstart.y = _rect.bottom - (_rect.bottom - _rect.top) / 4;
	_lend.x = _rect.right - 20;_lend.y = _rect.bottom - (_rect.bottom - _rect.top) / 4;
	_ptCtrl.push(_lstart);
	_ptCtrl.push(_lend);
	while (_count < _KochTime)
	{
		while(_ptCtrl.size() > 1)
		{
			_lstart = _ptCtrl.front();
			_ptCtrl.pop();
			_lend = _ptCtrl.front();
			_l3rd.x = (2 * _lstart.x + _lend.x) / 3;
			_l3rd.y = (2 * _lstart.y + _lend.y) / 3;
			_l3rd3.x = (_lstart.x + 2 * _lend.x) / 3;
			_l3rd3.y = (_lstart.y + 2 * _lend.y) / 3;
			_l3rd2.x = (int)((_l3rd3.x - _l3rd.x) / 2 + sqrt(3) * (_l3rd3.y - _l3rd.y) / 2 + _l3rd.x);                      //用乘旋转矩阵的方法，相当于顺时针旋转60度
			_l3rd2.y = (int)(-sqrt(3) * (_l3rd3.x - _l3rd.x) / 2 + (_l3rd3.y - _l3rd.y) / 2 + _l3rd.y);
			
			_ptTemp.push(_lstart);
			_ptTemp.push(_l3rd);
			_ptTemp.push(_l3rd2);
			_ptTemp.push(_l3rd3);
			_ptTemp.push(_lend);
		}

		_ptCtrl.pop();

		while(!_ptTemp.empty())
		{
			POINT _temp = _ptTemp.front();
			_ptCtrl.push(_temp);
			_ptTemp.pop();
		}

		_count = _count + 1;
	}

	while(_ptCtrl.size() > 1)
	{
		POINT _istart = _ptCtrl.front();
		_ptCtrl.pop();
		POINT _iend = _ptCtrl.front();
		if ((_istart.x!=_iend.x) || (_istart.y != _iend.y))
		{
			DrawLine(_hdc, _istart, _iend, _color);
		}
	}
	_ptCtrl.pop();
}

void DrawMandelbrot(HDC _hdc,MyComplex _zzmin, MyComplex _zzmax, RECT _rect,COLORREF _color)
{
	int _countmax=300;
	int _count;
	int _x, _y;
	double _factX = (_zzmax.real() - _zzmin.real()) / (_rect.right - _rect.left);
	double _factY = (_zzmax.imag() - _zzmin.imag()) / (_rect.bottom - _rect.top);
	for (_y = _rect.top; _y <= _rect.bottom; _y++)
	{
		for (_x = _rect.left; _x <= _rect.right; _x++)
		{
			_count = 0;
			MyComplex _c;
			MyComplex _z;
			_c.setReal(_zzmin.real() + _x * _factX) ;
			_c.setImag(_zzmin.imag() + _y * _factY) ;
			do
			{
				double _temp = (_z.real() * _z.real() - _z.imag() * _z.imag() + _c.real());
				_z.setImag(2 * _z.real() * _z.imag() + _c.imag());
				_z.setReal(_temp);
				_count = _count + 1;
			}while ((_z.real() * _z.real() + _z.imag() * _z.imag() <= 4) && (_count <= _countmax));
			if (_count <= _countmax)
			{
				SetPixel(_hdc, _x, _y, _count * _color + 300000);
			}
			else
			{
				SetPixel(_hdc, _x, _y, 0);
			}
		}
	}
}

void DrawJulia(HDC _hdc,MyComplex _cc, MyComplex _zzmin, MyComplex _zzmax, RECT _rect, COLORREF _color)
{
	int _countmax=300;
	int _count;
	int _x,_y;
	double _factX = (_zzmax.real() - _zzmin.real()) / (_rect.right - _rect.left);
	double _factY = (_zzmax.imag() - _zzmin.imag()) / (_rect.bottom - _rect.top);
	for (_y = _rect.top; _y <= _rect.bottom; _y++)
	{
		for (_x = _rect.left; _x <= _rect.right; _x++)
		{
			_count = 0;
			MyComplex _z;
			_z.setReal(_zzmin.real() + _x * _factX) ;
			_z.setImag(_zzmin.imag() + _y * _factY) ;
			do
			{
				double _temp = (_z.real() * _z.real() - _z.imag() * _z.imag() + _cc.real());
				_z.setImag(2 * _z.real() * _z.imag() + _cc.imag());
				_z.setReal(_temp);
				_count = _count + 1;
			}while ((_z.real() * _z.real() + _z.imag() * _z.imag() <= 4) && (_count <= _countmax));
			if (_count <= _countmax)
			{
				SetPixel(_hdc, _x, _y, _count * _color + 300000);
			}
			else
			{
				SetPixel(_hdc, _x, _y, 0);
			}
		}
	}
}

void DrawFern(HDC _hdc, RECT _rect,int _fernchoice, COLORREF _color)                  //使用IFS随机迭代算法
{
	double _at[4][4] = {/*1*/0, 0.85, 0.09, -0.09, /*2*/-0.04, 0.65, 0.41, 0.52, /*3*/0, 0.85, 0.2, -0.15, /*4*/0.5, 0.75, 0, 0};
	double _bt[4][4] = {/*1*/0, 0.02, -0.28, 0.25, /*2*/0, 0, 0.46, -0.35, /*3*/0, 0.04, -0.26, 0.28, /*4*/0.25, -0.25, 0, 0};
	double _ct[4][4] = {/*1*/0, -0.02, 0.3, 0.3, /*2*/-0.19, 0, -0.39, 0.25, /*3*/0, -0.04, 0.23, 0.26, /*4*/0.25, 0.25, 0, 0};
	double _dt[4][4] = {/*1*/0.25, 0.83, 0.11, 0.09, /*2*/-0.47, 0.56, 0.61, 0.74, /*3*/0.16, 0.85, 0.22, 0.24, /*4*/-0.5, 0.75, 0, 0};
	double _et[4][4] = {/*1*/0, 0, 0, 0, /*2*/-0.12, 0.06, 0.46, -0.48, /*3*/0, 0, 0, 0, /*4*/0, 0.75, 0, 0};
	double _ft[4][4] = {/*1*/-0.14, 1, 0.6, 0.7, /*2*/0.3, 1.56, 0.4, 0.38, /*3*/0, 1.6, 1.6, 0.44, /*4*/0, 0, 0, 0};
	double _pt[4][4] = {/*1*/0.02, 0.84, 0.07, 0.07, /*2*/0.25, 0.25, 0.25, 0.25, /*3*/0.01, 0.85, 0.07, 0.07, /*4*/0.5, 0.5, 0, 0};
	double _a[4], _b[4], _c[4], _d[4], _e[4], _f[4], _p[4];
	double _rate;
	switch (_fernchoice)
	{
	case 1:
		_rate = 100;
		break;
	case 2:
		_rate = 150;
		break;
	case 3:
		_rate = 50;
		break;
	case 4:
		_rate = 250;
		break;
	default:
		_rate = 50;
		break;
	}

	for(int i = 0; i < 4; i++)
	{
		_a[i]=_at[_fernchoice - 1][i];
		_b[i]=_bt[_fernchoice - 1][i];
		_c[i]=_ct[_fernchoice - 1][i];
		_d[i]=_dt[_fernchoice - 1][i];
		_e[i]=_et[_fernchoice - 1][i];
		_f[i]=_ft[_fernchoice - 1][i];
		_p[i]=_pt[_fernchoice - 1][i];
	}

	double _x = 0;
	double _y = 0;
	int _r;
	int _pr;
	double _m;
	long long _k;
	double _newx, _newy;

	for(_k = 0; _k<99999; _k++)
	{
		srand(GetCycleCount());
		_r = rand();
		_m = (double)((double)(_r % 100) / 100);
		if (_m < _p[0]) _pr = 0;
		else if (_m >= _p[0] && _m < _p[0] + _p[1]) _pr = 1;
		else if (_m >= _p[0] + _p[1] && _m < _p[0] + _p[1] + _p[2]) _pr = 2;
		else _pr = 3;

		_newx = _a[_pr] * _x + _b[_pr] * _y + _e[_pr];
		_newy = _c[_pr] * _x + _d[_pr] * _y + _f[_pr];
		int _xt,_yt;
		_xt = (int)(_x * _rate + (_rect.left + _rect.right) * 0.5 - ((_fernchoice == 4) ? 200 : 0));
		_yt = (int)(-_y * _rate + 600);
		_x = _newx;
		_y = _newy;
		SetPixel(_hdc, _xt, _yt, _color);
	}
}

void DrawTrueScene(HDC _hdc, RECT _rect, COLORREF _bgc)
{
	double rate1 = (double)(_rect.bottom - _rect.top) / 700;
	double rate2 = (double)(_rect.right - _rect.left) / 1400;
	double rate = min(rate1, rate2);
	MtrlParam mtrl;
	LightParam light;
	MyVector3 viewPoint = MyVector3(0, 0, 0);
	MyVector3 viewDir = MyVector3(0, 0, 1);
	mtrl.oa.x = 0.4, mtrl.oa.y = 0.4, mtrl.oa.z = 0.4;
	mtrl.od.x = 0.4, mtrl.od.y = 0.4, mtrl.od.z = 0.4;
	mtrl.os.x = 0.6, mtrl.os.y = 0.6, mtrl.os.z = 0.6;
	mtrl.op = 2;
	light.bi = light.gi = light.ri = 255;
	light.direction = MyVector3(-1, -1, 1);
	TrueScene trueScene;
	TrueObject ball1, ball2;
	TrueObject cube1, cube2;
	ball1.CreateBall(0, 0, 600 * rate, 200 * rate);
	ball1.SetMtrl(mtrl);
	ball1.SetTextureFromFile("balltexture.bmp");
	ball2.CreateBall(-150 * rate, -150 * rate, 100 * rate, 80 * rate);
	ball2.SetMtrl(mtrl);
	cube1.CreateCube(200 * rate, 50 * rate, 100 * rate, 170 * rate);
	cube1.SetMtrl(mtrl);
	cube1.SetTextureFromFile("cubetexture.bmp");
	cube2.CreateCube(100 * rate, -200 * rate, 100 * rate, 150 * rate);
	cube2.SetMtrl(mtrl);
	cube2.SetTextureFromFile("cube2texture.bmp");
	trueScene.AddObject(ball1);
	trueScene.AddObject(ball2);
	trueScene.AddObject(cube1);
	trueScene.AddObject(cube2);
	trueScene.AddLight(light);
	trueScene.SetAmLight(100, 100, 100);
	trueScene.SetView(viewPoint, viewDir);
	trueScene.SetZMax(2000.0);
	trueScene.Render(_hdc, _rect, _bgc);
}

inline unsigned __int64 GetCycleCount()
{
	__asm _emit 0x0F
	__asm _emit 0x31
}

template <typename T> void myswap(T &_x, T &_y)
{
	T _temp = _x;
	_x = _y;
	_y = _temp;
}
