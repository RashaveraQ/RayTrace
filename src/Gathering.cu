#include <math.h>
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "DoCuda.h"
#include "Gathering.cuh"

__device__
DevGathering::DevGathering(const DevGathering& other) : DevNode(other), m_Member(other.m_Member)
{
	for (int i = 0; i < m_Member; i++)
		m_Node[i] = (class DevNode*)other.m_Node[i]->MakeCopy();
}

__device__
DevGathering::~DevGathering()
{
	for (short i = 0; i < m_Member; i++)
		delete m_Node[i];
}

__device__
bool DevGathering::Delete(DevNode* pNode)
{
	short	i, j;

	for (i = 0; i < m_Member; i++) {
		if (m_Node[i] == pNode) {
			delete pNode;
			m_Member--;
			for (j = i; j < m_Member; j++)
				m_Node[j] = m_Node[j + 1];
			return true;
		}
		else if (m_Node[i]->Delete(pNode))
			return true;
	}
	return false;
}

/*
__device__
BOOL DevGathering::AddNode(CTreeCtrl& c, HTREEITEM hitem, DevNode* pNode)
{
	if (m_Member >= 1000)
		return FALSE;

	m_Node[m_Member++] = pNode;

	pNode->InsertItem(c, hitem);

	//pNode->updateMatrix(this);

	pNode->SetParent(this);

	return TRUE;
}
*/
__device__
bool DevGathering::AddNode(DevNode** node)
{
	if (m_Member >= 1000)
		return false;

	m_Node[m_Member++] = *node;

	(*node)->SetParent(this);

	return true;
}

__device__
void DevGathering::SetRoot(DevNode** const root)
{
	m_Root = root;

	for (int i = 0; i < m_Member; i++)
		m_Node[i]->SetRoot(root);
}
