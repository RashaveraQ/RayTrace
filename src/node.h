#ifndef __NODE_H
#define __NODE_H

#include "node_type.h"

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
class   DevNode;

struct fsize
{
	float	top;
	float	left;
	float	bottom;
	float	right;
};
#define PERSPECTIVE_RATIO 0.1

// 境界
struct Boundary {
	float	Radius;	// 半径
	sp		Center;	// 中心
	Boundary(float r = 0, const sp& c = sp(0, 0, 0)) : Radius(r), Center(c) {}
};

class	Node : public CObject
{
	virtual Boundary getBoundary() = 0;
	void OnUpdateBoundary();
	virtual	bool IsInside(const sp& L) const = 0;
protected:
	Node*		m_pParent;
	// アトリビュート
	node_type	m_NodeType;
	char		m_Name[99];	// 名前
	D3DMATERIAL9 m_Material;

	matrix		m_Scale;	// スケール変換
	matrix		m_Rotate;	// 回転
	matrix		m_Move;		// 平行移動
	matrix		m_Pivot;	// マニュピレータの中心点
	matrix		m_Matrix;

	float		m_Reflect;		// 反射率
	float		m_Through;  	// 透過率
	float		m_Refractive;	// 屈折率

	struct Boundary m_Boundary;	// 境界

	CString		m_TextureFileName;
	CDC			m_TextureDC;		// テクスチャイメージ格納用
	CSize		m_TextureSize;

	//	const CRayTraceDoc* m_pDoc;
	Node* m_Root;

	void updateMatrix();

	virtual bool newDeviceNode() = 0;
public:
	DevNode** m_devNode;

	Node(Node* const root, node_type NodeType, const char* const Name, const sp Color = sp(-1, -1, -1));
	Node(const Node &other);

	virtual	~Node();

	// オペレーション
	void Set_Name(const char* const str) { strncpy_s(m_Name, sizeof(m_Name), str, 99); }
	virtual void SetRoot(Node* const root) { m_Root = root; }

	sp GetColor(const sp& K, const sp& L, int nest, const Info* pHint, bool fromOutSide);
	bool GetInfo2(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide);
	sp GetPixel(float x, float y) const;
	virtual	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const = 0;

	bool IsInside2(const sp& L);

	virtual	BOOL AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target) { return FALSE; }
	virtual void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	void Move(eAxis axis, float d);
	void Move(POINT d);
	void Rotate(eAxis axis, float d);
	void Rotate(POINT d);
	void Scale(eAxis axis, float d);
	void MovePivot(eAxis axis, float d);
	virtual bool SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& m) const;

	virtual void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	virtual void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST) = 0;
	virtual BOOL Edit();
	BOOL EditAfin();
	BOOL EditColor();
	BOOL EditMaterial();
	BOOL EditTexture();

	// インプリメンテーション
	friend CDlgMatrix;
	friend CDlgMaterial;
	friend Geometry;

	bool MakeMemoryDCfromTextureFileName();

	virtual const Node*		MakeCopy() const = 0;
	virtual	bool Delete(Node*) { return false; }
	virtual	void Serialize(CArchive& ar);

	matrix getMatrix() { return m_Matrix; }
	Boundary getBoundary2() { return m_Boundary; }

	void SetParent(Node* pParent) {
		m_pParent = pParent;
	}

};

#endif

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

