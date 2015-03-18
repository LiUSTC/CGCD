#ifndef CGFUNCTIONS_H
#define CGFUNCTIONS_H
#include <vector>

void DrawLine(HDC, POINT, POINT, COLORREF);                                        //画直线
void DrawVirtualLine(HDC, POINT, POINT, COLORREF);                                 //画虚线
void DrawEllipse(HDC, POINT, int, int, COLORREF);                                  //画椭圆
void DrawVirtualRect(HDC, POINT, POINT, COLORREF);                                 //画虚线框
void DrawFill(HDC, POINT, COLORREF, RECT);                                         //区域填充
void DrawBezier(HDC, std::vector<POINT>, int, COLORREF);                           //画贝塞尔曲线
void DrawBSpline(HDC, std::vector<POINT>, int, COLORREF);                          //画B样条曲线
void DrawKoch(HDC, RECT, int, COLORREF);                                           //画Koch曲线
void DrawMandelbrot(HDC, MyComplex, MyComplex, RECT,COLORREF);                      //画Mandelbrot集
void DrawJulia(HDC, MyComplex, MyComplex, MyComplex, RECT, COLORREF);               //画Julia集
void DrawFern(HDC, RECT, int, COLORREF);                                           //画蕨类植物
void DrawTrueScene(HDC, RECT, COLORREF);                                           //画真实感图形
inline unsigned __int64 GetCycleCount();                                           //此函数是获取开机之后CPU时钟数，用来设置成随机数的种子，效果很好
template <typename T> void myswap(T &, T &);                                       //交换函数，本来想放到general.h内，可老是出错，只好放这

#endif