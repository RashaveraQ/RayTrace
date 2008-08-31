#ifndef __BASENODE_H
#define __BASENODE_H

enum node_type 
{
	SPHERE = 1, PLANE, PLUS, MINUS, MULTIPLE, CONE, CYLINDER, TORUS, POLYGON, CUBE, TEAPOT
};

enum eType { eSELECT, eMOVE, eROTATE, eSCALE, ePIVOT_MOVE };
enum eAxis { eNONE, eX, eY, eZ };

struct	sp;
struct	Info;
class	CDlgMatrix;
class	CDlgMaterial;
class	CRayTraceDoc;
class	CRayTraceView;
class   Viewport;
struct	matrix;

struct fsize
{
	double	top;
	double	left;
	double	bottom;
	double	right;
};

class BaseNode
{
protected:
	// アトリビュート
	node_type	m_NodeType;
	char		m_Name[99];	// 名前
	D3DMATERIAL9 m_Material;

	matrix		m_Scale;	// スケール変換
	matrix		m_Rotate;	// 回転
	matrix		m_Move;		// 平行移動
	matrix		m_Pivot;	// マニュピレータの中心点
	matrix		m_Matrix;


	// アトリビュート
	char*		m_TextureFileName;
	HDC			m_hTextureDC;		// テクスチャイメージ格納用
	CSize		m_TextureSize;

	struct tagBoundary {
		sp		Center;	// 中心
		double	Radius;	// 半径
		TARGET tagBoundary() : Center(), Radius(1) {}
	} m_Boundary;	// 境界
	BaseNode*		m_DeviceData;

public:
	double		m_Reflect ;		// 反射率
	double		m_Through ;  	// 透過率
	double		m_Refractive ;	// 屈折率

	TARGET BaseNode() : m_DeviceData(0) {}
	BaseNode(node_type NodeType, const char* const Name, const sp Color);
	~BaseNode();

	void updateDeviceData();

	// オペレーション
	void Set_Name(const char* const str) { memcpy(m_Name, str, 99); }  
	sp GetColor(const sp* K, const sp* L, int nest);
	BOOL GetInfo2(const sp* K, const sp* L, Info* info);
	sp GetPixel(double x, double y);
	BOOL GetInfo(const sp* K, const sp* L, Info* info);
	BOOL IsInside(const sp* L);
	
	TARGET matrix getMatrix() { return m_Matrix; }
	TARGET BaseNode* getDeviceData() { return m_DeviceData; }
	BOOL MakeMemoryDCfromTextureFileName();
};

#endif	// __BASENODE_H

/*
	bool	GetInfo(const sp& K, const sp& L, Info& info) const;

	引数
		const sp& K,	方向ベクトル
		const sp& L,	視点の位置
		Info& info		情報セットのアドレス
	戻り値
		bool	true : 交点あり
				false: 交点なし
*/

