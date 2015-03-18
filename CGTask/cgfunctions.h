#ifndef CGFUNCTIONS_H
#define CGFUNCTIONS_H
#include <vector>

void DrawLine(HDC, POINT, POINT, COLORREF);                                        //��ֱ��
void DrawVirtualLine(HDC, POINT, POINT, COLORREF);                                 //������
void DrawEllipse(HDC, POINT, int, int, COLORREF);                                  //����Բ
void DrawVirtualRect(HDC, POINT, POINT, COLORREF);                                 //�����߿�
void DrawFill(HDC, POINT, COLORREF, RECT);                                         //�������
void DrawBezier(HDC, std::vector<POINT>, int, COLORREF);                           //������������
void DrawBSpline(HDC, std::vector<POINT>, int, COLORREF);                          //��B��������
void DrawKoch(HDC, RECT, int, COLORREF);                                           //��Koch����
void DrawMandelbrot(HDC, MyComplex, MyComplex, RECT,COLORREF);                      //��Mandelbrot��
void DrawJulia(HDC, MyComplex, MyComplex, MyComplex, RECT, COLORREF);               //��Julia��
void DrawFern(HDC, RECT, int, COLORREF);                                           //��ާ��ֲ��
void DrawTrueScene(HDC, RECT, COLORREF);                                           //����ʵ��ͼ��
inline unsigned __int64 GetCycleCount();                                           //�˺����ǻ�ȡ����֮��CPUʱ�������������ó�����������ӣ�Ч���ܺ�
template <typename T> void myswap(T &, T &);                                       //����������������ŵ�general.h�ڣ������ǳ���ֻ�÷���

#endif