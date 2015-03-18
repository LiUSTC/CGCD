#include "stdafx.h"

void TrueScene::Render(HDC _hdc, RECT _rect, COLORREF _bgc)			//使用Z-buffer消隐算法，Phong局部光照模型
{
	double xt, yt;
	double pi = 3.14159265359;
	std::vector<std::vector<double>> zChart;				//深度缓存
	std::vector<std::vector<int>> triChart;					//与深度缓存对应的三角形序号缓存
	std::vector<std::vector<std::vector<int>>> wuYuA;		//预处理的得到的屏幕点可能经过的三角形缓存

	for(int x = 0; x < _rect.right - _rect.left; x++)                     
	{
		std::vector<double> zChartTmp;
		std::vector<int> triChartTmp;
		std::vector<std::vector<int>> wuYuATmp;
		for(int y = 0; y < _rect.bottom - _rect.top; y++)
		{
			std::vector<int> tmpTmp;
			zChartTmp.push_back(zMax);
			triChartTmp.push_back(-1);
			wuYuATmp.push_back(tmpTmp);
		}
		zChart.push_back(zChartTmp);
		triChart.push_back(triChartTmp);
		wuYuA.push_back(wuYuATmp);
	}

	for(int i = 0; i < objects.size(); i++)                                 //先把背离视点的三角形剔除
	{
		for(int j = 0; j < objects[i].triangles.size(); j++)
		{
			if(dot(objects[i].triangles[j].n, viewDir) < 0)
			{
				objects[i].triangles[j].no = i;
				triangles.push_back(objects[i].triangles[j]);
			}
		}
	}
	
	for(int a = 0; a < triangles.size(); a++)                          //预处理，统计屏幕每个点可能经过的三角形
	{
		double minxt = min(min(triangles[a].p1.x, triangles[a].p2.x), triangles[a].p3.x);
		double minyt = min(min(triangles[a].p1.y, triangles[a].p2.y), triangles[a].p3.y);
		double maxxt = max(max(triangles[a].p1.x, triangles[a].p2.x), triangles[a].p3.x);
		double maxyt = max(max(triangles[a].p1.y, triangles[a].p2.y), triangles[a].p3.y);
		int minx = floor(minxt + (_rect.right - _rect.left) * 0.5);
		int miny = floor(0.5 * (_rect.bottom - _rect.top) - maxyt);
		int maxx = ceil(maxxt + (_rect.right - _rect.left) * 0.5);
		int maxy = ceil(0.5 * (_rect.bottom - _rect.top) - minyt);
		for(int b = minx; b <= maxx; b++)
		{
			for(int c = miny; c <= maxy; c++)
				wuYuA[b][c].push_back(a);
		}
	}

	for(int x = 0; x < _rect.right - _rect.left; x++)                      //Z-buffer消隐算法
	{
		for(int y = 0; y < _rect.bottom - _rect.top; y++)
		{
			double zTmp;
			MyVector2 pTmp;
			xt = x - (_rect.right - _rect.left) * 0.5;
			yt = (_rect.bottom - _rect.top) * 0.5 - y;
			if(!wuYuA[x][y].empty())
			{
				for(int m = 0; m < wuYuA[x][y].size(); m++)
				{
					pTmp.x = xt, pTmp.y = yt;
					if (IfPInTri2D(Tri3Dto2D(triangles[wuYuA[x][y][m]]), pTmp))
					{
						zTmp = ComputeZ(triangles[wuYuA[x][y][m]], pTmp);
						if(zTmp < zChart[x][y])
						{
							triChart[x][y] = wuYuA[x][y][m];
							zChart[x][y] = zTmp;
						}
					}
				}
			}
		}
	}

	for(int x = 0; x < _rect.right - _rect.left; x++)                      //Phong模型
	{
		for(int y = 0; y < _rect.bottom - _rect.top; y++)
		{
			xt = x - (_rect.right - _rect.left) * 0.5;
			yt = (_rect.bottom - _rect.top) * 0.5 - y;
			if(triChart[x][y] < 0) SetPixel(_hdc, x, y, _bgc);
			else
			{
				int RED = 0, GREEN = 0, BLUE = 0;
				double texr, texg, texb;
				if (!objects[triangles[triChart[x][y]].no].texture.empty())				//计算纹理点
				{
					if(triangles[triChart[x][y]].p1t.x !=0 || triangles[triChart[x][y]].p2t.x != 0 || triangles[triChart[x][y]].p3t.x !=0 || triangles[triChart[x][y]].p1t.y != 0 || triangles[triChart[x][y]].p2t.y != 0 || triangles[triChart[x][y]].p3t.y != 0)
					{
						double theta, cc;
						int length = objects[triangles[triChart[x][y]].no].texture.size() - 1;
						int width = objects[triangles[triChart[x][y]].no].texture[0].size() - 1;
						MyVector3 v21(triangles[triChart[x][y]].p2.x - triangles[triChart[x][y]].p1.x, triangles[triChart[x][y]].p2.y - triangles[triChart[x][y]].p1.y, triangles[triChart[x][y]].p2.z - triangles[triChart[x][y]].p1.z);
						MyVector3 vp1(xt - triangles[triChart[x][y]].p1.x, yt - triangles[triChart[x][y]].p1.y, zChart[x][y] - triangles[triChart[x][y]].p1.z);
						if (v21 * v21 !=0 && vp1 * vp1 != 0)
						{
							theta = acos(vp1 * v21 / sqrt((vp1 * vp1) * (v21 * v21)));
							cc = sqrt((vp1 * vp1) / (v21 * v21));
						}
						else
						{
							theta = 0;
							cc = 0;
						}
						MyVector2 u21(triangles[triChart[x][y]].p2t.x - triangles[triChart[x][y]].p1t.x, triangles[triChart[x][y]].p2t.y - triangles[triChart[x][y]].p1t.y);
						MyVector2 up1(cos(theta) * u21.x - sin(theta) * u21.y, sin(theta) * u21.x + cos(theta) * u21.y);
						MyVector2 uu(up1.x * cc, up1.y * cc);
						int tx = (uu.x + triangles[triChart[x][y]].p1t.x) * (double)length;
						int ty = (uu.y + triangles[triChart[x][y]].p1t.y) * (double)width;
						tx = min(tx, length);
						ty = min(ty, width);
						tx = max(tx, 0);
						ty = max(ty, 0);
						IMAGE24DATA temp = objects[triangles[triChart[x][y]].no].texture[tx][ty];
						texr = (double)(temp.red) / 255;
						texg = (double)(temp.green) / 255;
						texb = (double)(temp.blue) / 255;
					}
					else
					{
						texr = 1;
						texg = 1;
						texb = 1;
					}
				}
				else
				{
					texr = 1;
					texg = 1;
					texb = 1;
				}

				for (int k = 0; k < lightSource.size(); k++)
				{
					LightParam light = lightSource[k];
					MyVector3 L = - normalize(light.direction);
					MyVector3 V = normalize(MyVector3(viewPoint.x - xt, viewPoint.y - yt, viewPoint.z - zChart[x][y]));
					MyVector3 H = (L + V) * 0.5;
					
					double cosna = triangles[triChart[x][y]].n * normalize(H);
					for(int count = 1; count < objects[triangles[triChart[x][y]].no].material.op; count++)
					{
						cosna *= cosna;
					}

					double aColorRED = amLight.x * objects[triangles[triChart[x][y]].no].material.oa.x;
					double dColorRED = light.ri * objects[triangles[triChart[x][y]].no].material.od.x * (triangles[triChart[x][y]].n * normalize(L));
					double sColorRED = light.ri * objects[triangles[triChart[x][y]].no].material.os.x * cosna;
					if (dColorRED < 0) dColorRED = 0;
					if (sColorRED < 0) sColorRED = 0;
					RED = aColorRED + dColorRED + sColorRED + RED;

					double aColorGREEN = amLight.y * objects[triangles[triChart[x][y]].no].material.oa.y;
					double dColorGREEN = light.gi * objects[triangles[triChart[x][y]].no].material.od.y * (triangles[triChart[x][y]].n * normalize(L));
					double sColorGREEN = light.gi * objects[triangles[triChart[x][y]].no].material.os.y * cosna;
					if (dColorGREEN < 0) dColorGREEN = 0;
					if (sColorGREEN < 0) sColorGREEN = 0;
					GREEN = aColorGREEN + dColorGREEN + sColorGREEN +GREEN;

					double aColorBLUE = amLight.z * objects[triangles[triChart[x][y]].no].material.oa.z;
					double dColorBLUE = light.bi * objects[triangles[triChart[x][y]].no].material.od.z * (triangles[triChart[x][y]].n * normalize(L));
					double sColorBLUE = light.bi * objects[triangles[triChart[x][y]].no].material.os.z * cosna;
					if (dColorBLUE < 0) dColorBLUE = 0;
					if (sColorBLUE < 0) sColorBLUE = 0;
					BLUE = aColorBLUE + dColorBLUE + sColorBLUE + BLUE;
				}
				if (RED < 0) RED = 0; if (RED > 255) RED = 255;
				if (GREEN < 0) GREEN = 0; if (GREEN > 255) GREEN = 255;
				if (BLUE < 0) BLUE = 0; if (BLUE > 255) BLUE = 255;

				SetPixel(_hdc, x, y, RGB(RED * texr, GREEN * texg, BLUE * texb));
			}
		}
	}
}

