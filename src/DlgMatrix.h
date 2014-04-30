// DlgMatrix.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMatrix ダイアログ

class CDlgMatrix : public CDialog
{
// コンストラクション
public:
	matrix	m_Matrix;
	void	Set( Node& );
	void	Get( Node& );
	void	Calc();
	CDlgMatrix(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDlgMatrix)
	enum { IDD = IDD_MATRIX };
	double	m_1_2;
	double	m_1_3;
	double	m_1_4;
	double	m_2_1;
	double	m_2_2;
	double	m_2_3;
	double	m_2_4;
	double	m_3_1;
	double	m_3_2;
	double	m_3_3;
	double	m_3_4;
	double	m_4_1;
	double	m_4_2;
	double	m_4_3;
	double	m_4_4;
	double	m_1_1;
	double	m_move_x;
	double	m_move_y;
	double	m_move_z;
	double	m_rotate_angle;
	double	m_rotate_axis_x;
	double	m_rotate_axis_y;
	double	m_rotate_axis_z;
	double	m_scale_x;
	double	m_scale_y;
	double	m_scale_z;
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
