//==================================================
// model_data.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "manager.h"
#include "renderer.h"

#include "model_data.h"

//**************************************************
// 定義
//**************************************************
const char* CModelData::s_FileName[] =
{// テクスチャのパス
	"data/MODEL/player/body.x",			// 体
	"data/MODEL/player/head.x",			// 頭
	"data/MODEL/player/right_arm.x",	// 右腕
	"data/MODEL/player/left_arm.x",		// 左腕
	"data/MODEL/player/board.x",		// 板
	"data/MODEL/player/left_thigh.x",	// 左太もも
	"data/MODEL/player/right_thigh.x",	// 右太もも
	"data/MODEL/player/left_leg.x",		// 左足
	"data/MODEL/player/right_leg.x",	// 右足
	"data/MODEL/ball.x",				// 球
	"data/MODEL/cube4.x",				// ギミック
	"data/MODEL/gimmick/goal.x",		// ゴール
	"data/MODEL/cube_player.x",			// キューブプレイヤー
	"data/MODEL/tree.x",				// 木
	"data/MODEL/Coin03.x",				// コイン
	"data/MODEL/cleannessObject.x",		// 透明オブジェクト
	"data/MODEL/cleannessObject_vertical.x",		// 透明オブジェクト縦
	"data/MODEL/cleannessObject_short.x",			// 透明オブジェクト(短い)
	"data/MODEL/cleannessObject_vertical_short.x",	// 透明オブジェクト縦(短い)
};

static_assert(sizeof(CModelData::s_FileName) / sizeof(CModelData::s_FileName[0]) == CModelData::MODEL_MAX, "aho");

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CModelData::CModelData() : m_model()
{
	memset(m_model, 0, sizeof(m_model));
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CModelData::~CModelData()
{
}

//--------------------------------------------------
// 全ての読み込み
//--------------------------------------------------
void CModelData::LoadAll()
{
	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	for (int nCnt = 0; nCnt < MODEL_MAX; ++nCnt)
	{// Xファイルの読み込み
		D3DXLoadMeshFromX(s_FileName[nCnt],
			D3DXMESH_SYSTEMMEM,
			pDevice,
			NULL,
			&m_model[nCnt].buffMat,
			NULL,
			&m_model[nCnt].numMat,
			&m_model[nCnt].mesh);

		// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
		D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_model[nCnt].buffMat->GetBufferPointer();

		// 各メッシュのマテリアル情報を取得する
		for (int i = 0; i < (int)m_model[i].numMat; i++)
		{
			m_model[nCnt].texture[i] = NULL;

			if (pMat[i].pTextureFilename != NULL)
			{// マテリアルで設定されているテクスチャ読み込み
				D3DXCreateTextureFromFileA(pDevice,
					pMat[i].pTextureFilename,
					&m_model[nCnt].texture[i]);
			}
		}

		int nNumVtx;		// 頂点数
		DWORD pSizeFVF;		// 頂点フォーマットのサイズ
		BYTE *pVtxBuff;		// 頂点バッファのポインタ

		// 頂点数の取得
		nNumVtx = m_model[nCnt].mesh->GetNumVertices();

		// 頂点フォーマットのサイズを取得
		pSizeFVF = D3DXGetFVFVertexSize(m_model[nCnt].mesh->GetFVF());

		// 頂点バッファのロック
		m_model[nCnt].mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

		for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
		{
			// 頂点座標の代入
			D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

			// 比較(最小値を求める)x
			if (vtx.x < m_model[nCnt].vtxMin.x)
			{
				m_model[nCnt].vtxMin.x = vtx.x;
			}
			// 比較(最小値を求める)y
			if (vtx.y < m_model[nCnt].vtxMin.y)
			{
				m_model[nCnt].vtxMin.y = vtx.y;
			}
			// 比較(最小値を求める)z
			if (vtx.z < m_model[nCnt].vtxMin.z)
			{
				m_model[nCnt].vtxMin.z = vtx.z;
			}

			// 比較(最大値を求める)x
			if (vtx.x > m_model[nCnt].vtxMax.x)
			{
				m_model[nCnt].vtxMax.x = vtx.x;
			}
			// 比較(最大値を求める)y
			if (vtx.y > m_model[nCnt].vtxMax.y)
			{
				m_model[nCnt].vtxMax.y = vtx.y;
			}
			// 比較(最大値を求める)z
			if (vtx.z > m_model[nCnt].vtxMax.z)
			{
				m_model[nCnt].vtxMax.z = vtx.z;
			}

			// 頂点フォーマットのサイズ分ポインタを進める
			pVtxBuff += pSizeFVF;
		}

		// サイズの計算
		m_model[nCnt].size = m_model[nCnt].vtxMax - m_model[nCnt].vtxMin;

		// 頂点バッファのアンロック
		m_model[nCnt].mesh->UnlockVertexBuffer();
	}
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void CModelData::Load(MODEL_TYPE inModel)
{
	if (m_model[inModel].mesh != nullptr)
	{// テクスチャの読み込みがされている
		return;
	}

	// デバイスへのポインタの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	// Xファイルの読み込み
	D3DXLoadMeshFromX(s_FileName[inModel],
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_model[inModel].buffMat,
		NULL,
		&m_model[inModel].numMat,
		&m_model[inModel].mesh);

	// バッファの先頭ポインタをD3DXMATERIALにキャストして取得
	D3DXMATERIAL *pMat = (D3DXMATERIAL*)m_model[inModel].buffMat->GetBufferPointer();

	// 各メッシュのマテリアル情報を取得する
	for (int i = 0; i < (int)m_model[inModel].numMat; i++)
	{
		m_model[inModel].texture[i] = NULL;

		if (pMat[i].pTextureFilename != NULL)
		{// マテリアルで設定されているテクスチャ読み込み
			D3DXCreateTextureFromFileA(pDevice,
				pMat[i].pTextureFilename,
				&m_model[inModel].texture[i]);
		}
	}

	int nNumVtx;		// 頂点数
	DWORD pSizeFVF;		// 頂点フォーマットのサイズ
	BYTE *pVtxBuff;		// 頂点バッファのポインタ

	// 頂点数の取得
	nNumVtx = m_model[inModel].mesh->GetNumVertices();

	// 頂点フォーマットのサイズを取得
	pSizeFVF = D3DXGetFVFVertexSize(m_model[inModel].mesh->GetFVF());

	// 頂点バッファのロック
	m_model[inModel].mesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);

	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		// 頂点座標の代入
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff;

		// 比較(最小値を求める)x
		if (vtx.x < m_model[inModel].vtxMin.x)
		{
			m_model[inModel].vtxMin.x = vtx.x;
		}
		// 比較(最小値を求める)y
		if (vtx.y < m_model[inModel].vtxMin.y)
		{
			m_model[inModel].vtxMin.y = vtx.y;
		}
		// 比較(最小値を求める)z
		if (vtx.z < m_model[inModel].vtxMin.z)
		{
			m_model[inModel].vtxMin.z = vtx.z;
		}

		// 比較(最大値を求める)x
		if (vtx.x > m_model[inModel].vtxMax.x)
		{
			m_model[inModel].vtxMax.x = vtx.x;
		}
		// 比較(最大値を求める)y
		if (vtx.y > m_model[inModel].vtxMax.y)
		{
			m_model[inModel].vtxMax.y = vtx.y;
		}
		// 比較(最大値を求める)z
		if (vtx.z > m_model[inModel].vtxMax.z)
		{
			m_model[inModel].vtxMax.z = vtx.z;
		}

		// 頂点フォーマットのサイズ分ポインタを進める
		pVtxBuff += pSizeFVF;
	}

	// サイズの計算
	m_model[inModel].size = m_model[inModel].vtxMax - m_model[inModel].vtxMin;

	// 頂点バッファのアンロック
	m_model[inModel].mesh->UnlockVertexBuffer();
}

