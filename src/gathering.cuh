#pragma once
#include "node.cuh"

class DevGathering : public DevNode
{
protected:
	short	m_Member;
	DevNode*	m_Node[1000];
public:
	// コンストラクタ
	__device__ DevGathering(DevNode** const root, node_type NodeType, const char* const Name)
		: DevNode(root, NodeType, Name), m_Member(0) {}
	__device__ DevGathering(const DevGathering& other);

	// デストラクタ
	__device__  ~DevGathering();

	__device__ void SetRoot(DevNode** const root);
	//void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& Matrix) const;
	//bool SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& m) const;
	//bool AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target);
	//void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& Matrix) const;
	//void InsertItem(CTreeCtrl& c, HTREEITEM hParent = TVI_ROOT, HTREEITEM hInsertAfter = TVI_LAST);
	__device__ bool Delete(DevNode*);
	//void Serialize(CArchive& ar);
};
