//==================================================
// model.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>

#include "utility.h"

#include "manager.h"

#include "model.h"
#include "light.h"

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CModel::CModel()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CModel::~CModel()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CModel::Init(CModelData::MODEL_TYPE modelData)
{
	m_modelData = modelData;

	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(3.0f, 3.0f, 3.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fHeight = 0.0f;
	m_isStencil = false;

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CModel::Uninit()
{
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CModel::Update()
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CModel::Draw(D3DXMATRIX *mtxWorld, CObject::DRAW_MODE drawMode)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();
	CModelData* modelData = CManager::GetManager()->GetModelData();
	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale, mtxParent;
	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;
	// マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	if (m_isStencil)
	{
		// ステンシルバッファ -> 有効
		pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

		// ステンシルバッファと比較する参照値設定 -> ref
		pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

		// ステンシルバッファの値に対してのマスク設定 -> 0xff(全て真)
		pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

		// ステンシルテストの比較方法 ->
		// （参照値 >= ステンシルバッファの参照値）なら合格
		pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);

		// ステンシルテストの結果に対しての反映設定
		pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);		// Zとステンシル成功
		pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);			// Zとステンシル失敗
		pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);			// Zのみ失敗
	}
	else
	{
		// ステンシルバッファ -> 無効
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	}

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 行列拡縮関数
	D3DXMatrixScaling(&mtxScale, m_scale.x, m_scale.y, m_scale.z);
	// 行列掛け算関数(第2引数×第3引数第を１引数に格納)
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScale);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParent != nullptr)
	{
		mtxParent = m_pParent->GetMtxWorld();
	}
	else
	{
		mtxParent = *mtxWorld;
	}

	// マトリックスの掛け算
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);

	if (drawMode == CObject::DRAW_MODE_SHADOW)
	{
		DrawShadow_();
	}
	else
	{
		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// 現在のマテリアル保持
		pDevice->GetMaterial(&matDef);

		// マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)modelData->GetModel(m_modelData).buffMat->GetBufferPointer();

		for (int i = 0; i < (int)modelData->GetModel(m_modelData).numMat; i++)
		{
			pMat[i].MatD3D.Ambient = pMat[i].MatD3D.Diffuse;
			// マテリアルの設定
			pDevice->SetMaterial(&pMat[i].MatD3D);

			// テクスチャの設定
			pDevice->SetTexture(0, modelData->GetModel(m_modelData).texture[i]);

			// モデルパーツの描画
			modelData->GetModel(m_modelData).mesh->DrawSubset(i);
		}

		// 保存していたマテリアルを戻す
		pDevice->SetMaterial(&matDef);

		// テクスチャの設定
		pDevice->SetTexture(0, NULL);

		// ステンシルバッファ -> 無効
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	}
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CModel *CModel::Create(CModelData::MODEL_TYPE modelData)
{
	CModel *pModel;
	pModel = new CModel;

	if (pModel != nullptr)
	{
		pModel->Init(modelData);
	}
	else
	{
		assert(false);
	}

	return pModel;
}

//--------------------------------------------------
// 影の描画
//--------------------------------------------------
void CModel::DrawShadow_()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();
	CModelData* modelData = CManager::GetManager()->GetModelData();

	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;
	// マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;
	D3DXMATRIX mtxShadow;
	D3DXPLANE planeField;
	D3DXVECTOR4 vecLight;
	D3DXVECTOR3 pos, normal;

	// アンビエントを無効にする
	pDevice->SetRenderState(D3DRS_AMBIENT, 0);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// ステンシルバッファ -> 無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxShadow);	

	// ライトを取ってくる
	CLight *pLight = CManager::GetManager()->GetLight();

	D3DXVECTOR3 vecDir = pLight->Get().Direction;

	vecLight = D3DXVECTOR4(-vecDir.x, -vecDir.y, -vecDir.z, 0.0f);

	pos = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);
	normal = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 
	D3DXPlaneFromPointNormal(&planeField, &pos, &normal);
	// 影のマトリックスを作成
	D3DXMatrixShadow(&mtxShadow, &vecLight, &planeField);

	// 行列の掛け算
	D3DXMatrixMultiply(&mtxShadow, &m_mtxWorld, &mtxShadow);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtxShadow);

	// 現在のマテリアル保持
	pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)modelData->GetModel(m_modelData).buffMat->GetBufferPointer();

	// テクスチャの設定
	pDevice->SetTexture(0, NULL);

	for (int i = 0; i < (int)modelData->GetModel(m_modelData).numMat; i++)
	{
		// マテリアル情報の設定
		D3DMATERIAL9 matD3D = pMat[i].MatD3D;

		// 引数を色に設定
		matD3D.Diffuse = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
		matD3D.Emissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// マテリアルの設定
		pDevice->SetMaterial(&matD3D);

		// モデルパーツの描画
		modelData->GetModel(m_modelData).mesh->DrawSubset(i);
	}

	// 保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
	// アンビエントをもとに戻す
	pDevice->SetRenderState(D3DRS_AMBIENT, 0xffffffff);

	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}
