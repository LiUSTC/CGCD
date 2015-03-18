#ifndef TRUESCENE_H
#define TRUESCENE_H
#include <vector>
#include <fstream>

struct MyTriangle3D
{
	int no;
	MyVector3 p1;
	MyVector3 p2;
	MyVector3 p3;
	MyVector3 n;
	MyVector2 p1t;
	MyVector2 p2t;
	MyVector2 p3t;
};

struct MyTriangle2D
{
	MyVector2 p1;
	MyVector2 p2;
	MyVector2 p3;
};

MyTriangle2D Tri3Dto2D(const MyTriangle3D &);			//将三维三角形转化为二维三角形
BOOL IfPInTri2D(const MyTriangle2D &, MyVector2);			//判断平面上点是否在三角形内
double ComputeZ(const MyTriangle3D &, MyVector2);			//根据三角形平面上x,y算z值

struct MtrlParam
{
	MyVector3 oa;	//环境光反射率，三个分量分别对应对红、绿、蓝的反射率，下同
	MyVector3 od;	//漫射光反射率
	MyVector3 os;	//镜面光反射率
	int op;	//镜面高光点的锐度
};

struct LightParam       //模拟平行光
{
	double ri;		//入射光红颜色分量强度
	double gi;		//入射光绿颜色分量强度
	double bi;		//入射光蓝颜色分量强度
	MyVector3 direction;	//入射单位方向向量
};

class TrueObject
{
	friend class TrueScene;
public:
	void CreateBall(double, double, double, double);
	void CreateCube(double, double, double, double);
	void SetMtrl(MtrlParam &);
	void SetTextureFromFile(const char *);
private:
	MtrlParam material;
	MyVector3 SetNVector(const MyVector3 &, const MyVector3 &, const MyVector3 &);
	std::vector<std::vector<MyVector3>> vertex;
	std::vector<MyTriangle3D> triangles;
	std::vector<std::vector<IMAGE24DATA>> texture;    //纹理矩阵
	int countx, county;              //经纬度上的顶点数
};

class TrueScene
{
public:
	void AddLight(const LightParam &);
	void AddObject(const TrueObject &);
	void Render(HDC, RECT, COLORREF);
	void SetView(const MyVector3 &, MyVector3 &);
	void SetAmLight(double, double, double);   
	void SetZMax(double);
private:
	std::vector<LightParam> lightSource;
	std::vector<TrueObject> objects;
	std::vector<MyTriangle3D> triangles;
	MyVector3 amLight;
	MyVector3 viewPoint;
	MyVector3 viewDir;
	double zMax;
};
MyTriangle3D GenerateMyTriangle3D(int, const MyVector3 &, const MyVector3 &, const MyVector3 &, const MyVector3 &);
MyTriangle3D GenerateMyTriangle3D(int, const MyVector3 &, const MyVector3 &, const MyVector3 &, const MyVector3 &, const MyVector2 &, const MyVector2 &, const MyVector2 &);
#endif