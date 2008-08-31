// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���A
//            �܂��͎Q�Ɖ񐔂������A�����܂�ύX����Ȃ�
//            �v���W�F�N�g��p�̃C���N���[�h �t�@�C�����L�q���܂��B
//

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows XP or later.
#define WINVER 0x0501		// Change this to the appropriate value to target other versions of Windows.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.                   
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 6.0 or later.
#define _WIN32_IE 0x0600	// Change this to the appropriate value to target other versions of IE.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

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

#define TARGET

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

#include <cuda_runtime.h>

//#define _CRTDBG_MAP_ALLOC
//#include <crtdbg.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

