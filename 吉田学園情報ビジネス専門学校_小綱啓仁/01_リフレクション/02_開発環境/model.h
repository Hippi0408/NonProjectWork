//==================================================
// model.h
// Author: Buriya Kota
//==================================================
#ifndef _MODEL_H_
#define _MODEL_H_

//**************************************************
// インクルード
//**************************************************
#include "object.h"
#include "texture.h"
#include "model_data.h"

//**************************************************
// 前方前言　実態はNG　ポインタだけならOK
//**************************************************

//**************************************************
// マクロ定義
//**************************************************
#define MAX_MODEL				(54)
#define MAX_TEXTURE				(54)

//**************************************************
// クラス
//**************************************************
class CModel
{
public:
	CModel();
	~CModel();

	HRESULT Init(CModelData::MODEL_TYPE modelData);
	void Uninit();
	void Update();
	void Draw(D3DXMATRIX *mtxWorld, CObject::DRAW_MODE drawMode);

	static CModel *Create(CModelData::MODEL_TYPE modelData);

	// セッター
	void SetPos(const D3DXVECTOR3& pos) { m_pos = pos; }
	void SetScale(const D3DXVECTOR3& scale) { m_scale = scale; }
	void SetRot(const D3DXVECTOR3& rot) { m_rot = rot; }
	void SetParent(CModel *pModel) { m_pParent = pModel; }
	void SetModelData(CModelData::MODEL_TYPE modelData) { m_modelData = modelData; }	// モデルの設定
	void SetShadowHeight(const float fHeight) { m_fHeight = fHeight; }

	// ゲッター
	const D3DXVECTOR3& GetPos() const { return m_pos; }
	const D3DXVECTOR3& GetScale() const { return m_scale; }
	const D3DXVECTOR3& GetRot() const { return m_rot; }
	D3DXMATRIX GetMtxWorld() { return m_mtxWorld; }
	CModelData::MODEL_TYPE GetModelData() { return m_modelData; }

	void SetStencil(bool isStencil) { m_isStencil = isStencil; }

private:
	void DrawShadow_();

private:
	// 頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	// ワールドマトリックス
	D3DXMATRIX m_mtxWorld;
	// 位置
	D3DXVECTOR3 m_pos;
	// 大きさ
	D3DXVECTOR3 m_scale;
	// 向き
	D3DXVECTOR3 m_rot;
	// 最小値
	D3DXVECTOR3 m_vtxMinModel;
	// 最大値
	D3DXVECTOR3 m_vtxMaxModel;
	// 親の情報
	CModel *m_pParent;
	// 高さ
	float m_fHeight;
	// 
	int m_nType;
	// 
	bool m_isStencil;
	// モデルデータの列挙型
	CModelData::MODEL_TYPE m_modelData;
};

#endif	// _MODEL_H_