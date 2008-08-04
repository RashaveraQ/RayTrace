#include "stdafx.h"
#include "RayTrace.h"
#include "DlgMatrix.h"
#include "DlgMaterial.h"
#include "RayTraceDoc.h"
#include "RayTraceView.h"

//IMPLEMENT_DYNAMIC( Node, CObject ) 

Node::Node(const Node& other) : m_Scale(4,4), m_Rotate(4,4), m_Move(4,4), m_Matrix(4,4)
{
	m_NodeType = other.m_NodeType;
	strcpy_s(m_Name, sizeof(m_Name), other.m_Name);
	m_Material = other.m_Material;
	m_Scale = other.m_Scale;
	m_Rotate = other.m_Rotate;
	m_Move = other.m_Move;
	m_Matrix = other.m_Matrix;
	m_Boundary = other.m_Boundary;
	m_Reflect = other.m_Reflect;
	m_Through = other.m_Through;
	m_Refractive = other.m_Refractive;
	m_TextureFileName = other.m_TextureFileName;
	MakeMemoryDCfromTextureFileName();
}

void Node::Serialize(CArchive& ar)
{
	int		i;

	if (ar.IsStoring())
	{
		ar << (WORD)m_NodeType;
		for (i = 0; i < 99; i++)
			ar << m_Name[i];

		ar << m_Material.Diffuse.r;
		ar << m_Material.Diffuse.g;
		ar << m_Material.Diffuse.b;
		ar << m_Material.Diffuse.a;
		ar << m_Material.Ambient.r;
		ar << m_Material.Ambient.g;
		ar << m_Material.Ambient.b;
		ar << m_Material.Ambient.a;
		ar << m_Material.Specular.r;
		ar << m_Material.Specular.g;
		ar << m_Material.Specular.b;
		ar << m_Material.Specular.a;
		ar << m_Material.Emissive.r;
		ar << m_Material.Emissive.g;
		ar << m_Material.Emissive.b;
		ar << m_Material.Emissive.a;
		ar << m_Material.Power;

		for (int i = 0; i < 3; i++) {
			ar << m_Scale.m_data[i][i];
			ar << m_Move.m_data[i][3];
			for (int j = 0; j < 3; j++ )
				ar << m_Rotate.m_data[i][j];
		}
		ar << m_Reflect;	ar << m_Through;	ar << m_Refractive;
	}
	else
	{
		for (i = 0; i < 99; i++)
			ar >> m_Name[i];

		ar >> m_Material.Diffuse.r;
		ar >> m_Material.Diffuse.g;
		ar >> m_Material.Diffuse.b;
		ar >> m_Material.Diffuse.a;
		ar >> m_Material.Ambient.r;
		ar >> m_Material.Ambient.g;
		ar >> m_Material.Ambient.b;
		ar >> m_Material.Ambient.a;
		ar >> m_Material.Specular.r;
		ar >> m_Material.Specular.g;
		ar >> m_Material.Specular.b;
		ar >> m_Material.Specular.a;
		ar >> m_Material.Emissive.r;
		ar >> m_Material.Emissive.g;
		ar >> m_Material.Emissive.b;
		ar >> m_Material.Emissive.a;
		ar >> m_Material.Power;

		for (int i = 0; i < 3; i++) {
			ar >> m_Scale.m_data[i][i];
			ar >> m_Move.m_data[i][3];
			for (int j = 0; j < 3; j++ )
				ar >> m_Rotate.m_data[i][j];
		}
		m_Matrix = m_Move * m_Rotate * m_Scale;
		ar >> m_Reflect;	ar >> m_Through;	ar >> m_Refractive;
	}
}

BOOL Node::Edit()
{
	return FALSE;
}

