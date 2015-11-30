// ScnTreeView.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "RayTrace.h"
#include "RayTraceDoc.h"
#include "RayTraceView.h"
#include "ScnTreeView.h"

extern	Node*	m_pEditNode;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScnTreeView

IMPLEMENT_DYNCREATE(CScnTreeView, CTreeView)

CScnTreeView::CScnTreeView()
{
}

CScnTreeView::~CScnTreeView()
{
}

BEGIN_MESSAGE_MAP(CScnTreeView, CTreeView)
	//{{AFX_MSG_MAP(CScnTreeView)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_COMMAND(ID_ADD_SPHERE, OnAddSphere)
	ON_COMMAND(ID_ADD_CUBE, OnAddCube)
	ON_COMMAND(ID_ADD_MINUS, OnAddMinus)
	ON_COMMAND(ID_ADD_MULTIPLE, OnAddMultiple)
	ON_COMMAND(ID_ADD_PLANE, OnAddPlane)
	ON_COMMAND(ID_ADD_PLUS, OnAddPlus)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_AFIN, OnEditAfin)
	ON_COMMAND(ID_TEXTURE, OnEditTexture)
	ON_COMMAND(ID_COLOR, OnEditColor)
	ON_COMMAND(ID_MATERIAL, OnEditMaterial)
	ON_COMMAND(ID_PROPERTY, OnProperty)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_ADD_CONE, OnAddCone)
	ON_COMMAND(ID_ADD_CYLINDER, OnAddCylinder)
	ON_COMMAND(ID_ADD_TORUS, OnAddTorus)
	ON_COMMAND(ID_ADD_POLYGON, OnAddPolygon)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_COMMAND(ID_ADD_TEAPOT, OnAddTeapot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScnTreeView �f�f

#ifdef _DEBUG
void CScnTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CScnTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScnTreeView ���b�Z�[�W �n���h��

void CScnTreeView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	CTreeCtrl&	c = GetTreeCtrl();
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������

	m_normal_IL.Create( IDB_BITMAP_NORMAL, 17, 0, RGB(127,0,255) );
	m_state_IL.Create( IDB_BITMAP_STATE,  17, 0, RGB(255,127,0) );

	c.SetImageList( &m_normal_IL, TVSIL_NORMAL );  
	c.SetImageList( &m_state_IL, TVSIL_STATE );  

	SetWindowLong( c, GWL_STYLE, GetWindowLong( c, GWL_STYLE )
		| TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS |  TVS_EDITLABELS );

}

void CScnTreeView::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������

	if (pTVDispInfo->item.pszText && pTVDispInfo->item.pszText[0])
	{
		HTREEITEM	hItem = pTVDispInfo->item.hItem;
		char*		str   = pTVDispInfo->item.pszText;
		GetTreeCtrl().SetItemText(hItem, str);
		CRayTraceDoc*	pDoc = (CRayTraceDoc*)GetDocument();
		Node*			pTarget;

		if (pTarget = (Node*)GetTreeCtrl().GetItemData(hItem))
			pTarget->Set_Name(str);
	}
	*pResult = 0;
}

BOOL CScnTreeView::AddNode(Node* pNode)
{
	CTreeCtrl&		c = GetTreeCtrl();
	HTREEITEM		hItem;
	CRayTraceDoc*	pDoc = (CRayTraceDoc*)GetDocument();
	Node*			pTarget;

	while(1)
	{
		// �I������Ă���A�C�e���𓾂�B
		if (!(hItem = c.GetSelectedItem()))
			break;

		// �I������Ă���m�[�h�𓾂�B
		if (!(pTarget = (Node*)c.GetItemData(hItem)))
			break;

		if (!(pTarget->AddNode(c, hItem, pNode)))
			break;

		//pNode->updateMatrix(pTarget);

		c.Expand(hItem, TVE_EXPAND);

		pDoc->UpdateAllViews(this);
	
		return TRUE;
	}
	return FALSE;
}