//--------------------------------------------------
// 全ての解放
//--------------------------------------------------
void CModelData::ReleaseAll()
{
	for (int nCnt = 0; nCnt < MODEL_MAX; nCnt++)
	{
		for (int i = 0; i < (int)m_model[nCnt].numMat; i++)
		{
			if (m_model[nCnt].texture[i] != nullptr)
			{// テクスチャの解放
				m_model[nCnt].texture[i]->Release();
				m_model[nCnt].texture[i] = nullptr;
			}
		}

		// メッシュの破棄
		if (m_model[nCnt].mesh != nullptr)
		{
			m_model[nCnt].mesh->Release();
			m_model[nCnt].mesh = nullptr;
		}

		// マテリアルの解放
		if (m_model[nCnt].buffMat != nullptr)
		{
			m_model[nCnt].buffMat->Release();
			m_model[nCnt].buffMat = nullptr;
		}
	}
}

//--------------------------------------------------
// 解放
//--------------------------------------------------
void CModelData::Release(MODEL_TYPE inModel)
{
	assert(inModel >= 0 && inModel < MODEL_MAX);

	for (int i = 0; i < (int)m_model[inModel].numMat; i++)
	{
		if (m_model[inModel].texture[i] != nullptr)
		{// テクスチャの解放
			m_model[inModel].texture[i]->Release();
			m_model[inModel].texture[i] = nullptr;
		}
	}

	// メッシュの破棄
	if (m_model[inModel].mesh != nullptr)
	{
		m_model[inModel].mesh->Release();
		m_model[inModel].mesh = nullptr;
	}

	// マテリアルの解放
	if (m_model[inModel].buffMat != nullptr)
	{
		m_model[inModel].buffMat->Release();
		m_model[inModel].buffMat = nullptr;
	}
}

//--------------------------------------------------
// 取得
//--------------------------------------------------
CModelData::MODEL CModelData::GetModel(MODEL_TYPE inModel)
{
	assert(inModel >= 0 && inModel < MODEL_MAX);

	// 読み込み
	Load(inModel);

	return m_model[inModel];
}
