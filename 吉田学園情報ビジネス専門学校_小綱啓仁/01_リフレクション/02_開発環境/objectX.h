//==================================================
// objectX.h
// Author: Buriya Kota
//==================================================
#ifndef _OBJECTX_H_
#define _OBJECTX_H_

//**************************************************
// インクルード
//**************************************************
#include "main.h"
#include "texture.h"
#include "object.h"
#include "model_data.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************

//**************************************************
// クラス
//**************************************************
class CObjectX : public CObject
{
public:
	static const int NUM_TEXTURE = 16;
	static const int MAX_BILLBOARD = 5;
	static const float BILLBORAD_WIDTH;
	static const float BILLBOARD_HEIGHT;

public:
	explicit CObjectX(int nPriority = PRIORITY_OBJECT);
	virtual ~CObjectX() override;

	virtual HRESULT Init() override;
	virtual void Uninit() override;
	virtual void Update() override;
	virtual void Draw(DRAW_MODE drawMode) override;

	// セッター
	void SetPos(const D3DXVECTOR3& pos);
	void SetPosOld(const D3DXVECTOR3& posOld) { m_posOld = posOld; }
	void SetSize(const D3DXVECTOR3& size) { m_size = size; }
	void SetMove(const D3DXVECTOR3& move) { m_move = move; }
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	void SetRotDest(const D3DXVECTOR3& rotDest) { m_rotDest = rotDest; }
	void SetLife(const int &life) { m_nLife = life; }
	void SetWorldMtx(const D3DXMATRIX& mtx) { m_mtxWorld = mtx; }
	void SetQuat(const bool& quatanion) { m_bQuat = quatanion; }
	void SetVecAxis(D3DXVECTOR3 vecAxis) { m_vecAxis = vecAxis; }
	void SetValueRot(float valueRot) { m_fValueRot = valueRot; }
	void SetModelData(CModelData::MODEL_TYPE modelData) { m_modelData = modelData; }	// モデルの設定

	// ゲッター
	D3DXVECTOR3 GetMove() { return m_move; }
	//D3DXVECTOR3 *GetPosPointer() { return &m_posOrigin; }
	const D3DXVECTOR3& GetPos() const { return m_pos; }
	const D3DXVECTOR3& GetPosOld() const { return m_posOld; }
	const D3DXVECTOR3& GetSize() const { return m_size; }
	const D3DXVECTOR3& GetRot() const { return m_rot; }
	const D3DXVECTOR3& GetRotDest() const { return m_rotDest; }
	D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }
	D3DXVECTOR3 GetVtxMax() { return m_vtxMaxModel; }
	D3DXVECTOR3 GetVtxMin() { return m_vtxMinModel; }
	CModelData::MODEL_TYPE GetModelData() { return m_modelData; }

	// 移動量代入
	void MovePos(const D3DXVECTOR3& move) { m_pos += move; }

	// 角度の正規化
	D3DXVECTOR3 RotNormalization(D3DXVECTOR3 rot);
	D3DXVECTOR3 RotDestNormalization(D3DXVECTOR3 rot, D3DXVECTOR3 rotDest);

	static CObjectX *Create();

private:
	void DrawShadow_();

private:
	// 位置
	D3DXVECTOR3 m_pos;
	// 過去の位置
	D3DXVECTOR3 m_posOld;
	// 位置
	D3DXVECTOR3 m_move;
	// 向き
	D3DXVECTOR3 m_rot;
	// 向き
	D3DXVECTOR3 m_rotDest;
	// 最小値
	D3DXVECTOR3 m_vtxMinModel;
	// 最大値
	D3DXVECTOR3 m_vtxMaxModel;
	// ワールドマトリックス
	D3DXMATRIX m_mtxWorld;
	// テクスチャの列挙型
	LPDIRECT3DTEXTURE9 m_texture[NUM_TEXTURE];
	// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	// メッシュ(頂点の集まり)情報へのポインタ
	LPD3DXMESH m_mesh;
	// マテリアル情報へのポインタ	←　1のXファイルに複数のマテリアルが入っている
	LPD3DXBUFFER m_buffMat;
	// マテリアル情報の数
	DWORD m_numMat;
	// 大きさ
	D3DXVECTOR3 m_size;
	// 体力
	int m_nLife;
	// クォータニオンかどうか
	bool m_bQuat;
	// 
	D3DXMATRIX m_mtxRot;
	// クォータニオン
	D3DXQUATERNION m_quat;
	// 回転軸
	D3DXVECTOR3 m_vecAxis;
	// 回転角
	float m_fValueRot;
	// モデルデータの列挙型
	CModelData::MODEL_TYPE m_modelData;
	// 影の高さ
	float m_fHeight;
};

#endif	// _OBJECTX_H_