// RayTrace.h : RAYTRACE �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//

#if !defined(AFX_RAYTRACE_H__B4EED835_53CC_11D3_9AB7_000000000000__INCLUDED_)
#define AFX_RAYTRACE_H__B4EED835_53CC_11D3_9AB7_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "..\resource.h"       // ���C�� �V���{��

/////////////////////////////////////////////////////////////////////////////
// CRayTraceApp:
// ���̃N���X�̓���̒�`�Ɋւ��Ă� RayTrace.cpp �t�@�C�����Q�Ƃ��Ă��������B
//

class CRayTraceApp : public CWinApp
{
public:
	CRayTraceApp();

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CRayTraceApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����

	//{{AFX_MSG(CRayTraceApp)
	afx_msg void OnAppAbout();
		// ���� - ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ��܂��͍폜���܂��B
		//        ���̈ʒu�ɐ��������R�[�h��ҏW���Ȃ��ł��������B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_RAYTRACE_H__B4EED835_53CC_11D3_9AB7_000000000000__INCLUDED_)