BOOL Node::EditColor()
{
	COLORREF	color = RGB(256 * m_Material.Diffuse.r, 256 * m_Material.Diffuse.g, 256 * m_Material.Diffuse.b);

	CColorDialog	color_dlg(color);

	if (color_dlg.DoModal() != IDOK)
		return FALSE;

	color = color_dlg.GetColor();
	m_Material.Diffuse.r = m_Material.Ambient.r = (float)(0xff & color) / 256;
	m_Material.Diffuse.g = m_Material.Ambient.g = (float)(0xff & color >> 8) / 256;
	m_Material.Diffuse.b = m_Material.Ambient.b = (float)(0xff & color >> 16) / 256;

	return TRUE;
}

BOOL Node::EditAfin()
{
	CDlgMatrix	dlg_matrix;

	dlg_matrix.Set(*this);

	if (dlg_matrix.DoModal() != IDOK)
		return FALSE;

	dlg_matrix.Get(*this);

	return TRUE;
}

BOOL Node::EditMaterial()
{
	CDlgMaterial dlg_material;

	dlg_material.Set(*this);

	if (dlg_material.DoModal() != IDOK)
		return FALSE;

	dlg_material.Get(*this);
	
	return TRUE;
}

BOOL Node::EditTexture()
{
	CFileDialog	dlg_file(TRUE);
	CString		b = m_TextureFileName;

	do {
		if (dlg_file.DoModal() != IDOK)
			return FALSE;

		m_TextureFileName = dlg_file.GetPathName();

		if (MakeMemoryDCfromTextureFileName())
			return TRUE;

		if (MessageBox(NULL, "ビットマップファイルの読み込みに失敗しました。", "テクスチャの選択エラー", MB_RETRYCANCEL|MB_ICONERROR) != IDRETRY)
			return FALSE;
	} while (1);

	m_TextureFileName = b;

	return FALSE;
}

BOOL Node::MakeMemoryDCfromTextureFileName()
{
	HBITMAP		h;
	BITMAP		b;
	CBitmap*	p;

	m_TextureDC.DeleteDC();
	m_TextureDC.CreateCompatibleDC(NULL);

	if (!(h = (HBITMAP)LoadImage(NULL, m_TextureFileName, IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE|LR_LOADFROMFILE)))
		return FALSE;

	(p = CBitmap::FromHandle(h))->GetObject(sizeof(BITMAP), &b);

	m_TextureSize.cx = b.bmWidth;
	m_TextureSize.cy = b.bmHeight;

	m_TextureDC.SelectObject(p);
	m_TextureDC.BitBlt(0, 0, m_TextureSize.cx, m_TextureSize.cy, &m_TextureDC, 0, 0, SRCCOPY);

	return TRUE;
}

sp Node::GetPixel(double x, double y) const
{
	COLORREF	c;

	if (m_TextureFileName.IsEmpty())
		return sp(256 * m_Material.Diffuse.r, 256 * m_Material.Diffuse.g, 256 * m_Material.Diffuse.b);

	c = m_TextureDC.GetPixel((int)(x * m_TextureSize.cx), (int)(y * m_TextureSize.cy));
	
	if (c == -1)
		return sp(256 * m_Material.Diffuse.r, 256 * m_Material.Diffuse.g, 256 * m_Material.Diffuse.b);

	return sp(GetRValue(c),GetGValue(c),GetBValue(c));
}

BOOL Node::GetInfo2(const sp& K, const sp& L, Info& info, const Node* pOmit, const Node& viewport) const
{
	// START Boundary 
/*
	double a = gK * gK;
	double b = (gL - m_Boundary.Center) * gK;
	double c = (m_Boundary.Center - gL) * (m_Boundary.Center - gL) - m_Boundary.Radius * m_Boundary.Radius;
	double bb_ac = b*b-a*c;

	if (bb_ac < 0)
		return FALSE;

	double t1, t2;

	t1 = (-b+sqrt(bb_ac))/a;
	t2 = (-b-sqrt(bb_ac))/a;

	if (t1 <= 0 || t2 <= 0)
		return FALSE;
	// End Boundary
*/
	matrix m = m_Move * m_Rotate * m_Scale;
	matrix Inv_m = m.Inv();

	if (!GetInfo(Inv_m * matrix(K,0), Inv_m * L, info, pOmit, viewport)) {
		return FALSE;
	}

	info.Cross = m * info.Cross;
	info.Vertical = m * matrix(info.Vertical,0);
	info.Distance = (info.Cross - L).abs();
//	info->Refractive = info->pNode->m_Refractive / ((pOmit) ? pOmit->m_Refractive : 1);

	sp k = K.e();
	sp v = info.Vertical.e();

	if (m_Reflect > 0) {
		sp c = viewport.GetColor(k-2*(k*v)*v, info.Cross, this, viewport);
		info.Material = (m_Reflect * c + (1 - m_Reflect) * sp(info.Material)).getMaterial();
	}

	if (m_Through > 0) {
		double r = m_Refractive;
		sp c = viewport.GetColor((k+v)/r-v, info.Cross, this, viewport);
		info.Material = (m_Through * c + (1 - m_Through) * sp(info.Material)).getMaterial();
	}

	return TRUE;
}

