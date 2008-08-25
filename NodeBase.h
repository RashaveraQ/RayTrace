#ifndef __NODEBASE_H
#define __NODEBASE_H

enum node_type 
{
	SPHERE = 1, PLANE, PLUS, MINUS, MULTIPLE, CONE, CYLINDER, TORUS, POLYGON, CUBE, TEAPOT
};

enum eType { eSELECT, eMOVE, eROTATE, eSCALE, ePIVOT_MOVE };
enum eAxis { eNONE, eX, eY, eZ };

#include "sp.h"
#include "info.h"
class	CRayTraceDoc;
class	CRayTraceView;
class   Viewport;

struct fsize
{
	double	top;
	double	left;
	double	bottom;
	double	right;
};

class NodeBase
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

	double		m_Reflect ;		// 反射率
	double		m_Through ;  	// 透過率
	double		m_Refractive ;	// 屈折率

	struct tagBoundary {
		sp		Center;	// 中心
		double	Radius;	// 半径
		TARGET tagBoundary() : Center(), Radius(1) {}
	} m_Boundary;	// 境界

	char*	m_TextureFileName;
	HDC		m_hTextureDC;
	CSize	m_TextureSize;

	const CRayTraceDoc* m_pDoc;

public:

	NodeBase(const CRayTraceDoc* const pDoc, node_type NodeType, const char* const Name, const sp Color);
	NodeBase(const NodeBase &other);

	TARGET virtual	~NodeBase() {}

	// オペレーション
	void Set_Name(const char* const str);
	sp GetColor(NodeBase* Root, const sp& Light, const sp& K, const sp& L, int nest = 0) const;
	BOOL GetInfo2(const sp& K, const sp& L, Info& info) const;
	sp GetPixel(double x, double y) const;
	TARGET virtual	BOOL GetInfo(const sp& K, const sp& L, Info& info) const = 0;
	virtual	BOOL IsInside(const sp& L) const = 0;
	TARGET matrix getMatrix() { return m_Matrix; }
	BOOL MakeMemoryDCfromTextureFileName();
};

#endif	// __NODEBASE_H

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

