#pragma once
class Selectable
{
protected:
	bool		m_Selected;
	matrix		m_Scale;	// スケール変換
	matrix		m_Rotate;	// 回転
	matrix		m_Move;		// 平行移動
	matrix		m_Pivot;	// マニュピレータの中心点
	matrix		m_Matrix;
	virtual void updateMatrix();

public:
	Selectable();
	Selectable(const Selectable& other);
	~Selectable();
	virtual void Draw_Outline(CDC* pDC, CRayTraceView& raytraceview, const matrix& m) const;
	virtual bool SetManipulatorAxis(CRayTraceView& rtv, CPoint point, const matrix& m) const;
	Selectable* GetSelectable(const sp& K, const sp& L);
	virtual	bool GetInfo(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const = 0;
	void Move(eAxis axis, float d);
	void Move(POINT d);
	void Rotate(eAxis axis, float d);
	void Rotate(POINT d);
	void Scale(eAxis axis, float d);
	void MovePivot(eAxis axis, float d);
	bool GetInfo3(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide);
	virtual	void Serialize(CArchive& ar);
	virtual bool OnObjectMode() { return false; }
	virtual bool ChangeSelection(const CRect* pRect, int cx, int cy, const matrix& mat);
	virtual bool ResetSelection();
};