sp Node::GetColor(const sp& K, const sp& L, const Node* pOmit, const Node& viewport) const
{
	Info	info;

	if (!GetInfo2(K, L, info, pOmit, viewport))
		return sp(127, 127, 127);

	double	x = -m_pDoc->m_Light.e() * info.Vertical.e();

	x = (x > 0.0) ? x : 0.0;

	double t = 64 + 191 * sin(M_PI / 2 * x);
	double b = 191 * (1 - cos(M_PI / 2 * x));

	return (t - b) * sp(info.Material) / 255 + sp(b,b,b);
}

void Node::Move(eAxis axis, double d)
{
	switch (axis) {
	case eX:	m_Move.m_data[2][3] -= (double)d / 20;	break;
	case eY:	m_Move.m_data[1][3] -= (double)d / 20;	break;
	case eZ:	m_Move.m_data[0][3] -= (double)d / 20;	break;
	}
	updateMatrix();
}

void Node::Move(POINT d)
{
	m_Move.m_data[0][3] -= (double)d.x / 20;
	m_Move.m_data[1][3] -= (double)d.y / 20;
	updateMatrix();
}

void Node::Rotate(eAxis axis, double d)
{
	switch (axis) {
	case eX:
		m_Rotate = m_Rotate * m_Pivot * rotate(0,0,1,d) * m_Pivot.Inv();
		break;
	case eY:
		m_Rotate = m_Rotate * m_Pivot * rotate(0,1,0,d) * m_Pivot.Inv();
		break;
	case eZ:
		m_Rotate = m_Rotate * m_Pivot * rotate(1,0,0,d) * m_Pivot.Inv();
		break;
	}
	updateMatrix();
}

void Node::Rotate(POINT d)
{
	rotate	r(-d.y, -d.x, 0, sqrt((double)d.x*d.x+d.y*d.y) / 2);
	m_Rotate = r * m_Rotate;
	updateMatrix();
}

void Node::Scale(eAxis axis, double d)
{
	if (axis == eX || axis == eNONE)
		m_Scale.m_data[2][2] -= d / 50;
	if (axis == eY || axis == eNONE)
		m_Scale.m_data[1][1] -= d / 50;
	if (axis == eZ || axis == eNONE)
		m_Scale.m_data[0][0] -= d / 50;
	updateMatrix();
}

void Node::MovePivot(eAxis axis, double d)
{
	switch (axis) {
	case eX:	m_Pivot.m_data[2][3] -= (double)d / 20;	break;
	case eY:	m_Pivot.m_data[1][3] -= (double)d / 20;	break;
	case eZ:	m_Pivot.m_data[0][3] -= (double)d / 20;	break;
	}
}

