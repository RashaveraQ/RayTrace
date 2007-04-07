// stdafx.h : 標準のシステム インクルード ファイル、
//            または参照回数が多く、かつあまり変更されない
//            プロジェクト専用のインクルード ファイルを記述します。
//

#if !defined(AFX_STDAFX_H__B4EED837_53CC_11D3_9AB7_000000000000__INCLUDED_)
#define AFX_STDAFX_H__B4EED837_53CC_11D3_9AB7_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#define VC_EXTRALEAN		// Windows ヘッダーから殆ど使用されないスタッフを除外します。


#include <afxwin.h>         // MFC のコアおよび標準コンポーネント
#include <afxext.h>         // MFC の拡張部分
#include <afxdisp.h>        // MFC の OLE オートメーション クラス
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC の Windows コモン コントロール サポート
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
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_STDAFX_H__B4EED837_53CC_11D3_9AB7_000000000000__INCLUDED_)
