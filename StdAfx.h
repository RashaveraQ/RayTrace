// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#if !defined(AFX_STDAFX_H__B4EED837_53CC_11D3_9AB7_000000000000__INCLUDED_)
#define AFX_STDAFX_H__B4EED837_53CC_11D3_9AB7_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Windows �w�b�_�[����w�ǎg�p����Ȃ��X�^�b�t�����O���܂��B


#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������
#include <afxdisp.h>        // MFC �� OLE �I�[�g���[�V���� �N���X
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcview.h>
#include <afxtempl.h>

#include <mmsystem.h>
#include <d3dx9.h>

struct CUSTOMVERTEX
{
    D3DXVECTOR3 position; // The 3D position for the vertex
    D3DXVECTOR3 normal;   // The surface normal for the vertex
};

// Our custom FVF, which describes our custom vertex structure
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZ|D3DFVF_NORMAL)

#include <stdio.h>
#include <math.h>
#ifndef M_PI
#define M_PI (4.0*atan(1.0))
#endif

#include "geometry.h"

typedef CList<Geometry,Geometry&> CListGeometry;

bool InitVertexBuffer(LPDIRECT3DDEVICE9 pd3dDevice, LPDIRECT3DVERTEXBUFFER9& pVB, CUSTOMVERTEX*& pVertices, UINT count);

#include "move.h"
#include "expand.h"
#include "rotate_x.h"
#include "rotate_y.h"
#include "rotate_z.h"
#include "rotate.h"
#include "sp.h"
#include "matrix.h"
#include "info.h"
#include "node.h"
#include "sphere.h"
#include "cone.h"
#include "cylinder.h"
#include "plane.h"
#include "gathering.h"
#include "plus.h"
#include "Viewport.h"
#include "minus.h"
#include "multiple.h"
#include "cube.h"
#include "Torus.h"
#include "Polygon2.h"
#include "Teapot.h"
#include "RayTraceView.h"

//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_STDAFX_H__B4EED837_53CC_11D3_9AB7_000000000000__INCLUDED_)
