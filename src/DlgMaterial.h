#if !defined(AFX_DLGMATERIAL_H__60E64592_D3DA_11D4_BE9E_00104B17BB23__INCLUDED_)
#define AFX_DLGMATERIAL_H__60E64592_D3DA_11D4_BE9E_00104B17BB23__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgMaterial.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDlgMaterial �_�C�A���O

class CDlgMaterial : public CDialog
{
// �R���X�g���N�V����
public:
	CDlgMaterial(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	void Set( Node& );
	void Get( Node& );
	
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgMaterial)
	enum { IDD = IDD_MATERIAL };
	double	m_Reflect;
	double	m_Through;
	double	m_Refractive;
	int		m_Material;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDlgMaterial)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgMaterial)
	afx_msg void OnSelchangeMaterial();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGMATERIAL_H__60E64592_D3DA_11D4_BE9E_00104B17BB23__INCLUDED_)
