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
				info.isEnter	= sd->isEnter;		// 入り込む
				info.Distance	= sd->Distance;		// 交点までの距離
				info.Cross_x	= sd->Cross_x;		// 交点座標x
				info.Cross_y	= sd->Cross_y;		// 交点座標y
				info.Cross_z	= sd->Cross_z;		// 交点座標z
				info.Vertical_x	= sd->Vertical_x;	// 法線ベクトルx
				info.Vertical_y	= sd->Vertical_y;	// 法線ベクトルy
				info.Vertical_z	= sd->Vertical_z;	// 法線ベクトルz
				info.Refractive	= sd->Refractive;	// 屈折率
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