bool Node::SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& Matrix) const
{
	if (rtv.m_SelectedNode != this || rtv.m_Manipulator.Type == eSELECT)
		return false;

	const CSize& size = rtv.m_ClientSize;
	const Viewport& viewport = rtv.m_Viewport;

	matrix m = Matrix * m_Matrix;

	sp	p0 = m * m_Pivot * sp( 0, 0, 0);
	sp	px, py, pz;

	int r = 100;
	
	switch (rtv.m_Manipulator.Type) {
	case ePIVOT_MOVE:
		r = 50;
	case eMOVE:
		px = p0 + viewport.m_Rotate * sp( 0,  0, -1);
		py = p0 + viewport.m_Rotate * sp( 0, -1,  0);
		pz = p0 + viewport.m_Rotate * sp(-1,  0,  0);
		break;
	default:
		px = m * m_Pivot * sp( 0,  0, -1);
		py = m * m_Pivot * sp( 0, -1,  0);
		pz = m * m_Pivot * sp(-1,  0,  0);
	}

/*
	POINT	P0 = { (long)((p0.x - view.left) * size.cx / (view.right - view.left)),
				   (long)((p0.y - view.top) * size.cx / (view.bottom - view.top)) },
			PX = { (long)(P0.x + r * (px - p0).e().x), (long)(P0.y + r * (px - p0).e().y)}, 
			PY = { (long)(P0.x + r * (py - p0).e().x), (long)(P0.y + r * (py - p0).e().y)}, 
			PZ = { (long)(P0.x + r * (pz - p0).e().x), (long)(P0.y + r * (pz - p0).e().y)};
*/
	POINT	P0 = p0.getPOINT(size),
			PX = px.getPOINT(size),
			PY = py.getPOINT(size),
			PZ = pz.getPOINT(size);

	switch (rtv.m_Manipulator.Type)
	{
	// case eMOVE:
		// point が P0-PX 線上の場合は、rtv.m_Manipulator.Axis = eX;
		// point が P0-PY 線上の場合は、rtv.m_Manipulator.Axis = eY;
		// point が P0-PZ 線上の場合は、rtv.m_Manipulator.Axis = eZ;
		// break;
	case eROTATE:
		{
			eAxis tbl[] = {eX, eZ, eY};
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 50; j++) {
					double th = (i == 2) ? (6.28 * (double)j / 50) : (3.14 * (double)i / 2);
					double ph = (i == 2) ? (3.14 / 2) : (6.28 * (double)j / 50);
					sp p = m * sp(2 * cos(th)*sin(ph), 2 * cos(ph), 2 * sin(th)*sin(ph));
					double dx = (P0.x + (r - 10) * (p - p0).e().x) - point.x;
					double dy = (P0.y + (r - 10) * (p - p0).e().y) - point.y;
					if (dx*dx+dy*dy < 20) {
						rtv.m_Manipulator.Axis = tbl[i];
						return true;
					}
				}
			}
		}

		return false;
		break;
	case eMOVE: // temp
	case ePIVOT_MOVE: // temp
	case eSCALE:
		if (abs(PX.x - point.x) + abs(PX.y - point.y) <= 20)
			rtv.m_Manipulator.Axis = eX;
		else if (abs(PY.x - point.x) + abs(PY.y - point.y) <= 20)
			rtv.m_Manipulator.Axis = eY;
		else if (abs(PZ.x - point.x) + abs(PZ.y - point.y) <= 20)
			rtv.m_Manipulator.Axis = eZ;
		else if (abs(P0.x - point.x) + abs(P0.y - point.y) <= 20)
			rtv.m_Manipulator.Axis = eNONE;
		else
			return false;
		break;
	default:
		return false;
	}
	return true;
}

bool InitVertexBuffer(LPDIRECT3DDEVICE9 pd3dDevice, LPDIRECT3DVERTEXBUFFER9& pVB, CUSTOMVERTEX*& pVertices, UINT count)
{
	if (FAILED(pd3dDevice->CreateVertexBuffer(
								count * sizeof(CUSTOMVERTEX), 0,
								D3DFVF_CUSTOMVERTEX, D3DPOOL_DEFAULT,
								&pVB, NULL))) {
		AfxMessageBox("Failed to CreateVertexBuffer");
		return false;
	}

	if (FAILED(pVB->Lock(0, 0, (void**)&pVertices, 0))) {
		AfxMessageBox("Failed to Lock");
		return false;
	}

	return true;
}

