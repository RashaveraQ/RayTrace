// ChildFrm.cpp : CChildFrame �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "RayTrace.h"

#include "RayTraceDoc.h"
#include "RayTraceView.h"
#include "ScnTreeView.h"
#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame �N���X�̍\�z/����

CChildFrame::CChildFrame()
{
	// TODO: �����o�������R�[�h�����̈ʒu�ɒǉ����Ă��������B
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: ���̈ʒu�� CREATESTRUCT cs �̐ݒ���s���āAWindow �N���X�܂���
	//       �X�^�C����ύX���Ă��������B

	return CMDIChildWnd::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CChildFrame �N���X�̐f�f

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame �N���X�̃��b�Z�[�W�n���h��

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: ���̈ʒu�ɌŗL�̏�����ǉ����邩�A�܂��͊�{�N���X���Ăяo���Ă�������
	return	m_SplitterWnd.CreateStatic(this, 1, 2, WS_CHILD | WS_VISIBLE)
		&&	m_SplitterWnd.CreateView(0, 0, RUNTIME_CLASS(CScnTreeView), CSize(200,100), pContext)
		&&	m_SplitterWnd.CreateView(0, 1, RUNTIME_CLASS(CRayTraceView), CSize(300,100), pContext);
}
