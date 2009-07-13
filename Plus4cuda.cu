extern "C" __device__ 
Info4cuda GetInfo_Plus(Task task, sp4cuda K, sp4cuda L, Stack *stack)
{
	int idx = stack->Index;
	Info4cuda info;

return info;
	float	l = -1;
	for (int i = 0; i < task.data.gathering.member; i++) {
		int j = --idx;
		if (stack->data[j].valid) {
			if (l == -1 || stack->data[j].Distance < l) {
				StackData* sd = &(stack->data[j]);
				l = sd->Distance;
				info.valid		= sd->valid;
				info.Color_r	= sd->Color_r;
				info.Color_b	= sd->Color_g;
				info.Color_g	= sd->Color_b;
				info.isEnter	= sd->isEnter;		// ���荞��
				info.Distance	= sd->Distance;		// ��_�܂ł̋���
				info.Cross_x	= sd->Cross_x;		// ��_���Wx
				info.Cross_y	= sd->Cross_y;		// ��_���Wy
				info.Cross_z	= sd->Cross_z;		// ��_���Wz
				info.Vertical_x	= sd->Vertical_x;	// �@���x�N�g��x
				info.Vertical_y	= sd->Vertical_y;	// �@���x�N�g��y
				info.Vertical_z	= sd->Vertical_z;	// �@���x�N�g��z
				info.Refractive	= sd->Refractive;	// ���ܗ�
				info.nodeInfo	= sd->nodeInfo;		//
			}
		}
	}

	if (l < 0) {
		info.valid = false;
		return info;
	}

//	if (info.Material.Diffuse.r < 0)
	if (info.Color_r < 0) {
		info.Color_r = task.nodeInfo.m_Color_r;
		info.Color_g = task.nodeInfo.m_Color_g;
		info.Color_b = task.nodeInfo.m_Color_b;
	}
	info.valid = true;
	
	return info;
}
