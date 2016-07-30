#pragma once

class	NurbsPrimitive;
struct	matrix;

class NurbsSphere : public NurbsPrimitive
{
protected:
	DECLARE_SERIAL(NurbsSphere)
	Vertex* getVertex(int a, int h) const;

	// ▽▽ 暫定 スプラインに変更する予定 ▽▽
	int		m_NumberOfFaces;
	Face**	m_pFaces;
	// △△ 暫定 スプラインに変更する予定 △△

public:
	// コンストラクタ
	NurbsSphere(Node* const root = 0, const TCHAR* const Name = _T("NurbsSphere"), const sp Color = sp(255, 255, 255));
	NurbsSphere(const NurbsSphere& other);
	void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& matrix) const;
};