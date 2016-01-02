#pragma once
#include "node.h"
#include "Selectable.h"

class Point : public sp, Selectable
{
public:
	Point(const sp& = sp());
	bool GetInfo(const struct sp& K, const sp& L, Info& info, const Info* pHint, bool fromOutSide) const;
};

