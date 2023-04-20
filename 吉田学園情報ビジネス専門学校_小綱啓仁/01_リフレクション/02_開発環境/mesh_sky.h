//==================================================
// mesh_sky.h
// Author: Buriya Kota
//==================================================
#ifndef _MESH_SKY_H_
#define _MESH_SKY_H_

//**************************************************
// インクルード
//**************************************************
#include "object.h"
#include "texture.h"

//**************************************************
// 名前付け
//**************************************************
namespace nl = nlohmann;

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************

//**************************************************
// クラス
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

	// テクスチャの設定
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }

private:
	void MeshVtxAndIdxCalculation_();

private:
	// ワールドマトリックス
	D3DXMATRIX m_mtxWorld;
	// テクスチャの列挙型
	CTexture::TEXTURE m_texture;
	// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	// 円錐の頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffCone;
	// インデックスバッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
	// 位置
	D3DXVECTOR3	m_pos;
	// 向き
	D3DXVECTOR3	m_rot;
	// 横
	int	m_nHorizontal;
	// 縦
	int	m_nVertical;
	// 頂点数
	int	m_nVtx;
	// インデックス数
	int	m_nIdx;
	// ポリゴン数
	int	m_nPolygon;
};

#endif	// _MESH_SKY_H_
