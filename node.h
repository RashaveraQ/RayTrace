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

class	Node : public CObject
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
		tagBoundary() : Center(), Radius(1) {}
	} m_Boundary;	// 境界

	CString		m_TextureFileName;
	CDC			m_TextureDC;		// テクスチャイメージ格納用
	CSize		m_TextureSize;

	const CRayTraceDoc* m_pDoc;
	void updateMatrix() {
		m_Matrix = m_Move * m_Rotate * m_Scale;
	}

public:

	Node(const CRayTraceDoc* const pDoc, node_type NodeType, const char* const Name, const sp Color = sp(-1,-1,-1))
	: m_NodeType(NodeType), m_Reflect(0), m_Through(0), m_Refractive(1), m_TextureFileName("")
	{
		m_pDoc = (const CRayTraceDoc*)pDoc;
		Set_Name( Name );
		MakeMemoryDCfromTextureFileName();
		m_Material = Color.getMaterial();
	}
	Node( const Node &other );

	virtual	~Node() {}

	// オペレーション
			void Set_Name(const char* const str) { strncpy_s(m_Name, sizeof(m_Name), str, 99); }  
	virtual void SetDocument(const CRayTraceDoc* const pDoc) { m_pDoc = (const CRayTraceDoc*)pDoc; }

			sp GetColor(const sp& K, const sp& L, const Node * pOmit) const;
			BOOL GetInfo2(const sp& K, const sp& L, Info& info, const Node *pOmit) const;
			sp GetPixel(double x, double y) const;
	virtual	BOOL GetInfo(const sp& K, const sp& L, Info& info, const Node *pOmit) const = 0;
	virtual	BOOL IsInside(const sp& L) const = 0;
	
	void Move(eAxis axis, double d);
	void Move(POINT d);
	void Rotate(eAxis axis, double d);
	void Rotate(POINT d);
	void Scale(eAxis axis, double d);
	void MovePivot(eAxis axis, double d);

	virtual void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	virtual bool SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& m) const;
	virtual	BOOL AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target) { return FALSE; }
	virtual void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	virtual void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST) = 0;
	virtual BOOL Edit();
			BOOL EditAfin();
			BOOL EditColor();
			BOOL EditMaterial();
			BOOL EditTexture();
			BOOL MakeMemoryDCfromTextureFileName();
	virtual const Node*		MakeCopy() const = 0;
	virtual	BOOL Delete( Node* ) { return FALSE; }
	virtual	void Serialize(CArchive& ar);

	// インプリメンテーション
	friend CDlgMatrix;
	friend CDlgMaterial;
	friend Geometry;
};

#endif

/*
	int	GetInfo(const sp& K, const sp& L, Info& info) const;

	引数
		const sp& K,	方向ベクトル
		const sp& L,	視点の位置
		Info& info		情報セットのアドレス
	戻り値
		int		0: 交点なし
				1: 交点あり
*/

