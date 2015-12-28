// DlgMatrix.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMatrix ダイアログ

class CDlgMatrix : public CDialog
{
	// コンストラクション
public:
	matrix	m_Matrix;
	void	Set(Node&);
	void	Get(Node&);
	void	Calc();
	CDlgMatrix(CWnd* pParent = NULL);   // 標準のコンストラクタ

	// ダイアログ データ
	//{{AFX_DATA(CDlgMatrix)
	enum { IDD = IDD_MATRIX };
	float	m_1_2;
	float	m_1_3;
	float	m_1_4;
	float	m_2_1;
	float	m_2_2;
	float	m_2_3;
	float	m_2_4;
	float	m_3_1;
	float	m_3_2;
	float	m_3_3;
	float	m_3_4;
	float	m_4_1;
	float	m_4_2;
	float	m_4_3;
	float	m_4_4;
	float	m_1_1;
	float	m_move_x;
	float	m_move_y;
	float	m_move_z;
	float	m_rotate_angle;
	float	m_rotate_axis_x;
	float	m_rotate_axis_y;
	float	m_rotate_axis_z;
	float	m_scale_x;
	float	m_scale_y;
	float	m_scale_z;
	//}}AFX_DATA


	// オーバーライド
	// ClassWizard は仮想関数を生成しオーバーライドします。
	//{{AFX_VIRTUAL(CDlgMatrix)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

	// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDlgMatrix)
	afx_msg void OnKillfocusEditMoveY();
	afx_msg void OnKillfocusEditMoveZ();
	afx_msg void OnKillfocusEditRotateAngle();
	afx_msg void OnKillfocusEditRotateX();
	afx_msg void OnKillfocusEditRotateY();
	afx_msg void OnKillfocusEditRotateZ();
	afx_msg void OnKillfocusEditScaleX();
	afx_msg void OnKillfocusEditScaleY();
	afx_msg void OnKillfocusEditScaleZ();
	afx_msg void OnKillfocusEditMoveX();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