void CScnTreeView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: ���̈ʒu�ɃR���g���[���ʒm�n���h���p�̃R�[�h��ǉ����Ă�������
	
	*pResult = 0;
}

void CScnTreeView::OnEditCut() 
{
	CTreeCtrl&		c = GetTreeCtrl();
	HTREEITEM		hItem = c.GetSelectedItem();
	CRayTraceDoc*	pDoc = (CRayTraceDoc*)GetDocument();
	Node*			pTarget;

	if (!hItem || !(pTarget = (Node*)c.GetItemData(hItem)))
		return;
	
	delete m_pEditNode;
	
	m_pEditNode = (class Node*)pTarget->MakeCopy();
	pDoc->m_Root.Delete(pTarget);
	c.DeleteItem(hItem);
	pDoc->UpdateAllViews(this);
}

void CScnTreeView::OnEditCopy() 
{
	CTreeCtrl&		c = GetTreeCtrl();
	HTREEITEM		hItem = c.GetSelectedItem();
	CRayTraceDoc*	pDoc = (CRayTraceDoc*)GetDocument();
	Node*			pTarget;

	if (!hItem || !(pTarget = (Node*)c.GetItemData(hItem)))
		return;

	delete m_pEditNode;
	m_pEditNode = (class Node*)pTarget->MakeCopy();
}

void CScnTreeView::OnEditPaste() 
{
	if (m_pEditNode) {
		CRayTraceDoc*	pDoc = (CRayTraceDoc*)GetDocument();
		m_pEditNode->SetRoot(&pDoc->m_Root);
		if (AddNode(m_pEditNode))
			m_pEditNode = (class Node*)m_pEditNode->MakeCopy();
	}
}

void CScnTreeView::OnProperty() 
{
	CTreeCtrl&		c = GetTreeCtrl();
	HTREEITEM		hItem = c.GetSelectedItem();
	CRayTraceDoc*	pDoc = (CRayTraceDoc*)GetDocument();
	Node*			pTarget;

	if (!hItem || !(pTarget = (Node*)c.GetItemData(hItem)) || !pTarget->Edit())
		return;

	pDoc->UpdateAllViews(this);
}

void CScnTreeView::OnEditAfin() 
{
	CTreeCtrl&		c = GetTreeCtrl();
	HTREEITEM		hItem = c.GetSelectedItem();
	CRayTraceDoc*	pDoc = (CRayTraceDoc*)GetDocument();
	Node*			pTarget;

	if (!hItem || !(pTarget = (Node*)c.GetItemData(hItem)) || !pTarget->EditAfin())
		return;

	pDoc->UpdateAllViews(this);
}

void CScnTreeView::OnEditColor() 
{
	CTreeCtrl&		c = GetTreeCtrl();
	HTREEITEM		hItem = c.GetSelectedItem();
	CRayTraceDoc*	pDoc = (CRayTraceDoc*)GetDocument();
	Node*			pTarget;

	if (!hItem || !(pTarget = (Node*)c.GetItemData(hItem)) || !pTarget->EditColor())
		return;

	pDoc->UpdateAllViews(this);
}

void CScnTreeView::OnEditMaterial() 
{
	CTreeCtrl&		c = GetTreeCtrl();
	HTREEITEM		hItem = c.GetSelectedItem();
	CRayTraceDoc*	pDoc = (CRayTraceDoc*)GetDocument();
	Node*			pTarget;

	if (!hItem || !(pTarget = (Node*)c.GetItemData(hItem)) || !pTarget->EditMaterial())
		return;

	pDoc->UpdateAllViews(this);
}

void CScnTreeView::OnEditTexture()
{
	CTreeCtrl&		c = GetTreeCtrl();
	HTREEITEM		hItem = c.GetSelectedItem();
	CRayTraceDoc*	pDoc = (CRayTraceDoc*)GetDocument();
	Node*			pTarget;

	if (!hItem || !(pTarget = (Node*)c.GetItemData(hItem)) || !pTarget->EditTexture())
		return;

	pDoc->UpdateAllViews(this);
}

void CScnTreeView::OnAddSphere()	{ Node* obj = new Sphere	(&((CRayTraceDoc*)GetDocument())->m_Root);	if (!AddNode(obj)) delete obj; }
void CScnTreeView::OnAddPlane()		{ Node*	obj = new Plane		(&((CRayTraceDoc*)GetDocument())->m_Root);	if (!AddNode(obj)) delete obj; }
void CScnTreeView::OnAddCone()		{ Node*	obj = new Cone		(&((CRayTraceDoc*)GetDocument())->m_Root);	if (!AddNode(obj)) delete obj; }
void CScnTreeView::OnAddCylinder()	{ Node*	obj = new Cylinder	(&((CRayTraceDoc*)GetDocument())->m_Root);	if (!AddNode(obj)) delete obj; }
void CScnTreeView::OnAddTorus()		{ Node*	obj = new Torus		(&((CRayTraceDoc*)GetDocument())->m_Root);	if (!AddNode(obj)) delete obj; }
void CScnTreeView::OnAddPolygon()	{ Node*	obj = new Polygon2	(&((CRayTraceDoc*)GetDocument())->m_Root);	if (!AddNode(obj)) delete obj; }
void CScnTreeView::OnAddTeapot()	{ Node* obj = new Teapot	(&((CRayTraceDoc*)GetDocument())->m_Root);	if (!AddNode(obj)) delete obj; }
void CScnTreeView::OnAddPlus()		{ Node*	obj = new Plus		(&((CRayTraceDoc*)GetDocument())->m_Root);	if (!AddNode(obj)) delete obj; }
void CScnTreeView::OnAddMinus()		{ Node*	obj = new Minus		(&((CRayTraceDoc*)GetDocument())->m_Root);	if (!AddNode(obj)) delete obj; }
void CScnTreeView::OnAddMultiple()	{ Node*	obj = new Multiple	(&((CRayTraceDoc*)GetDocument())->m_Root);	if (!AddNode(obj)) delete obj; }
void CScnTreeView::OnAddCube()		{ Node*	obj = new Cube		(&((CRayTraceDoc*)GetDocument())->m_Root);	if (!AddNode(obj)) delete obj; }

void CScnTreeView::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	CTreeCtrl&		c = GetTreeCtrl();
	CRayTraceDoc*	pDoc = (CRayTraceDoc*)GetDocument();
	CRayTraceView*  pRayTraceView = (CRayTraceView*)((CSplitterWnd*)GetParent())->GetPane(0,1);

	pRayTraceView->m_SelectedNode = (Node*)c.GetItemData(c.GetSelectedItem());
	
	if (pRayTraceView->m_ViewMode == CRayTraceView::eWireFrame)
		pRayTraceView->Invalidate();

	*pResult = 0;
/*
	// for debug
	static done = false;
	if (!done) {
		done = true;
		OnAddSphere();
	}
*/
}

void CScnTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	CTreeCtrl&	c = GetTreeCtrl();
	HTREEITEM		hItem = c.GetSelectedItem();
	
	c.DeleteAllItems();
	
	CRayTraceDoc*	pDoc = (CRayTraceDoc*)GetDocument();

	pDoc->m_Root.InsertItem(c);

	c.Expand(hItem, TVE_EXPAND);
}

