extern "C" __device__ 
void GetInfo_Plus(Task task, sp4cuda K, sp4cuda L, Info4cuda* info, Stack *stack)
{
	double	l = -1;
	for (int i = 0; i < task.data.gathering.member; i++) {
		int j = --(stack->Index);
		if (stack->valid[j]) {
			if (l == -1 || stack->Distance[j] < l) {
				l = stack->Distance[j];
				info->valid			= stack->valid[j];
				info->Color_r		= stack->Color_r[j];
				info->Color_b		= stack->Color_g[j];
				info->Color_g		= stack->Color_b[j];
				info->isEnter		= stack->isEnter[j];	// 入り込む
				info->Distance		= stack->Distance[j];	// 交点までの距離
				info->Cross_x		= stack->Cross_x[j];		// 交点座標x
				info->Cross_y		= stack->Cross_y[j];		// 交点座標y
				info->Cross_z		= stack->Cross_z[j];		// 交点座標z
				info->Vertical_x	= stack->Vertical_x[j];	// 法線ベクトルx
				info->Vertical_y	= stack->Vertical_y[j];	// 法線ベクトルy
				info->Vertical_z	= stack->Vertical_z[j];	// 法線ベクトルz
				info->Refractive	= stack->Refractive[j];	// 屈折率
				info->nodeInfo		= stack->nodeInfo[j];	//
			}
		}
	}

	if (l < 0) {
		info->valid = false;
		return;
	}

//	if (info.Material.Diffuse.r < 0)
	if (info->Color_r < 0) {
		info->Color_r = task.nodeInfo.m_Color_r;
		info->Color_g = task.nodeInfo.m_Color_g;
		info->Color_b = task.nodeInfo.m_Color_b;
	}
	info->valid = true;
}