void TrueObject::CreateBall(double _xp, double _yp, double _zp, double _ra)
{
	int i = 0;
	int j;
	double pi = 3.14159265359;
	std::vector<std::vector<MyVector2>> uv;
	for(double u = - pi / 2; u < pi / 2 + pi / 64; u += pi / 64)
	{
		j = 0;
		std::vector<MyVector3> temp;
		std::vector<MyVector2> uvTmp;
		for(double v = 0; v < 2 * pi + pi / 64; v += pi / 64)
		{
			MyVector3 tempp;
			MyVector2 uvtemp;
			uvtemp.x = ((double)u + pi / 2) / pi;
			uvtemp.y = (double)v / (2 * pi);
			uvTmp.push_back(uvtemp);
			tempp.x = _xp + _ra * cos(u) * cos(v);
			tempp.y = _yp + _ra * cos(u) * sin(v);
			tempp.z = _zp + _ra * sin(u);
			temp.push_back(tempp);
			++j;		
		}
		vertex.push_back(temp);
		uv.push_back(uvTmp);
		++i;
	}

	countx = i;
	county = j;

	for (int i = 0; i < countx - 1; i++)
	{
		for (int j = 0; j < county; j++)
		{
			MyTriangle3D triangle1;
			triangle1.p1 = vertex[i][j];
			triangle1.p2 = vertex[(i + 1) % countx][(j + 1) % county];
			triangle1.p3 = vertex[(i + 1) % countx][j];
			triangle1.n = SetNVector(triangle1.p1, triangle1.p2, triangle1.p3);
			triangle1.no = -1;
			triangle1.p1t = uv[i][j];
			triangle1.p2t = uv[(i + 1) % countx][(j + 1) % county];
			triangle1.p3t = uv[(i + 1) % countx][j];
			triangles.push_back(triangle1);
			MyTriangle3D triangle2;
			triangle2.p1 = vertex[i][j];
			triangle2.p2 = vertex[i][(j + 1) % county];
			triangle2.p3 = vertex[(i + 1) % countx][(j + 1) % county];
			triangle2.n = SetNVector(triangle2.p1, triangle2.p2, triangle2.p3);
			triangle2.no = -1;
			triangle2.p1t = uv[i][j];
			triangle2.p2t = uv[i][(j + 1) % county];
			triangle2.p3t = uv[(i + 1) % countx][(j + 1) % county];
			triangles.push_back(triangle2);
		}
	}
}

