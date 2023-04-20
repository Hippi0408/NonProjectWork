//==================================================
// meshfield.h
// Author: Buriya Kota
//==================================================
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

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
class CMeshField : public CObject
{
public:
	explicit CMeshField(int nPriority = PRIORITY_OBJECT);
	~CMeshField() override;

	HRESULT Init() override;
	void Uninit() override;
	void Update() override;
	void Draw(DRAW_MODE /*drawMode*/) override;

	// セッター
	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	void SetMeshOneSideNumber(const int& nMeshPrimitive) { m_nMeshPrimitive = nMeshPrimitive; }
	void SetMeshSize(const float& fMeshSizeX, const float& fMeshSizeZ) { m_fMeshSizeX = fMeshSizeX, m_fMeshSizeY = fMeshSizeZ; }
	void SetSize(const D3DXVECTOR3& size) { m_size = size; }
	void SetTexture(CTexture::TEXTURE texture) { m_texture = texture; }	// テクスチャの設定

	// ゲッター
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
	// おなかの計算
	void StomachVtxAndIdxCalculation_(int nStomachMeshNumber, float fMeshSizeX, float fMeshSizeY);

private:
	// ワールドマトリックス
	D3DXMATRIX m_mtxWorld;
	// テクスチャの列挙型
	CTexture::TEXTURE m_texture;
	// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	// インデックスバッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
	// 位置
	D3DXVECTOR3 m_pos;
	// 移動量
	D3DXVECTOR3 m_move;
	// 回転
	D3DXVECTOR3 m_rot;
	// サイズ
	D3DXVECTOR3 m_size;
	// 高さ
	float m_height;
	// 回転角度
	float m_rotationAngle;
	// 目的の回転角度
	float m_rotationAngleDest;
	// 姿勢回転軸ベクトル
	D3DXVECTOR3 m_postureVec;
	// 目的の姿勢回転軸ベクトル
	D3DXVECTOR3 m_postureVecDest;
	// リストの生成
	nl::json m_JMesh;
	// 一辺のの面数
	int m_nMeshOneSideNumber;
	// メッシュの頂点数
	int m_nMeshVtx;
	// メッシュのインデックス数
	int m_nMeshIdx;
	// メッシュのポリゴン数
	int m_nMeshPrimitive;
	// メッシュの大きさX
	float m_fMeshSizeX;
	// メッシュの大きさY
	float m_fMeshSizeY;
	// メッシュの大きさZ
	float m_fMeshSizeZ;
	// 一辺の個数（おなか）
	int m_nStomachMeshNumber;

	// タッチした座標
	D3DXVECTOR3 m_touchPos;
};

#endif	// _MESHFIELD_H_
