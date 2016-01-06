// Viewport.h: Viewport クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWPORT_H__526166D6_44D0_4C72_AAE2_26BB886824C1__INCLUDED_)
#define AFX_VIEWPORT_H__526166D6_44D0_4C72_AAE2_26BB886824C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "plus.h"

class Viewport : public Plus  
{
friend class CRayTraceView;
public:
	Viewport();
	virtual ~Viewport();
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& matrix) const;
	void AttachRoot(const Node* pRoot);
	void DetachRoot();
	BOOL AddNode(CTreeCtrl& c, HTREEITEM SelectItem, Node* Target);
	void AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& matrix) const;
};

#endif // !defined(AFX_VIEWPORT_H__526166D6_44D0_4C72_AAE2_26BB886824C1__INCLUDED_)