void TrueObject::CreateCube(double _xp, double _yp, double _zp, double _l)
{
	MyVector3 E(_xp + sqrt(5) / 5 * _l, _l + _yp + sqrt(5) / 5 * _l, _zp + sqrt(15) / 5 * _l);
	MyVector3 F(_l + _xp + sqrt(5) / 5 * _l, _l + _yp + sqrt(5) / 5 * _l, _zp + sqrt(15) / 5 * _l);
	MyVector3 G(_l + _xp + sqrt(5) / 5 * _l, _yp + sqrt(5) / 5 * _l, _zp + sqrt(15) / 5 * _l);
	MyVector3 H(_xp + sqrt(5) / 5 * _l, _yp + sqrt(5) / 5 * _l, _zp + sqrt(15) / 5 * _l);
	MyVector3 A(_xp, _l + _yp, _zp);
	MyVector3 B(_l + _xp, _l + _yp, _zp);
	MyVector3 C(_l + _xp, _yp, _zp);
	MyVector3 D(_xp, _yp, _zp);
	MyVector2 t1(0, 0);
	MyVector2 t2(1, 0);
	MyVector2 b1(0, 1);
	MyVector2 b2(1, 1);
	triangles.push_back(GenerateMyTriangle3D(-1, A, B, C, SetNVector(A, B, C), t1, t2, b2));
	triangles.push_back(GenerateMyTriangle3D(-1, A, C, D, SetNVector(A, C, D), t1, b2, b1));
	triangles.push_back(GenerateMyTriangle3D(-1, B, F, G, SetNVector(B, F, G)));
	triangles.push_back(GenerateMyTriangle3D(-1, B, G, C, SetNVector(B, G, C)));
	triangles.push_back(GenerateMyTriangle3D(-1, F, E, H, SetNVector(F, E, H)));
	triangles.push_back(GenerateMyTriangle3D(-1, F, H, G, SetNVector(F, H, G)));
	triangles.push_back(GenerateMyTriangle3D(-1, E, A, D, SetNVector(E, A, D)));
	triangles.push_back(GenerateMyTriangle3D(-1, E, D, H, SetNVector(E, D, H)));
	triangles.push_back(GenerateMyTriangle3D(-1, E, F, B, SetNVector(E, F, B)));
	triangles.push_back(GenerateMyTriangle3D(-1, E, B, A, SetNVector(E, B, A)));
	triangles.push_back(GenerateMyTriangle3D(-1, D, C, G, SetNVector(D, C, G)));
	triangles.push_back(GenerateMyTriangle3D(-1, D, G, H, SetNVector(D, G, H)));
}

