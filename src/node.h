#ifndef __NODE_H
#define __NODE_H

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
#define PERSPECTIVE_RATIO 0.1

// 境界
struct Boundary {
	double	Radius;	// 半径
	sp		Center;	// 中心
	Boundary(double r = 0, const sp& c = sp(0,0,0)) : Radius(r), Center(c) {}
};

class	Node : public CObject
{
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

	double		m_Reflect ;		// 反射率
	double		m_Through ;  	// 透過率
	double		m_Refractive ;	// 屈折率

	struct Boundary m_Boundary;	// 境界

	CString		m_TextureFileName;
	CDC			m_TextureDC;		// テクスチャイメージ格納用
	CSize		m_TextureSize;

//	const CRayTraceDoc* m_pDoc;
	const Node* m_Root;

	void updateMatrix() {
		m_Matrix = m_Move * m_Rotate * m_Scale;
		OnUpdateBoundary();
	}
private:
	virtual Boundary getBoundary() = 0;
	void OnUpdateBoundary(); 
public:

	Node(const Node* root, node_type NodeType, const char* const Name, const sp Color = sp(-1,-1,-1))
	: m_Root(root), m_pParent(0), m_NodeType(NodeType), m_Reflect(0), m_Through(0), m_Refractive(1)
	, m_TextureFileName("")
	{
		Set_Name( Name );
		MakeMemoryDCfromTextureFileName();
		m_Material = Color.getMaterial();
	}
	Node(const Node &other);

	virtual	~Node() {}

	// オペレーション
			void Set_Name(const char* const str) { strncpy_s(m_Name, sizeof(m_Name), str, 99); }  
	virtual void SetRoot(const Node* root) { m_Root = root; }

			sp GetColor(const sp& K, const sp& L, int nest, const Info* pHint, bool fromOutSide) const;
			bool GetInfo2(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
			sp GetPixel(double x, double y) const;
	virtual	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const = 0;
private:
	virtual	bool IsInside(const sp& L) const = 0;
public:
			bool IsInside2(const sp& L) const;

	virtual	BOOL AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target) { return FALSE; }
	virtual void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
			void Move(eAxis axis, double d);
			void Move(POINT d);
			void Rotate(eAxis axis, double d);
			void Rotate(POINT d);
			void Scale(eAxis axis, double d);
			void MovePivot(eAxis axis, double d);
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

