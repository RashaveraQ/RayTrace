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
	float	top;
	float	left;
	float	bottom;
	float	right;
};
#define PERSPECTIVE_RATIO 0.1

// ���E
struct Boundary {
	float	Radius;	// ���a
	sp		Center;	// ���S
	Boundary(float r = 0, const sp& c = sp(0,0,0)) : Radius(r), Center(c) {}
};

class	Node : public CObject
{
protected:
	Node*		m_pParent;
	// �A�g���r���[�g
	node_type	m_NodeType;
	char		m_Name[99];	// ���O
	D3DMATERIAL9 m_Material;

	matrix		m_Scale;	// �X�P�[���ϊ�
	matrix		m_Rotate;	// ��]
	matrix		m_Move;		// ���s�ړ�
	matrix		m_Pivot;	// �}�j���s���[�^�̒��S�_
	matrix		m_Matrix;

	float		m_Reflect ;		// ���˗�
	float		m_Through;  	// ���ߗ�
	float		m_Refractive;	// ���ܗ�

	struct Boundary m_Boundary;	// ���E

	CString		m_TextureFileName;
	CDC			m_TextureDC;		// �e�N�X�`���C���[�W�i�[�p
	CSize		m_TextureSize;

//	const CRayTraceDoc* m_pDoc;
	Node* m_Root;


	void updateMatrix() {
		m_Matrix = m_Move * m_Rotate * m_Scale;
		OnUpdateBoundary();
	}
private:
	virtual Boundary getBoundary() = 0;
	void OnUpdateBoundary(); 
public:
	class DevNode** m_devNode;

	Node(Node* const root, node_type NodeType, const char* const Name, const sp Color = sp(-1, -1, -1));
	Node(const Node &other);

	virtual	~Node();

	// �I�y���[�V����
			void Set_Name(const char* const str) { strncpy_s(m_Name, sizeof(m_Name), str, 99); }  
	virtual void SetRoot(Node* const root) { m_Root = root; }

			sp GetColor(const sp& K, const sp& L, int nest, const Info* pHint, bool fromOutSide);
			bool GetInfo2(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide);
			sp GetPixel(float x, float y) const;
	virtual	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const = 0;
private:
	virtual	bool IsInside(const sp& L) const = 0;
public:
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

	// �C���v�������e�[�V����
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

	����
		const sp& K,	�����x�N�g��
		const sp& L,	���_�̈ʒu
		Info& info		���Z�b�g�̃A�h���X
	�߂�l
		bool	true : ��_����
				false: ��_�Ȃ�
*/

