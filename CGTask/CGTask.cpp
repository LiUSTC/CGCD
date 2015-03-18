// CGTask.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "CGTask.h"
#include <string>
#include <memory>

#define MAX_LOADSTRING 100

using namespace std;
// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������
int x=0;                                        //�˵���ѡ��״̬

POINT ptZero;
POINT pBeg, pEnd;         //ֱ�ߡ����Ρ���Բ�ȵ�����յ�
POINT pSet;                //����������ʼ��
int pSetcount;
std::vector<POINT> pCtrl;   //���߿��Ƶ�
int cccount = 0;       //���߿��Ƶ����
MyComplex zMin, zMax, cSet;    //Mandelbrot����Julia���Ĳ���
POINT MjPoint;
int drawkmjf = 0;
int KochTimes = 0;
int kochcount = 0;
int ferntype = 2;
int keep = 0;

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK       JuliaProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK       KochProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK       FernProc(HWND, UINT, WPARAM, LPARAM);
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CGTASK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CGTASK));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CGTASK));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CGTASK);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	static HDC hdc;
	HMENU hMenu;
	static RECT rect;
	RECT *lprect = &rect;
	GetClientRect (hWnd, &rect) ;

	switch (message)
	{
	case WM_MOUSEWHEEL:
		switch (x)
		{
		case 7:
			MjPoint.x = LOWORD(lParam);
			MjPoint.y = HIWORD(lParam);
			ScreenToClient(hWnd, &MjPoint);
			if(MjPoint.x >= rect.left && MjPoint.y >= rect.top && MjPoint.x <= rect.right && MjPoint.y <= rect.bottom)
			{
				if ((short)HIWORD(wParam) > 0)
				{
					if ((zMax.imag() - zMin.imag()) > 0.000023)
					{
						double tempMinReal = zMin.real() + (zMax.real() - zMin.real()) * 0.2 * (double)(MjPoint.x - rect.left)/((double)(rect.right - rect.left));
						double tempMinImag = zMin.imag() + (zMax.imag() - zMin.imag()) * 0.2 * (double)(MjPoint.y - rect.top)/((double)(rect.bottom - rect.top));
						zMax.setReal(tempMinReal + 0.8 * zMax.real() - 0.8 * zMin.real());
						zMax.setImag(tempMinImag + 0.8 * zMax.imag() - 0.8 * zMin.imag());
						zMin.setReal(tempMinReal);
						zMin.setImag(tempMinImag);
						InvalidateRect(hWnd, lprect, TRUE);
					}
				}
				else
				{
					if ((zMax.imag() -zMin.imag()) < 2)
					{
						double tempMinReal = zMin.real() - (zMax.real() - zMin.real()) * 0.25 * (double)(MjPoint.x - rect.left)/((double)(rect.right - rect.left));
						double tempMinImag = zMin.imag() - (zMax.imag() - zMin.imag()) * 0.25 * (double)(MjPoint.y - rect.top)/((double)(rect.bottom - rect.top));
						zMax.setReal(tempMinReal + 1.25 * zMax.real() - 1.25 * zMin.real());
						zMax.setImag(tempMinImag + 1.25 * zMax.imag() - 1.25 * zMin.imag());
						zMin.setReal(tempMinReal);
						zMin.setImag(tempMinImag);
						InvalidateRect(hWnd, lprect, TRUE);
					}
					if (((zMax.imag() -zMin.imag()) > 2) || (zMax.real() -zMin.real()) > 3)
					{
						zMin.setReal(-2);
						zMin.setImag(-1);
						zMax.setReal(1);
						zMax.setImag(1);
						InvalidateRect(hWnd, lprect, TRUE);
					}
					if(zMin.imag() < -1)
					{
						double add = -1 - zMin.imag();
						zMax.setImag(zMax.imag() + add);
						zMin.setImag(zMin.imag() + add);
						InvalidateRect(hWnd, lprect, TRUE);
					}
					if(zMax.imag() > 1)
					{
						double minus = zMax.imag() - 1;
						zMax.setImag(zMax.imag() - minus);
						zMin.setImag(zMin.imag() - minus);
						InvalidateRect(hWnd, lprect, TRUE);
					}
					if(zMin.real() < -2)
					{
						double add = -1 - zMin.real();
						zMax.setReal(zMax.real() + add);
						zMin.setReal(zMin.real() + add);
						InvalidateRect(hWnd, lprect, TRUE);
					}
					if(zMax.real() > 1)
					{
						double minus = zMax.real() - 1;
						zMax.setReal(zMax.real() - minus);
						zMin.setReal(zMin.real() - minus);
						InvalidateRect(hWnd, lprect, TRUE);
					}
				}
			}
			break;
		case 8:
			MjPoint.x = LOWORD(lParam);
			MjPoint.y = HIWORD(lParam);
			ScreenToClient(hWnd, &MjPoint);
			if(MjPoint.x >= rect.left && MjPoint.y >= rect.top && MjPoint.x <= rect.right && MjPoint.y <= rect.bottom)
			{
				if ((short)HIWORD(wParam) > 0)
				{
					if ((zMax.imag() - zMin.imag()) > 0.000023)
					{
						double tempMinReal = zMin.real() + (zMax.real() - zMin.real()) * 0.2 * (double)(MjPoint.x - rect.left)/((double)(rect.right - rect.left));
						double tempMinImag = zMin.imag() + (zMax.imag() - zMin.imag()) * 0.2 * (double)(MjPoint.y - rect.top)/((double)(rect.bottom - rect.top));
						zMax.setReal(tempMinReal + 0.8 * zMax.real() - 0.8 * zMin.real());
						zMax.setImag(tempMinImag + 0.8 * zMax.imag() - 0.8 * zMin.imag());
						zMin.setReal(tempMinReal);
						zMin.setImag(tempMinImag);
						InvalidateRect(hWnd, lprect, TRUE);
					}
				}
				else
				{
					if ((zMax.imag() -zMin.imag()) < 4)
					{
						double tempMinReal = zMin.real() - (zMax.real() - zMin.real()) * 0.25 * (double)(MjPoint.x - rect.left)/((double)(rect.right - rect.left));
						double tempMinImag = zMin.imag() - (zMax.imag() - zMin.imag()) * 0.25 * (double)(MjPoint.y - rect.top)/((double)(rect.bottom - rect.top));
						zMax.setReal(tempMinReal + 1.25 * zMax.real() - 1.25 * zMin.real());
						zMax.setImag(tempMinImag + 1.25 * zMax.imag() - 1.25 * zMin.imag());
						zMin.setReal(tempMinReal);
						zMin.setImag(tempMinImag);
						InvalidateRect(hWnd, lprect, TRUE);
					}
					if (((zMax.imag() -zMin.imag()) > 4) || (zMax.real() -zMin.real()) > 4)
					{
						zMin.setReal(-2);
						zMin.setImag(-2);
						zMax.setReal(2);
						zMax.setImag(2);
						InvalidateRect(hWnd, lprect, TRUE);
					}
					if(zMin.imag() < -2)
					{
						double add = -2 - zMin.imag();
						zMax.setImag(zMax.imag() + add);
						zMin.setImag(zMin.imag() + add);
						InvalidateRect(hWnd, lprect, TRUE);
					}
					if(zMax.imag() > 2)
					{
						double minus = zMax.imag() - 2;
						zMax.setImag(zMax.imag() - minus);
						zMin.setImag(zMin.imag() - minus);
						InvalidateRect(hWnd, lprect, TRUE);
					}
					if(zMin.real() < -2)
					{
						double add = -2 - zMin.real();
						zMax.setReal(zMax.real() + add);
						zMin.setReal(zMin.real() + add);
						InvalidateRect(hWnd, lprect, TRUE);
					}
					if(zMax.real() > 2)
					{
						double minus = zMax.real() - 2;
						zMax.setReal(zMax.real() - minus);
						zMin.setReal(zMin.real() - minus);
						InvalidateRect(hWnd, lprect, TRUE);
					}
				}
			}
			break;
		default:
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		switch (x)
		{
		case 1:
		case 2:
			InvalidateRect(hWnd, lprect, TRUE);
			pBeg.x = LOWORD(lParam);
			pBeg.y = HIWORD(lParam);
			pEnd = pBeg;
			break;
		case 3:
			pSet.x = LOWORD(lParam);
			pSet.y = HIWORD(lParam);
			hdc = GetDC(hWnd);
			DrawFill(hdc, pSet, 321321, rect);
			ReleaseDC(hWnd, hdc);
			break;
		case 4:
		case 5:
			POINT xxxx;
			xxxx.x = LOWORD(lParam);
			xxxx.y = HIWORD(lParam);
			pCtrl.push_back(xxxx);
			++cccount;
			InvalidateRect(hWnd, lprect, TRUE);
			break;
		default:
			break;
		}
		break;

	case WM_MOUSEMOVE:
		if (wParam & MK_LBUTTON)
		{
			switch (x)
			{
			case 1:
				pEnd.x = LOWORD(lParam);
				pEnd.y = HIWORD(lParam);
				InvalidateRect(hWnd, lprect, TRUE);
				break;
			case 2:
				pEnd.x = LOWORD(lParam);
				pEnd.y = HIWORD(lParam);
				InvalidateRect(hWnd, lprect, TRUE);
				hdc = GetDC(hWnd);
				DrawVirtualRect(hdc, pBeg, pEnd, 0);
				ReleaseDC(hWnd, hdc);
				break;
			default:
				break;
			}
		}
		break;

	case WM_LBUTTONUP:
		switch (x)
		{
		case 1:
		case 2:
			pEnd.x = LOWORD(lParam);
			pEnd.y = HIWORD(lParam);
			InvalidateRect(hWnd, lprect, TRUE);
			break;
		default:
			break;
		}
		break;
		
	case WM_COMMAND:
		hMenu = GetMenu(hWnd);
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_CLEAR:
			pBeg.x = pBeg.y = 0;                       //�л��˵�ʱ���������յ�
			pEnd = pBeg;
			pSet = pBeg;
			for(;cccount > 0;cccount --)
			{
				pCtrl.pop_back();
			}
			InvalidateRect(hWnd, lprect, TRUE);
			drawkmjf = 0;
			kochcount = 0;
			break;
		case IDM_LRESET:
			x = 0;
			pBeg.x = pBeg.y = 0;                       //�л��˵�ʱ���������յ�
			pEnd = pBeg;
			pSet = pBeg;
			for(;cccount > 0;cccount --)
			{
				pCtrl.pop_back();
			}
			drawkmjf = 0;
			kochcount = 0;
			InvalidateRect(hWnd, lprect, TRUE);
			break;
		case IDM_LINE:
			x = 1;
			pBeg.x = pBeg.y = 0;                       //�л��˵�ʱ���������յ�
			pEnd = pBeg;
			InvalidateRect(hWnd, lprect, TRUE);
			break;
		case IDM_ELLIPSE:
			x = 2;
			pBeg.x = pBeg.y = 0;                       //�л��˵�ʱ���������յ�
			pEnd = pBeg;
			InvalidateRect(hWnd, lprect, TRUE);
			break;
		case IDM_FILL:
			x = 3;
			break;
		case IDM_BEZIER:
			x = 4;
			for(;cccount > 0;cccount --)
			{
				pCtrl.pop_back();
			}
			InvalidateRect(hWnd, lprect, TRUE);
			break;
		case IDM_BSPLINE:
			x = 5;
			for(;cccount > 0;cccount --)
			{
				pCtrl.pop_back();
			}
			InvalidateRect(hWnd, lprect, TRUE);
			break;
		case IDM_KOCH:
			x = 6;
			drawkmjf = 1;
			kochcount = 0;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, KochProc);
			InvalidateRect(hWnd, lprect, TRUE);
			break;
		case IDM_MANDEL:
			x = 7;
			drawkmjf = 1;
			zMin.setReal(-2);
			zMin.setImag(-1);
			zMax.setReal(1);
			zMax.setImag(1);
			InvalidateRect(hWnd, lprect, TRUE);
			break;
		case IDM_JULIA:
			x = 8;
			drawkmjf = 1;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, JuliaProc);
			zMin.setReal(-2);
			zMin.setImag(-2);
			zMax.setReal(2);
			zMax.setImag(2);
			InvalidateRect(hWnd, lprect, TRUE);
			break;
		case IDM_FERN:
			x = 9;
			drawkmjf = 1;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, FernProc);
			InvalidateRect(hWnd, lprect, TRUE);
			break;
		case IDM_TRUESCENE:
			x = 10;
			InvalidateRect(hWnd, lprect, TRUE);
			break;
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		switch (x)
			{
			case 1:
				if ((pBeg.x != pEnd.x)||(pBeg.y != pEnd.y))
					DrawLine(hdc, pBeg, pEnd, 0);
				break;
			case 2:
				POINT xx; int aa, bb;
				if ((pBeg.x != pEnd.x)||(pBeg.y != pEnd.y))
				{
					xx.x = (int)(0.5 * (pBeg.x + pEnd.x));
					xx.y = (int)(0.5 * (pBeg.y + pEnd.y));
					aa = abs((int)(0.5 * pEnd.x - 0.5 * pBeg.x));
					bb = abs((int)(0.5 * pEnd.y - 0.5 * pBeg.y));
					DrawEllipse(hdc, xx, aa, bb, 0);
				}
				break;
			case 3:
				break;
			case 4:
				if (cccount > 0)
				{
					DrawBezier(hdc, pCtrl, cccount, 0);
				}
				break;
			case 5:
				if (cccount > 0)
				{
					DrawBSpline(hdc, pCtrl, cccount, 0);
				}
				break;
			case 6:
				if (drawkmjf > 0)
				{
					if (kochcount < KochTimes)
					{
						InvalidateRect(hWnd, lprect, TRUE);
						DrawKoch(hdc, rect, kochcount, 0);
						Sleep(1000);
						kochcount++;
					}
					else
					{
						DrawKoch(hdc, rect, KochTimes, 0);
						kochcount = 0;
						drawkmjf = 0;
					}
				}
				break;
			case 7:
				if (drawkmjf > 0)
				{
					DrawMandelbrot(hdc, zMin, zMax, rect, 2000);
				}
				break;
			case 8:
				if (drawkmjf > 0)
				{
					DrawJulia(hdc,cSet, zMin, zMax, rect, 2000);
				}
				break;
			case 9:
				if (drawkmjf > 0)
				{
					DrawFern(hdc, rect, ferntype, 0x0000cc00);
				}
				break;
			case 10:
				DrawTrueScene(hdc, rect, RGB(255, 255, 255));
				break;
			default:
				DrawText(hdc, TEXT("���Ǽ����ͼ��ѧ��ҵ1.0��������½�룬ѧ�ţ�SA13023936"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
				break;
			}
		// TODO: �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL CALLBACK JuliaProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)      //Julia�Ի��������        
{

	LPTSTR p1 = new TCHAR;
	LPTSTR p2 = new TCHAR;
	int c1 = 0;
	int c2 = 0;
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			c1 = GetDlgItemText(hDlg, IDC_EDIT3, p1, 10);
			c2 = GetDlgItemText(hDlg, IDC_EDIT5, p2, 10);
			if (c1 > 0 && c2 > 0)
			{
				TCHAR * s1 = (TCHAR *) p1;
				TCHAR * s2 = (TCHAR *) p2;
				cSet.setReal(_ttof(s1));
				cSet.setImag(_ttof(s2));
				EndDialog (hDlg, 0);
				return TRUE ;
			}
			delete [] p1;
			delete [] p2;
			MessageBox(hDlg, TEXT("����������飡"), TEXT("����"), MB_OK);
		}
		break ;
	}
	return FALSE ;
}

BOOL CALLBACK KochProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)      //Koch�Ի��������        
{
	LPTSTR p = new TCHAR;
	int ccc = 0;
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD (wParam))
		{
		case IDOK:
			ccc = GetDlgItemText(hDlg, IDC_EDIT1, p, 10);
			if (ccc > 0)
			{
				TCHAR * sss = (TCHAR *) p;
				KochTimes = _ttoi(sss);
				EndDialog (hDlg, 0);
				return TRUE ;
			}
			delete [] p;
			MessageBox(hDlg, TEXT("����������飡"), TEXT("����"), MB_OK);
		}
		break ;
	}
	return FALSE;
}

BOOL CALLBACK FernProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			if (IsDlgButtonChecked(hDlg,IDC_RADIO1) == BST_CHECKED) ferntype = 1;
			if (IsDlgButtonChecked(hDlg,IDC_RADIO2) == BST_CHECKED) ferntype = 2;
			if (IsDlgButtonChecked(hDlg,IDC_RADIO6) == BST_CHECKED) ferntype = 3;
			if (IsDlgButtonChecked(hDlg,IDC_RADIO8) == BST_CHECKED) ferntype = 4;
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}