MyTriangle2D Tri3Dto2D(const MyTriangle3D &_tri3d)
{
	MyTriangle2D _temp;
	_temp.p1.x = _tri3d.p1.x;
	_temp.p1.y = _tri3d.p1.y;
	_temp.p2.x = _tri3d.p2.x;
	_temp.p2.y = _tri3d.p2.y;
	_temp.p3.x = _tri3d.p3.x;
	_temp.p3.y = _tri3d.p3.y;
	return _temp;
}

MyTriangle3D GenerateMyTriangle3D(int _no, const MyVector3 &_p1, const MyVector3 &_p2, const MyVector3 &_p3, const MyVector3 &_n)
{
	MyTriangle3D temp;
	temp.no = _no;
	temp.p1 = _p1;
	temp.p2 = _p2; 
	temp.p3 = _p3;
	temp.n = _n;
	return temp;
}

MyTriangle3D GenerateMyTriangle3D(int _no, const MyVector3 &_p1, const MyVector3 &_p2, const MyVector3 &_p3, const MyVector3 &_n, const MyVector2 & _p1t, const MyVector2 & _p2t, const MyVector2 & _p3t)
{
	MyTriangle3D temp;
	temp.no = _no;
	temp.p1 = _p1;
	temp.p2 = _p2; 
	temp.p3 = _p3;
	temp.n = _n;
	temp.p1t = _p1t;
	temp.p2t = _p2t; 
	temp.p3t = _p3t;
	return temp;
}

BOOL IfPInTri2D(const MyTriangle2D & _tri2d, MyVector2 _p)
{
	double pax = _p.x - _tri2d.p1.x; double pay = _p.y - _tri2d.p1.y;
	double pbx = _p.x - _tri2d.p2.x; double pby = _p.y - _tri2d.p2.y;
	double pcx = _p.x - _tri2d.p3.x; double pcy = _p.y - _tri2d.p3.y;
	
	double paXpb = pax * pby - pay * pbx;
	double pbXpc = pbx * pcy - pby * pcx;
	double pcXpa = pcx * pay - pcy * pax;

	if ((paXpb <= 0 && pbXpc <= 0 && pcXpa <= 0) || (paXpb >= 0 && pbXpc >= 0 && pcXpa >= 0)) return true;
	return false;
}

