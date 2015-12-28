// DlgMatrix.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "RayTrace.h"
#include "DlgMatrix.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgMatrix ダイアログ


CDlgMatrix::CDlgMatrix(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgMatrix::IDD, pParent), m_Matrix(4, 4)
{
	//{{AFX_DATA_INIT(CDlgMatrix)
	m_1_2 = 0.0;
	m_1_3 = 0.0;
	m_1_4 = 0.0;
	m_2_1 = 0.0;
	m_2_2 = 0.0;
	m_2_3 = 0.0;
	m_2_4 = 0.0;
	m_3_1 = 0.0;
	m_3_2 = 0.0;
	m_3_3 = 0.0;
	m_3_4 = 0.0;
	m_4_1 = 0.0;
	m_4_2 = 0.0;
	m_4_3 = 0.0;
	m_4_4 = 0.0;
	m_1_1 = 0.0;
	m_move_x = 0.0;
	m_move_y = 0.0;
	m_move_z = 0.0;
	m_rotate_angle = 0.0;
	m_rotate_axis_x = 0.0;
	m_rotate_axis_y = 0.0;
	m_rotate_axis_z = 0.0;
	m_scale_x = 1.0;
	m_scale_y = 1.0;
	m_scale_z = 1.0;
	//}}AFX_DATA_INIT
}


void CDlgMatrix::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgMatrix)
	DDX_Text(pDX, IDC_EDIT_MATRIX_1_2, m_1_2);
	DDX_Text(pDX, IDC_EDIT_MATRIX_1_3, m_1_3);
	DDX_Text(pDX, IDC_EDIT_MATRIX_1_4, m_1_4);
	DDX_Text(pDX, IDC_EDIT_MATRIX_2_1, m_2_1);
	DDX_Text(pDX, IDC_EDIT_MATRIX_2_2, m_2_2);
	DDX_Text(pDX, IDC_EDIT_MATRIX_2_3, m_2_3);
	DDX_Text(pDX, IDC_EDIT_MATRIX_2_4, m_2_4);
	DDX_Text(pDX, IDC_EDIT_MATRIX_3_1, m_3_1);
	DDX_Text(pDX, IDC_EDIT_MATRIX_3_2, m_3_2);
	DDX_Text(pDX, IDC_EDIT_MATRIX_3_3, m_3_3);
	DDX_Text(pDX, IDC_EDIT_MATRIX_3_4, m_3_4);
	DDX_Text(pDX, IDC_EDIT_MATRIX_4_1, m_4_1);
	DDX_Text(pDX, IDC_EDIT_MATRIX_4_2, m_4_2);
	DDX_Text(pDX, IDC_EDIT_MATRIX_4_3, m_4_3);
	DDX_Text(pDX, IDC_EDIT_MATRIX_4_4, m_4_4);
	DDX_Text(pDX, IDC_EDIT_MATRIX_1_1, m_1_1);
	DDX_Text(pDX, IDC_EDIT_MOVE_X, m_move_x);
	DDX_Text(pDX, IDC_EDIT_MOVE_Y, m_move_y);
	DDX_Text(pDX, IDC_EDIT_MOVE_Z, m_move_z);
	DDX_Text(pDX, IDC_EDIT_ROTATE_ANGLE, m_rotate_angle);
	DDX_Text(pDX, IDC_EDIT_ROTATE_X, m_rotate_axis_x);
	DDX_Text(pDX, IDC_EDIT_ROTATE_Y, m_rotate_axis_y);
	DDX_Text(pDX, IDC_EDIT_ROTATE_Z, m_rotate_axis_z);
	DDX_Text(pDX, IDC_EDIT_SCALE_X, m_scale_x);
	DDX_Text(pDX, IDC_EDIT_SCALE_Y, m_scale_y);
	DDX_Text(pDX, IDC_EDIT_SCALE_Z, m_scale_z);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgMatrix, CDialog)
	//{{AFX_MSG_MAP(CDlgMatrix)
	ON_EN_KILLFOCUS(IDC_EDIT_MOVE_Y, OnKillfocusEditMoveY)
	ON_EN_KILLFOCUS(IDC_EDIT_MOVE_Z, OnKillfocusEditMoveZ)
	ON_EN_KILLFOCUS(IDC_EDIT_ROTATE_ANGLE, OnKillfocusEditRotateAngle)
	ON_EN_KILLFOCUS(IDC_EDIT_ROTATE_X, OnKillfocusEditRotateX)
	ON_EN_KILLFOCUS(IDC_EDIT_ROTATE_Y, OnKillfocusEditRotateY)
	ON_EN_KILLFOCUS(IDC_EDIT_ROTATE_Z, OnKillfocusEditRotateZ)
	ON_EN_KILLFOCUS(IDC_EDIT_SCALE_X, OnKillfocusEditScaleX)
	ON_EN_KILLFOCUS(IDC_EDIT_SCALE_Y, OnKillfocusEditScaleY)
	ON_EN_KILLFOCUS(IDC_EDIT_SCALE_Z, OnKillfocusEditScaleZ)
	ON_EN_KILLFOCUS(IDC_EDIT_MOVE_X, OnKillfocusEditMoveX)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgMatrix メッセージ ハンドラ

