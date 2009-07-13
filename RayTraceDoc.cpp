// RayTraceDoc.cpp : CRayTraceDoc �N���X�̓���̒�`���s���܂��B
//

#include "stdafx.h"
#include "RayTrace.h"

#include "RayTraceDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDoc

IMPLEMENT_DYNCREATE(CRayTraceDoc, CDocument)

BEGIN_MESSAGE_MAP(CRayTraceDoc, CDocument)
	//{{AFX_MSG_MAP(CRayTraceDoc)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDoc �N���X�̍\�z/����

CRayTraceDoc::CRayTraceDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CRayTraceDoc::~CRayTraceDoc()
{
}

BOOL CRayTraceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
	InitDocument();

	return TRUE;
}

void CRayTraceDoc::InitDocument()
{
	m_Root.SetDocument( this );
	m_Light = sp( 1,1,1 );
	d_pTask = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDoc �V���A���C�[�[�V����

void CRayTraceDoc::Serialize(CArchive& ar)
{
	WORD	id;

	if (ar.IsStoring())
	{
		// TODO: ���̈ʒu�ɕۑ��p�̃R�[�h��ǉ����Ă��������B
	}
	else
	{
		// TODO: ���̈ʒu�ɓǂݍ��ݗp�̃R�[�h��ǉ����Ă��������B
		ar >> id;
	}
	m_Root.Serialize(ar);
}

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDoc �N���X�̐f�f

#ifdef _DEBUG
void CRayTraceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRayTraceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDoc �R�}���h

BOOL CRayTraceDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: ���̈ʒu�ɌŗL�̍쐬�p�R�[�h��ǉ����Ă�������
	InitDocument();
	
	return TRUE;
}

void CRayTraceDoc::UpdateAllViews(CView* pSender, LPARAM lHint, CObject* pHint)
{
	// �^�X�N�̏�����
	taskIndex_ = 0;

	m_Root.MakeTask(matrix());

	CDocument::UpdateAllViews(pSender,lHint,pHint);
}
