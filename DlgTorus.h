#if !defined(AFX_DLGTORUS_H__6073BB82_DB16_11D4_BEAD_00104B17BB23__INCLUDED_)
#define AFX_DLGTORUS_H__6073BB82_DB16_11D4_BEAD_00104B17BB23__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgTorus.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTorus �_�C�A���O

class CDlgTorus : public CDialog
{
// �R���X�g���N�V����
public:
	CDlgTorus(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDlgTorus)
	enum { IDD = IDD_TORUS };
	float	m_R;
	float	m_r;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDlgTorus)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDlgTorus)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGTORUS_H__6073BB82_DB16_11D4_BEAD_00104B17BB23__INCLUDED_)