void CDlgMatrix::Set(Node& trans)
{
	Calc();
}

void CDlgMatrix::Get(Node& trans)
{

}

void CDlgMatrix::Calc()
{
	matrix	M(4, 4), S(4, 4);
	rotate	R(m_rotate_axis_x, m_rotate_axis_y, m_rotate_axis_z, m_rotate_angle);

	S.set_data(1, 1, m_scale_x);
	S.set_data(2, 2, m_scale_y);
	S.set_data(3, 3, m_scale_z);

	M.set_data(1, 4, m_move_x);
	M.set_data(2, 4, m_move_y);
	M.set_data(3, 4, m_move_z);

	m_Matrix = M * R * S;

	m_1_1 = m_Matrix.get_data(1, 1);
	m_1_2 = m_Matrix.get_data(1, 2);
	m_1_3 = m_Matrix.get_data(1, 3);
	m_1_4 = m_Matrix.get_data(1, 4);

	m_2_1 = m_Matrix.get_data(2, 1);
	m_2_2 = m_Matrix.get_data(2, 2);
	m_2_3 = m_Matrix.get_data(2, 3);
	m_2_4 = m_Matrix.get_data(2, 4);

	m_3_1 = m_Matrix.get_data(3, 1);
	m_3_2 = m_Matrix.get_data(3, 2);
	m_3_3 = m_Matrix.get_data(3, 3);
	m_3_4 = m_Matrix.get_data(3, 4);

	m_4_1 = m_Matrix.get_data(4, 1);
	m_4_2 = m_Matrix.get_data(4, 2);
	m_4_3 = m_Matrix.get_data(4, 3);
	m_4_4 = m_Matrix.get_data(4, 4);
}



void CDlgMatrix::OnKillfocusEditMoveX()			{ UpdateData(); Calc(); UpdateData(FALSE); }
void CDlgMatrix::OnKillfocusEditMoveY()			{ UpdateData(); Calc(); UpdateData(FALSE); }
void CDlgMatrix::OnKillfocusEditMoveZ()			{ UpdateData(); Calc(); UpdateData(FALSE); }
void CDlgMatrix::OnKillfocusEditRotateAngle()	{ UpdateData(); Calc(); UpdateData(FALSE); }
void CDlgMatrix::OnKillfocusEditRotateX()		{ UpdateData(); Calc(); UpdateData(FALSE); }
void CDlgMatrix::OnKillfocusEditRotateY()		{ UpdateData(); Calc(); UpdateData(FALSE); }
void CDlgMatrix::OnKillfocusEditRotateZ()		{ UpdateData(); Calc(); UpdateData(FALSE); }
void CDlgMatrix::OnKillfocusEditScaleX()		{ UpdateData(); Calc(); UpdateData(FALSE); }
void CDlgMatrix::OnKillfocusEditScaleY()		{ UpdateData(); Calc(); UpdateData(FALSE); }
void CDlgMatrix::OnKillfocusEditScaleZ()		{ UpdateData(); Calc(); UpdateData(FALSE); }