void Node::AddGeometry(LPDIRECT3DDEVICE9 pd3dDevice, CListGeometry& lstGeometry, CRayTraceView& rtv, const matrix& m) const
{
	if (rtv.m_SelectedNode != this || rtv.m_Manipulator.Type == eSELECT)
		return;

	const Viewport& viewport = rtv.m_Viewport;
	sp	p0 = m * m_Pivot * sp(0, 0, 0);
	sp	px, py, pz;

	int r = 100;
	switch (rtv.m_Manipulator.Type) {
	case ePIVOT_MOVE:
		r = 50;
	case eMOVE:
		px = p0 + viewport.m_Rotate * sp( 0,  0, -r);
		py = p0 + viewport.m_Rotate * sp( 0, -r,  0);
		pz = p0 + viewport.m_Rotate * sp(-r,  0,  0);
		break;
	default:
		px = m * m_Pivot * sp( 0,  0, -r);
		py = m * m_Pivot * sp( 0, -r,  0);
		pz = m * m_Pivot * sp(-r,  0,  0);
	}

	LPDIRECT3DVERTEXBUFFER9 pVB;
	CUSTOMVERTEX*	pVertices;

	switch (rtv.m_Manipulator.Type)
	{
	case eMOVE:
	case eSCALE:
	case ePIVOT_MOVE:
		if (!InitVertexBuffer(pd3dDevice, pVB, pVertices, 6))
			return;
		pVertices[0].position = pVertices[0].normal = D3DXVECTOR3((float)p0.x, (float)p0.y, (float)p0.z);
		pVertices[1].position = pVertices[1].normal = D3DXVECTOR3((float)px.x, (float)px.y, (float)px.z);
		pVertices[2].position = pVertices[2].normal = D3DXVECTOR3((float)p0.x, (float)p0.y, (float)p0.z);
		pVertices[3].position = pVertices[3].normal = D3DXVECTOR3((float)py.x, (float)py.y, (float)py.z);
		pVertices[4].position = pVertices[4].normal = D3DXVECTOR3((float)p0.x, (float)p0.y, (float)p0.z);
		pVertices[5].position = pVertices[5].normal = D3DXVECTOR3((float)pz.x, (float)pz.y, (float)pz.z);
		pVB->Unlock();
		lstGeometry.AddTail(Geometry(this, pVB, D3DPT_LINELIST, 6));
		break;
	case eROTATE:
		if (!InitVertexBuffer(pd3dDevice, pVB, pVertices, 150))
			return;
		{
			double th, ph;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 50; j++) {
					th = (i == 2) ? (6.28 * (double)j / 50) : (3.14 * (double)i / 2);
					ph = (i == 2) ? (3.14 / 2) : (6.28 * (double)j / 50);
					sp p = m * sp(2 * cos(th)*sin(ph), 2 * cos(ph), 2 * sin(th)*sin(ph));
					pVertices[50*i+j].position = pVertices[50*i+j].normal = D3DXVECTOR3((float)p.x, (float)p.y, (float)p.z);
				}
			}
		}
		pVB->Unlock();
		lstGeometry.AddTail(Geometry(this, pVB, D3DPT_LINESTRIP, 150-1));
		break;
	default:
		return;
	}
}

