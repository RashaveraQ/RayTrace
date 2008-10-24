__device__
void GetInfo_Plus(const Task& task, const sp4cuda& K, const sp4cuda& L, Info4cuda& info, Stack& stack)
{
	double	l = -1;
	for (int i = 0; i < task.data.gathering.member; i++) {
		int j = --stack.Index;
		if (stack.valid[j]) {
			if (l == -1 || stack.Distance[j] < l) {
				l = stack.Distance[j];
				info.valid		= stack.valid	[j];
				info.Material	= stack.Material[j];
				info.isEnter	= stack.isEnter	[j];	// ���荞��
				info.Distance	= stack.Distance[j];	// ��_�܂ł̋���
				info.Cross_x	= stack.Cross_x[j];		// ��_���Wx
				info.Cross_y	= stack.Cross_y[j];		// ��_���Wy
				info.Cross_z	= stack.Cross_z[j];		// ��_���Wz
				info.Vertical_x	= stack.Vertical_x[j];	// �@���x�N�g��x
				info.Vertical_y	= stack.Vertical_y[j];	// �@���x�N�g��y
				info.Vertical_z	= stack.Vertical_z[j];	// �@���x�N�g��z
				info.Refractive	= stack.Refractive[j];	// ���ܗ�
				info.nodeInfo	= stack.nodeInfo[j];	//
			}
		}
	}

	if (l < 0) {
		info.valid = false;
		return;
	}

	if (info.Material.Diffuse.r < 0)
		info.Material = task.nodeInfo.m_Material;
	info.valid = true;
}
