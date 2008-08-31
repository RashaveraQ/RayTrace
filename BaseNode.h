#ifndef __BASENODE_H
#define __BASENODE_H

enum node_type 
{
	SPHERE = 1, PLANE, PLUS, MINUS, MULTIPLE, CONE, CYLINDER, TORUS, POLYGON, CUBE, TEAPOT
};

enum eType { eSELECT, eMOVE, eROTATE, eSCALE, ePIVOT_MOVE };
enum eAxis { eNONE, eX, eY, eZ };

struct	sp;
struct	Info;
class	CDlgMatrix;
class	CDlgMaterial;
class	CRayTraceDoc;
class	CRayTraceView;
class   Viewport;
struct	matrix;

struct fsize
{
	double	top;
	double	left;
	double	bottom;
	double	right;
};

class BaseNode
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


	// �A�g���r���[�g
	char*		m_TextureFileName;
	HDC			m_hTextureDC;		// �e�N�X�`���C���[�W�i�[�p
	CSize		m_TextureSize;

	struct tagBoundary {
		sp		Center;	// ���S
		double	Radius;	// ���a
		TARGET tagBoundary() : Center(), Radius(1) {}
	} m_Boundary;	// ���E
	BaseNode*		m_DeviceData;

public:
	double		m_Reflect ;		// ���˗�
	double		m_Through ;  	// ���ߗ�
	double		m_Refractive ;	// ���ܗ�

	TARGET BaseNode() : m_DeviceData(0) {}
	BaseNode(node_type NodeType, const char* const Name, const sp Color);
	~BaseNode();

	void updateDeviceData();

	// �I�y���[�V����
	void Set_Name(const char* const str) { memcpy(m_Name, str, 99); }  
	sp GetColor(const sp* K, const sp* L, int nest);
	BOOL GetInfo2(const sp* K, const sp* L, Info* info);
	sp GetPixel(double x, double y);
	BOOL GetInfo(const sp* K, const sp* L, Info* info);
	BOOL IsInside(const sp* L);
	
	TARGET matrix getMatrix() { return m_Matrix; }
	TARGET BaseNode* getDeviceData() { return m_DeviceData; }
	BOOL MakeMemoryDCfromTextureFileName();
};

#endif	// __BASENODE_H

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

