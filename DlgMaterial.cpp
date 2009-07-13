// DlgMaterial.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "RayTrace.h"
#include "DlgMaterial.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMaterial �_�C�A���O


CDlgMaterial::CDlgMaterial(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMaterial::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgMaterial)
	m_Reflect = 0.0;
	m_Through = 0.0;
	m_Refractive = 0.0;
	m_Material = -1;
	//}}AFX_DATA_INIT
}


void CDlgMaterial::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMaterial)
	DDX_Text(pDX, IDC_REFLECT, m_Reflect);
	DDV_MinMaxDouble(pDX, m_Reflect, 0., 1.);
	DDX_Text(pDX, IDC_THROUGH, m_Through);
	DDV_MinMaxDouble(pDX, m_Through, 0., 1.);
	DDX_Text(pDX, IDC_REFRACTIVE, m_Refractive);
	DDV_MinMaxDouble(pDX, m_Refractive, 0., 100.);
	DDX_CBIndex(pDX, IDC_MATERIAL, m_Material);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMaterial, CDialog)
	//{{AFX_MSG_MAP(CDlgMaterial)
	ON_CBN_SELCHANGE(IDC_MATERIAL, OnSelchangeMaterial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMaterial ���b�Z�[�W �n���h��

void CDlgMaterial::Set( Node& obj )
{
	m_Reflect		= obj.m_Reflect;
	m_Refractive	= obj.m_Refractive;
	m_Through		= obj.m_Through;
}

void CDlgMaterial::Get( Node& obj )
{
	obj.m_Reflect		= m_Reflect;
	obj.m_Refractive	= m_Refractive;
	obj.m_Through		= m_Through;
}


void CDlgMaterial::OnSelchangeMaterial() 
{
	UpdateData();

	static struct {
		float reflect;
		float refractive;
		float through;
	} material[] = { 
		{ 0.f, 2.417f,	1.f},	// �_�C�A�����h
		{ 0.f, 1.80f,	1.f},	// �K�[�l�b�g
		{ 0.f, 1.764f,	1.f},	// ���r�[�A�T�t�@�C�A
		{ 0.f, 1.717f,	1.f}, // �X�s�l��
		{ 0.f, 1.67f,	1.f}, // �y���h�b�g
		{ 0.f, 1.66f,	1.f}, // �Ђ���
		{ 0.f, 1.717f,	1.f}, // �g�p�[�Y
		{ 0.f, 1.627f,	1.f}, // �g���}����
		{ 0.f, 1.579f,	1.f}, // �G�������h
		{ 0.f, 1.549f,	1.f}, // ����
		{ 0.f, 1.445f,	1.f}, // �I�p�[��
		{ 0.f, 1.8f,	1.f}, // �K���X
		{ 0.f, 1.33f,	1.f}, // ��
	};

	m_Reflect = material[m_Material].reflect;
	m_Refractive = material[m_Material].refractive;
	m_Through = material[m_Material].through;

	UpdateData(FALSE);
}