void Node::Draw_Outline(CDC* pDC, CRayTraceView& rtv, const matrix& m) const
{
	if (rtv.m_SelectedNode != this || rtv.m_Manipulator.Type == eSELECT)
		return;

	static COLORREF s_color = RGB(255,255,0);

	CPen	x_pen(PS_SOLID, 1, (rtv.m_Manipulator.Axis == eX) ? s_color : RGB(255,0,0)),
			y_pen(PS_SOLID, 1, (rtv.m_Manipulator.Axis == eY) ? s_color : RGB(0,255,0)),
			z_pen(PS_SOLID, 1, (rtv.m_Manipulator.Axis == eZ) ? s_color : RGB(0,0,255)),
			c_pen(PS_SOLID, 1, (rtv.m_Manipulator.Axis == eNONE) ? s_color : RGB(0,255,255)),
			g_pen(PS_SOLID, 1, RGB(127, 127, 127));

	const CSize& size = rtv.m_ClientSize;
	const Viewport& viewport = rtv.m_Viewport;

	sp	p0 = m * m_Pivot * sp(0, 0, 0);
	sp	px, py, pz;

	int r = 100;
	switch (rtv.m_Manipulator.Type) {
	case ePIVOT_MOVE:
		r = 50;
	case eMOVE:
		px = p0 + viewport.m_Rotate * sp( 0,  0, -1);
		py = p0 + viewport.m_Rotate * sp( 0, -1,  0);
		pz = p0 + viewport.m_Rotate * sp(-1,  0,  0);
		break;
	default:
		px = m * m_Pivot * sp( 0,  0, -1);
		py = m * m_Pivot * sp( 0, -1,  0);
		pz = m * m_Pivot * sp(-1,  0,  0);
	}

	POINT	P0 = p0.getPOINT(size),
			PX = { (long)(P0.x + r * (px - p0).e().x), (long)(P0.y + r * (px - p0).e().y)}, 
			PY = { (long)(P0.x + r * (py - p0).e().x), (long)(P0.y + r * (py - p0).e().y)}, 
			PZ = { (long)(P0.x + r * (pz - p0).e().x), (long)(P0.y + r * (pz - p0).e().y)};

	// X-Axis
	CPen *old_pen;
	CPen *pens[] = {&x_pen, &z_pen, &y_pen};
	
	switch (rtv.m_Manipulator.Type)
	{
	case eMOVE:
	case ePIVOT_MOVE:
		old_pen = 
		pDC->SelectObject(&z_pen);
		pDC->MoveTo(P0);
		pDC->LineTo(PZ);
		pDC->SelectObject(&y_pen);
		pDC->MoveTo(P0);
		pDC->LineTo(PY);
		pDC->SelectObject(&x_pen);
		pDC->MoveTo(P0);
		pDC->LineTo(PX);
		pDC->SelectObject(&c_pen);
		pDC->Rectangle(P0.x - 10, P0.y - 10, P0.x + 10, P0.y + 10);
		break;
	case eROTATE:
		old_pen = 
		pDC->SelectObject(&g_pen);
		
		int i, j;
		double th, ph;
		POINT	P[50];

		for (i = 0; i < 3; i++) {
			for (j = 0; j < 50; j++) {
				th = (i == 2) ? (6.28 * (double)j / 50) : (3.14 * (double)i / 2);
				ph = (i == 2) ? (3.14 / 2) : (6.28 * (double)j / 50);
				sp p = m * sp(2 * cos(th)*sin(ph), 2 * cos(ph), 2 * sin(th)*sin(ph));
				P[j].x = (long)(P0.x + (r - 10) * (p - p0).e().x);
				P[j].y = (long)(P0.y + (r - 10) * (p - p0).e().y);
			}
			pDC->SelectObject(pens[i]);
			pDC->Polygon(P, 50);
		}
		pDC->SelectObject(&c_pen);
		pDC->Ellipse(P0.x - r, P0.y - r, P0.x + r, P0.y + r);
		break;

	case eSCALE:
		old_pen = 
		pDC->SelectObject(&g_pen);
		pDC->MoveTo(PX);
		pDC->LineTo(P0);
		pDC->LineTo(PY);
		pDC->MoveTo(P0);
		pDC->LineTo(PZ);
		pDC->SelectObject(&x_pen);
		pDC->Ellipse(PX.x - 5, PX.y - 5, PX.x + 5, PX.y + 5);
		pDC->SelectObject(&y_pen);
		pDC->Ellipse(PY.x - 5, PY.y - 5, PY.x + 5, PY.y + 5);
		pDC->SelectObject(&z_pen);
		pDC->Ellipse(PZ.x - 5, PZ.y - 5, PZ.x + 5, PZ.y + 5);
		pDC->SelectObject(&c_pen);
		pDC->Ellipse(P0.x - 5, P0.y - 5, P0.x + 5, P0.y + 5);
		break;
	default:
		return;
	}
	pDC->SelectObject(old_pen);
}

