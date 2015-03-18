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

MyTriangle2D Tri3Dto2D(const MyTriangle3D &);			//����ά������ת��Ϊ��ά������
BOOL IfPInTri2D(const MyTriangle2D &, MyVector2);			//�ж�ƽ���ϵ��Ƿ�����������
double ComputeZ(const MyTriangle3D &, MyVector2);			//����������ƽ����x,y��zֵ

struct MtrlParam
{
	MyVector3 oa;	//�����ⷴ���ʣ����������ֱ��Ӧ�Ժ졢�̡����ķ����ʣ���ͬ
	MyVector3 od;	//����ⷴ����
	MyVector3 os;	//����ⷴ����
	int op;	//����߹������
};

struct LightParam       //ģ��ƽ�й�
{
	double ri;		//��������ɫ����ǿ��
	double gi;		//���������ɫ����ǿ��
	double bi;		//���������ɫ����ǿ��
	MyVector3 direction;	//���䵥λ��������
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
	std::vector<std::vector<IMAGE24DATA>> texture;    //�������
	int countx, county;              //��γ���ϵĶ�����
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