//==================================================
// mesh_sky.h
// Author: Buriya Kota
//==================================================
#ifndef _MESH_SKY_H_
#define _MESH_SKY_H_

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
class CMeshSky : public CObject
{
public:
	explicit CMeshSky(int nPriority = PRIORITY_BG);
	~CMeshSky() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE drawMode) override;

	static CMeshSky *Create();

	// �e�N�X�`���̐ݒ�
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }

private:
	void MeshVtxAndIdxCalculation_();

private:
	// ���[���h�}�g���b�N�X
	D3DXMATRIX m_mtxWorld;
	// �e�N�X�`���̗񋓌^
	CTexture::TEXTURE m_texture;
	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	// �~���̒��_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffCone;
	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
	// �ʒu
	D3DXVECTOR3	m_pos;
	// ����
	D3DXVECTOR3	m_rot;
	// ��
	int	m_nHorizontal;
	// �c
	int	m_nVertical;
	// ���_��
	int	m_nVtx;
	// �C���f�b�N�X��
	int	m_nIdx;
	// �|���S����
	int	m_nPolygon;
};

#endif	// _MESH_SKY_H_
