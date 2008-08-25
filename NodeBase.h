#ifndef __NODEBASE_H
#define __NODEBASE_H

enum node_type 
{
	SPHERE = 1, PLANE, PLUS, MINUS, MULTIPLE, CONE, CYLINDER, TORUS, POLYGON, CUBE, TEAPOT
};

enum eType { eSELECT, eMOVE, eROTATE, eSCALE, ePIVOT_MOVE };
enum eAxis { eNONE, eX, eY, eZ };

#include "sp.h"
#include "info.h"
class	CRayTraceDoc;
class	CRayTraceView;
class   Viewport;

struct fsize
{
	double	top;
	double	left;
	double	bottom;
	double	right;
};

class NodeBase
{
protected:
	// �A�g���r���[�g
	node_type	m_NodeType;
	char		m_Name[99];	// ���O
	D3DMATERIAL9 m_Material;

	matrix		m_Scale;	// �X�P�[���ϊ�
	matrix		m_Rotate;	// ��]
	matrix		m_Move;		// ���s�ړ�
	matrix		m_Pivot;	// �}�j���s���[�^�̒��S�_
	matrix		m_Matrix;

	double		m_Reflect ;		// ���˗�
	double		m_Through ;  	// ���ߗ�
	double		m_Refractive ;	// ���ܗ�

	struct tagBoundary {
		sp		Center;	// ���S
		double	Radius;	// ���a
		TARGET tagBoundary() : Center(), Radius(1) {}
	} m_Boundary;	// ���E

	char*	m_TextureFileName;
	HDC		m_hTextureDC;
	CSize	m_TextureSize;

	const CRayTraceDoc* m_pDoc;

public:

	NodeBase(const CRayTraceDoc* const pDoc, node_type NodeType, const char* const Name, const sp Color);
	NodeBase(const NodeBase &other);

	TARGET virtual	~NodeBase() {}

	// �I�y���[�V����
	void Set_Name(const char* const str);
	sp GetColor(NodeBase* Root, const sp& Light, const sp& K, const sp& L, int nest = 0) const;
	BOOL GetInfo2(const sp& K, const sp& L, Info& info) const;
	sp GetPixel(double x, double y) const;
	TARGET virtual	BOOL GetInfo(const sp& K, const sp& L, Info& info) const = 0;
	virtual	BOOL IsInside(const sp& L) const = 0;
	TARGET matrix getMatrix() { return m_Matrix; }
	BOOL MakeMemoryDCfromTextureFileName();
};

#endif	// __NODEBASE_H

/*
	bool	GetInfo(const sp& K, const sp& L, Info& info) const;

	����
		const sp& K,	�����x�N�g��
		const sp& L,	���_�̈ʒu
		Info& info		���Z�b�g�̃A�h���X
	�߂�l
		bool	true : ��_����
				false: ��_�Ȃ�
*/