MyVector3 TrueObject::SetNVector(const MyVector3 & _p1, const MyVector3 & _p2, const MyVector3 & _p3)         //三个点排列按顺时针绕序
{
	MyVector3 _temp;
	double _xu,_yu,_zu,_xv,_yv,_zv,_d;
	_xu = _p2.x - _p1.x;
	_yu = _p2.y - _p1.y;
	_zu = _p2.z - _p1.z;
	_xv = _p3.x - _p1.x;
	_yv = _p3.y - _p1.y;
	_zv = _p3.z - _p1.z;
	_d = sqrt((_yu * _zv - _yv * _zu) * (_yu * _zv - _yv * _zu) + (_zu * _xv - _zv * _xu) * (_zu * _xv - _zv * _xu) + (_xu * _yv - _xv * _yu) * (_xu * _yv - _xv * _yu));
	_temp.x = (_yu * _zv - _yv * _zu) / _d;
	_temp.y = (_zu * _xv - _zv * _xu) / _d;
	_temp.z = (_xu * _yv - _xv * _yu) / _d;
	return _temp;
}

double ComputeZ(const MyTriangle3D & _tri3d, MyVector2 _p)
{
	double _z;
	_z = _tri3d.p1.z + (_tri3d.n.x * (_tri3d.p1.x - _p.x) + _tri3d.n.y * (_tri3d.p1.y - _p.y)) / _tri3d.n.z;
	return _z;
}

void TrueObject::SetMtrl(MtrlParam & _mtrl)
{
	material = _mtrl;
}


void TrueScene::AddLight(const LightParam & _light)
{
	lightSource.push_back(_light);
}

void TrueScene::SetZMax(double _zMax)
{
	zMax = _zMax;
}

void TrueScene::AddObject(const TrueObject & _object)
{
	objects.push_back(_object);
}

void TrueScene::SetView(const MyVector3 & _viewPoint, MyVector3 & _viewDir)
{
	viewPoint = _viewPoint;
	viewDir = _viewDir;
}

void TrueScene::SetAmLight(double _x, double _y, double _z)
{
	amLight.x = _x;
	amLight.y = _y;
	amLight.z = _z;
}

void TrueObject::SetTextureFromFile(const char *s)
{
	BITMAPFILEHEADER filehead;
	BITMAPINFOHEADER infohead;
	int bHeight, bWidth, bCount, store24Width, store8Width = 0;//分别为高，宽和每个像素占的位数
	int i, j;
	std::ifstream inbmp(s, std::ios::binary);
	if(inbmp)
	{
		inbmp.read((char *)(& filehead), sizeof(filehead));
		inbmp.read((char *)(& infohead), sizeof(infohead));
		bHeight = infohead.biHeight;
		bWidth = infohead.biWidth;
		bCount = infohead.biBitCount;
		store24Width = (4 * ((bWidth * bCount + 31) / 32)) / 3;

		RGBQUAD palette[256];
		RGBQUAD Colormap[256];
		for(i = 0; i < 256; i++){
			Colormap[i].rgbBlue = i;
			Colormap[i].rgbRed = i;
			Colormap[i].rgbGreen = i;
			Colormap[i].rgbReserved = 0;
		}

		IMAGE24DATA **imagedata;
		imagedata=new IMAGE24DATA *[2048];

		for(i = 0; i < 2048; i++)
		{  
			imagedata[i] = new IMAGE24DATA[2048];
		}

		if(bCount <= 8)
		{
			for(i = 0; i < pow(2, bCount); i++){
				inbmp.read((char *)(& palette[i]), sizeof(palette[i]));
			}
		}

		for(i = bHeight - 1; i >= 0; i--)
		{
			for(j = 0; j < store24Width; j++)
			{
				inbmp.read((char *)(& imagedata[i][j]), sizeof(imagedata[i][j]));
			}
		}

		for(i = 0; i < bWidth; i++)
		{
			std::vector<IMAGE24DATA> temp;
			for(j = 0; j < bHeight; j++)
			{
				temp.push_back(imagedata[j][i]);
			}
			texture.push_back(temp);
		}
	}
}