// DlgPolygon.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "RayTrace.h"
#include "DlgPolygon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPolygon �_�C�A���O


CDlgPolygon::CDlgPolygon(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPolygon::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPolygon)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CDlgPolygon::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPolygon)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPolygon, CDialog)
	//{{AFX_MSG_MAP(CDlgPolygon)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPolygon ���b�Z�[�W �n���h��
