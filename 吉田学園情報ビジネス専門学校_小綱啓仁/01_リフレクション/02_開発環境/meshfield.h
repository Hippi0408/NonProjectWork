//==================================================
// meshfield.h
// Author: Buriya Kota
//==================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

//**************************************************
// �C���N���[�h
//**************************************************
#include "object.h"
#include "texture.h"

//**************************************************
// ���O�t��
//**************************************************
namespace nl = nlohmann;

//**************************************************
// �O���O���@���Ԃ�NG�@�|�C���^�����Ȃ�OK
//**************************************************

//**************************************************
// �N���X
//**************************************************
class CMeshField : public CObject
{
public:
	explicit CMeshField(int nPriority = PRIORITY_OBJECT);
	~CMeshField() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE /*drawMode*/) override;

	// �Z�b�^�[
	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	void SetMeshOneSideNumber(const int& nMeshPrimitive) { m_nMeshPrimitive = nMeshPrimitive; }
	void SetMeshSize(const float& fMeshSizeX, const float& fMeshSizeZ) { m_fMeshSizeX = fMeshSizeX, m_fMeshSizeY = fMeshSizeZ; }
	void SetSize(const D3DXVECTOR3& size) { m_size = size; }
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }	// �e�N�X�`���̐ݒ�

	// �Q�b�^�[
	const D3DXVECTOR3& GetPos() const { return m_pos; }
	const D3DXVECTOR3& GetSize() const { return m_size; }

	static CMeshField *Create();

	bool CollisionMesh(D3DXVECTOR3 *pPos);
	bool CollisionShadow(D3DXVECTOR3 *pPos, float *height);

	void Touch();

	void Load(const char* pFileName);

	void SetStomach(int nStomachMeshNumber, float fMeshSizeX, float fMeshSizeY);
	void ShakeMeigen();

private:
	void MeshVtxAndIdxCalculation_();
	void NorCalculation_();
	// ���Ȃ��̌v�Z
	void StomachVtxAndIdxCalculation_(int nStomachMeshNumber, float fMeshSizeX, float fMeshSizeY);

private:
	// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxWorld;
	// �e�N�X�`���̗񋓌^
	CTexture::TEXTURE m_texture;
	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
	// �ʒu
	D3DXVECTOR3 m_pos;
	// �ړ���
	D3DXVECTOR3 m_move;
	// ��]
	D3DXVECTOR3 m_rot;
	// �T�C�Y
	D3DXVECTOR3 m_size;
	// ����
	float m_height;
	// ��]�p�x
	float m_rotationAngle;
	// �ړI�̉�]�p�x
	float m_rotationAngleDest;
	// �p����]���x�N�g��
	D3DXVECTOR3 m_postureVec;
	// �ړI�̎p����]���x�N�g��
	D3DXVECTOR3 m_postureVecDest;
	// ���X�g�̐���
	nl::json m_JMesh;
	// ��ӂ̖̂ʐ�
	int m_nMeshOneSideNumber;
	// ���b�V���̒��_��
	int m_nMeshVtx;
	// ���b�V���̃C���f�b�N�X��
	int m_nMeshIdx;
	// ���b�V���̃|���S����
	int m_nMeshPrimitive;
	// ���b�V���̑傫��X
	float m_fMeshSizeX;
	// ���b�V���̑傫��Y
	float m_fMeshSizeY;
	// ���b�V���̑傫��Z
	float m_fMeshSizeZ;
	// ��ӂ̌��i���Ȃ��j
	int m_nStomachMeshNumber;

	// �^�b�`�������W
	D3DXVECTOR3 m_touchPos;
};

#endif	// _MESHFIELD_H_
