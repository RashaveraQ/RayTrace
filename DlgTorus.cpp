// DlgTorus.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "RayTrace.h"
#include "DlgTorus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTorus �_�C�A���O


CDlgTorus::CDlgTorus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTorus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTorus)
	m_R = 0.0;
	m_r = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgTorus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTorus)
	DDX_Text(pDX, IDC_TORUS_LARGE_R, m_R);
	DDV_MinMaxDouble(pDX, m_R, 0., 100.);
	DDX_Text(pDX, IDC_TORUS_SMALL_R, m_r);
	DDV_MinMaxDouble(pDX, m_r, 0., 100.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTorus, CDialog)
	//{{AFX_MSG_MAP(CDlgTorus)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTorus ���b�Z�[�W �n���h��
