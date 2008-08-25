
#ifndef M_PI
#define M_PI (4.0*atan(1.0))
#endif

TARGET sp NodeBase::GetPixel(double x, double y) const
{
	COLORREF	c = -1;

	if (!m_TextureFileName)
		return sp(256 * m_Material.Diffuse.r, 256 * m_Material.Diffuse.g, 256 * m_Material.Diffuse.b);
#if 0
	::GetPixel(m_hTextureDC, (int)(x * m_TextureSize.cx), (int)(y * m_TextureSize.cy));
#endif

//	CDC	dc;
//	dc.Attach(m_hTextureDC);
//	c = dc.GetPixel((int)(x * m_TextureSize.cx), (int)(y * m_TextureSize.cy));
//	dc.Detach();
	
	if (c == -1)
		return sp(256 * m_Material.Diffuse.r, 256 * m_Material.Diffuse.g, 256 * m_Material.Diffuse.b);

	return sp(GetRValue(c),GetGValue(c),GetBValue(c));
}

// 視線ベクトル(Kt+L)と交差する物体の情報infoを返す。
// 戻り値:true 交差あり,false 交差なし
TARGET BOOL NodeBase::GetInfo2(const sp& K, const sp& L, Info& info) const
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

	sp L2 = Inv_m * L;
	sp K2 = Inv_m * (K + L) - L2;

	if (!GetInfo(K2, L2, info)) {
		return FALSE;
	}

	info.Vertical = m * (info.Vertical + info.Cross) - m * info.Cross;
	info.Cross = m * info.Cross;
	info.Distance = (info.Cross - L).abs();
	info.Refractive = info.pNode->m_Refractive;
	if (info.isEnter)
		info.Refractive = 1 / info.Refractive;

	return TRUE;
}

// 視線ベクトル(Kt+L)から色を返す。
TARGET sp NodeBase::GetColor(NodeBase* Root, const sp& light, const sp& K, const sp& L, int nest) const
{
	Info	info;

	// 再帰数が１０を越える又は、交点が存在しない場合、
	if (nest > 10 || !GetInfo2(K, L, info))
		return sp(127, 127, 127);

	sp k = K.e();
	sp v = info.Vertical.e();

	// 反射率がある場合、
	if (info.pNode->m_Reflect > 0) {
		sp k2 = k - 2 * (v * k) * v;
		sp l2 = info.Cross + 1E-05 * k2;
		// 反射した視線ベクトルから色を取得。
//		sp c = m_pDoc->m_Root.GetColor(k2, l2, nest + 1);
		sp c = Root->GetColor(Root, light, k2, l2, nest + 1);
		// 反射率で色を混ぜる。
		info.Material = (info.pNode->m_Reflect * c + (1 - info.pNode->m_Reflect) * sp(info.Material)).getMaterial();
	}

	// 透過率がある場合、
	if (info.pNode->m_Through > 0) {
		double r = info.Refractive;
		double i = k * v;
		sp k2 = r * (k -i * v - sqrt(r * r - 1.0 + i * i) * v);
		sp l2 = info.Cross + 1E-05 * k2;
		// 屈折した視線ベクトルから色を取得。
//		sp c = m_pDoc->m_Root.GetColor(k2, l2, nest + 1);
		sp c = Root->GetColor(Root, light, k2, l2, nest + 1);
		// 透過率で色を混ぜる。
		info.Material = (info.pNode->m_Through * c + (1 - info.pNode->m_Through) * sp(info.Material)).getMaterial();
	}

	// 光源より色を補正。
//	double	x = -m_pDoc->m_Light.e() * info.Vertical.e();
	double	x = -light.e() * info.Vertical.e();
	x = (x > 0.0) ? x : 0.0;
	double t = 64 + 191 * sin(M_PI / 2 * x);
	double b = 191 * (1 - cos(M_PI / 2 * x));

	return (t - b) * sp(info.Material) / 255 + sp(b,b,b);
}


