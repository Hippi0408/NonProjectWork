//==================================================
// objectX.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <assert.h>
#include "debug_proc.h"

#include "manager.h"
#include "game.h"
#include "objectX.h"
#include "renderer.h"
#include "camera.h"
#include "light.h"

//**************************************************
// 静的メンバ変数
//**************************************************
const float CObjectX::BILLBOARD_HEIGHT = 10.0f;
const float CObjectX::BILLBORAD_WIDTH = 10.0f;

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CObjectX::CObjectX(int nPriority /* = 3 */) : CObject(nPriority)
{
	// 位置
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 過去の位置
	m_posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 向き
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 最小値
	m_vtxMinModel = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	// 最大値
	m_vtxMaxModel = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	// 大きさ
	m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	// 影の高さ
	m_fHeight = 0.0f;

	for (int nCnt = 0; nCnt < NUM_TEXTURE; nCnt++)
	{// テクスチャの列挙型
		m_texture[nCnt] = nullptr;
	}
	// 頂点バッファへのポインタ
	m_pVtxBuff = nullptr;
	// メッシュ(頂点の集まり)情報へのポインタ
	m_mesh = nullptr;
	// マテリアル情報へのポインタ	←　1のXファイルに複数のマテリアルが入っている
	m_buffMat = nullptr;
	// マテリアル情報の数
	m_numMat = NULL;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxRot);
	// クォータニオンの初期化
	D3DXQuaternionIdentity(&m_quat);
	// クォータニオンかどうか
	m_bQuat = false;

	// モデルのデータ
	m_modelData = CModelData::MODEL_NONE;
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CObjectX::~CObjectX()
{
	assert(m_pVtxBuff == nullptr);
	assert(m_mesh == nullptr);
	assert(m_buffMat == nullptr);
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CObjectX::Init()
{
	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CObjectX::Uninit()
{
	for (int i = 0; i < (int)m_numMat; i++)
	{
		if (m_texture[i] != nullptr)
		{// テクスチャの解放
			m_texture[i]->Release();
			m_texture[i] = nullptr;
		}
	}
	// メッシュの破棄
	if (m_mesh != nullptr)
	{
		m_mesh->Release();
		m_mesh = nullptr;
	}
	// マテリアルの破棄
	if (m_buffMat != nullptr)
	{
		m_buffMat->Release();
		m_buffMat = nullptr;
	}

	CObject::DeletedObj();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CObjectX::Update()
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CObjectX::Draw(DRAW_MODE drawMode)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();
	CModelData* modelData = CManager::GetManager()->GetModelData();

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxScale;
	// 現在のマテリアル保存用
	D3DMATERIAL9 matDef;
	// マテリアルデータへのポインタ
	D3DXMATERIAL *pMat;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	if (m_bQuat)
	{// クォータニオンの使用
		// クォータニオンを作成
		D3DXQuaternionRotationAxis(&m_quat, &m_vecAxis, m_fValueRot);
		// クォータニオンから回転マトリックスを作成
		D3DXMatrixRotationQuaternion(&mtxRot, &m_quat);
		// 
		D3DXMatrixMultiply(&m_mtxRot, &m_mtxRot, &mtxRot);
		// 行列の掛け算
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &m_mtxRot);
	}
	else
	{
		// 向きを反映							↓rotの情報を使って回転行列を作る
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);		// 行列掛け算関数		第二引数 * 第三引数 を　第一引数に格納
	}

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (drawMode == DRAW_MODE_SHADOW)
	{
		if (!(m_modelData == CModelData::MODEL_CLEENNESS_OBJECT || m_modelData == CModelData::MODEL_CLEENNESS_OBJECT_SHORT
		|| m_modelData == CModelData::MODEL_CLEENNESS_OBJECT_VERTICAL || m_modelData == CModelData::MODEL_CLEENNESS_OBJECT_VERTICAL_SHORT || m_modelData == CModelData::MODEL_TREE))
		{
			DrawShadow_();
		}
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
	}
}

//--------------------------------------------------
// 位置の設定
//--------------------------------------------------
void CObjectX::SetPos(const D3DXVECTOR3 &pos)
{
	m_pos = pos;
}

//--------------------------------------------------
// 現在の角度の正規化
//--------------------------------------------------
D3DXVECTOR3 CObjectX::RotNormalization(D3DXVECTOR3 rot)
{
	m_rot = rot;

	// 現在の角度の正規化
	if (m_rot.x > D3DX_PI)
	{
		m_rot.x -= D3DX_PI * 2.0f;
	}
	else if (m_rot.x < -D3DX_PI)
	{
		m_rot.x += D3DX_PI * 2.0f;
	}

	// 現在の角度の正規化
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}

	// 現在の角度の正規化
	if (m_rot.z > D3DX_PI)
	{
		m_rot.z -= D3DX_PI * 2.0f;
	}
	else if (m_rot.z < -D3DX_PI)
	{
		m_rot.z += D3DX_PI * 2.0f;
	}

	return m_rot;
}

//--------------------------------------------------
// 目的の角度の正規化
//--------------------------------------------------
D3DXVECTOR3 CObjectX::RotDestNormalization(D3DXVECTOR3 rot, D3DXVECTOR3 rotDest)
{
	m_rot = rot;
	m_rotDest = rotDest;

	// 目的の角度の正規化
	if (m_rotDest.y - m_rot.y > D3DX_PI)
	{
		m_rotDest.y -= D3DX_PI * 2.0f;
	}
	else if (m_rotDest.y - m_rot.y < -D3DX_PI)
	{
		m_rotDest.y += D3DX_PI * 2.0f;
	}

	return m_rotDest;
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CObjectX *CObjectX::Create()
{
	CObjectX *pObjectX;
	pObjectX = new CObjectX;

	if (pObjectX != nullptr)
	{
		pObjectX->Init();
	}
	else
	{
		assert(false);
	}

	return pObjectX;
}

//--------------------------------------------------
// 影の描画
//--------------------------------------------------
void CObjectX::DrawShadow_()
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
	//pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);

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
