#include "StdAfx.h"
#include "Selectable.h"


Selectable::Selectable()
{
}

Selectable::Selectable(const Selectable& other)
	: m_Scale(other.m_Scale), m_Rotate(other.m_Rotate), m_Move(other.m_Move), m_Pivot(other.m_Pivot)
{
	updateMatrix();
}

Selectable::~Selectable()
{
}

bool Selectable::GetInfo3(const sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide)
{
	matrix& m = m_Matrix;
	matrix& Inv_m = m.Inv();
	sp L2 = Inv_m * L;
	sp K2 = Inv_m * (K + L) - L2;
	return GetInfo(K2, L2, info, pHint, fromOutSide);
}

Selectable* Selectable::GetSelectable(const sp& K, const sp& L)
{
	Info	info;
	if (GetInfo3(K, L, info, NULL, true)) {
		return (Selectable*)(info.pNode ? info.pNode : info.pSelectable);
	}
	return NULL;
}

void Selectable::updateMatrix()
{
	m_Matrix = m_Move * m_Rotate * m_Scale;
}

void Selectable::Move(eAxis axis, float d)
{
	switch (axis) {
	case eX:	m_Move.set_data(3, 4, m_Move.get_data(3, 4) - (float)d / 20);	break;
	case eY:	m_Move.set_data(2, 4, m_Move.get_data(2, 4) - (float)d / 20);	break;
	case eZ:	m_Move.set_data(1, 4, m_Move.get_data(1, 4) - (float)d / 20);	break;
	}
	updateMatrix();
}

void Selectable::Move(POINT d)
{
	m_Move.set_data(1, 4, m_Move.get_data(1, 4) - (float)d.x / 20);
	m_Move.set_data(2, 4, m_Move.get_data(2, 4) - (float)d.y / 20);
	updateMatrix();
}

void Selectable::Rotate(eAxis axis, float d)
{
	switch (axis) {
	case eX:
		m_Rotate = m_Rotate * m_Pivot * rotate(0, 0, 1, d) * m_Pivot.Inv();
		break;
	case eY:
		m_Rotate = m_Rotate * m_Pivot * rotate(0, 1, 0, d) * m_Pivot.Inv();
		break;
	case eZ:
		m_Rotate = m_Rotate * m_Pivot * rotate(1, 0, 0, d) * m_Pivot.Inv();
		break;
	}
	updateMatrix();
}

void Selectable::Rotate(POINT d)
{
	rotate	r((float)-d.y, (float)-d.x, 0, sqrt((float)(d.x * d.x + d.y * d.y)) / 2);
	m_Rotate = r * m_Rotate;
	updateMatrix();
}


void Selectable::Scale(eAxis axis, float d)
{
	if (axis == eX || axis == eNONE)
		m_Scale.set_data(3, 3, m_Scale.get_data(3, 3) - d / 50);
	if (axis == eY || axis == eNONE)
		m_Scale.set_data(2, 2, m_Scale.get_data(2, 2) - d / 50);
	if (axis == eZ || axis == eNONE)
		m_Scale.set_data(1, 1, m_Scale.get_data(1, 1) - d / 50);
	updateMatrix();
}

void Selectable::MovePivot(eAxis axis, float d)
{
	switch (axis) {
	case eX:	m_Pivot.set_data(3, 4, m_Pivot.get_data(3, 4) - (float)d / 20);	break;
	case eY:	m_Pivot.set_data(2, 4, m_Pivot.get_data(2, 4) - (float)d / 20);	break;
	case eZ:	m_Pivot.set_data(1, 4, m_Pivot.get_data(1, 4) - (float)d / 20);	break;
	}
}

bool Selectable::getManipulatorMatrix(matrix& mat) const
{
	if (!m_Selected)
		return false;
	mat = mat * m_Matrix * m_Pivot;
	return true;
}

void Selectable::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {
		for (int i = 1; i < 4; i++) {
			ar << m_Scale.get_data(i, i);
			ar << m_Move.get_data(i, 4);
			for (int j = 1; j < 4; j++)
				ar << m_Rotate.get_data(i, j);
		}
	} else {
		for (int i = 1; i < 4; i++) {
			float value;
			ar >> value;
			m_Scale.set_data(i, i, value);
			ar >> value;
			m_Move.set_data(i, 4, value);
			for (int j = 1; j < 4; j++) {
				ar >> value;
				m_Rotate.set_data(i, j, value);
			}
		}
		m_Matrix = m_Move * m_Rotate * m_Scale;
	}
}

bool Selectable::ChangeSelection(const CRect* pRect, int cx, int cy, const matrix& mat)
{
	long ox, oy;
	sp(mat * m_Matrix * sp()).getPOINT(ox, oy, cx, cy);
	if (ox < pRect->left || pRect->right < ox || oy < pRect->top || pRect->bottom < oy)
		return false;
	m_Selected = !m_Selected;
	return true;
}

bool Selectable::ResetSelection()
{
	if (!m_Selected)
		return false;
	m_Selected = false;
	return true;
}
