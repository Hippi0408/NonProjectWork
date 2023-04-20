//==================================================
// mesh_sky.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "manager.h"
#include "game.h"
#include "debug_proc.h"

#include "mesh_sky.h"

//**************************************************
// マクロ定義
//**************************************************
#define HEIGHT					(-2500.0f)
#define MAX_SIZE				(12000.0f)	// サイズの最大値
#define START_HORIZONTAL		(30)		// 横の最初の値
#define START_VERTICAL			(15)		// 縦の最初の値

//**************************************************
// 定数定義
//**************************************************
// 頂点フォーマット
const DWORD FVF_VERTEX_3D = (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1);		// 座標・法線・カラー・テクスチャ

//**************************************************
// 構造体定義
//**************************************************
// 頂点の情報[3D]の構造体を定義
struct VERTEX_3D
{
	D3DXVECTOR3 pos;	// 頂点座標	
	D3DXVECTOR3 nor;	// 法線ベクトル
	D3DCOLOR col;		// 頂点カラー	
	D3DXVECTOR2 tex;	// テクスチャの座標
};

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CMeshSky::CMeshSky(int nPriority) : CObject(nPriority)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CMeshSky::~CMeshSky()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CMeshSky::Init()
{
	SetTexture(CTexture::TEXTURE_SKY);

	m_nHorizontal = 0;
	m_nVertical = 0;
	m_nVtx = 0;
	m_nIdx = 0;
	m_nPolygon = 0;
	m_pos = D3DXVECTOR3(0.0f, HEIGHT, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 横・縦の初期化
	m_nHorizontal = START_HORIZONTAL;
	m_nVertical = START_VERTICAL;

	// 設定
	MeshVtxAndIdxCalculation_();

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CMeshSky::Uninit()
{
	if (m_pVtxBuff != nullptr)
	{// 頂点バッファの破棄
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	if (m_pIdxBuff != nullptr)
	{// インデックスバッファの破棄
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}

	CObject::DeletedObj();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CMeshSky::Update()
{
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CMeshSky::Draw(DRAW_MODE /*drawMode*/)
{	
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();
	CTexture* pTexture = CManager::GetManager()->GetTexture();

	D3DXMATRIX mtxRot, mtxTrans;		// 計算用マトリックス

	// ライトを無効
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// ポリゴン描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,		// プリミティブの種類
		0,							// 描画する最初の頂点バッファ
		0,							// インデックスの最小値
		m_nVtx,						// 頂点数
		0,							// 描画する最初の頂点インデックス
		m_nPolygon);				// プリミティブ(ポリゴン)数

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffCone, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLEFAN,			// プリミティブの種類
		0,							// 描画する最初の頂点インデックス
		m_nHorizontal);				// プリミティブ(ポリゴン)数

	// ライトを無効
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CMeshSky* CMeshSky::Create()
{
	CMeshSky *pMeshSky;
	pMeshSky = new CMeshSky;

	if (pMeshSky != nullptr)
	{
		pMeshSky->Init();
	}
	else
	{
		assert(false);
	}

	return pMeshSky;
}

//--------------------------------------------------
// vtxとidxの計算
//--------------------------------------------------
void CMeshSky::MeshVtxAndIdxCalculation_()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	int nXLine = m_nHorizontal + 1;
	int nYLine = m_nVertical + 1;

	// 頂点数を計算
	m_nVtx = nXLine * nYLine;

	// インデックス数を計算
	m_nIdx = ((nXLine * 2) * m_nVertical) + ((m_nVertical - 1) * 2);

	// ポリゴン数を計算
	m_nPolygon = (m_nHorizontal * m_nVertical * 2) + ((m_nVertical - 1) * 4);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * m_nVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	VERTEX_3D *pVtx = NULL;		// 頂点情報へのポインタ

	// 頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int y = 0; y < nYLine; y++)
	{
		float fYRot = (((D3DX_PI * 0.25f) / m_nVertical) * y) + (D3DX_PI * 0.25f);

		float fYPos = cosf(fYRot) * MAX_SIZE;

		for (int x = 0; x < nXLine; x++)
		{
			float fRot = ((D3DX_PI * 2.0f) / m_nHorizontal) * x;

			//正規化
			if (fRot > D3DX_PI)
			{
				fRot += D3DX_PI * 2;
			}
			if (fRot < -D3DX_PI)
			{
				fRot += -D3DX_PI * 2;
			}

			float fXPos = sinf(fRot) * sinf(fYRot) * MAX_SIZE;
			float fZPos = cosf(fRot) * sinf(fYRot) * MAX_SIZE;
			D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

			// 頂点座標の設定
			pVtx[x + (y * nXLine)].pos = pos;

			D3DXVECTOR3 vec;

			// 正規化する ( 大きさ 1 のベクトルにする )
			D3DXVec3Normalize(&vec, &pos);

			// 各頂点の法線の設定
			pVtx[x + (y * nXLine)].nor = vec;

			// 頂点カラーの設定
			pVtx[x + (y * nXLine)].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			float fUTex = (1.0f / m_nHorizontal) * x;
			float fVTex = (1.0f / m_nVertical) * y;

			// テクスチャ座標の設定
			pVtx[x + (y * nXLine)].tex = D3DXVECTOR2(fUTex, fVTex);
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(VERTEX_3D) * m_nIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	WORD *pIdx = NULL;		// インデックス情報へのポインタ

	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	for (int x = 0, y = 0; y < m_nVertical; x++, y++)
	{
		for (; x < (nXLine * (y + 1)) + y; x++)
		{
			pIdx[x * 2] = (WORD)(x - y + nXLine);
			pIdx[(x * 2) + 1] = (WORD)(x - y);
			x = x;
		}

		if (y < m_nVertical - 1)
		{// これで終わりじゃないなら
			pIdx[x * 2] = (WORD)(x - (y + 1));
			pIdx[(x * 2) + 1] = (WORD)((x * 2) - (y * (m_nHorizontal + 3)));
			x = x;
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();

	// 円錐の頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * (m_nHorizontal + 2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuffCone,
		NULL);

	// 頂点情報をロックし、頂点情報へのポインタを取得
	m_pVtxBuffCone->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < nXLine; i++)
	{
		float fYRot = D3DX_PI * 0.25f;
		float fRot = ((D3DX_PI * 2.0f) / m_nHorizontal) * i;

		//正規化
		if (fRot > D3DX_PI)
		{
			fRot += D3DX_PI * 2;
		}
		if (fRot < -D3DX_PI)
		{
			fRot += -D3DX_PI * 2;
		}

		float fXPos = sinf(-fRot) * sinf(fYRot) * MAX_SIZE;
		float fYPos = cosf(fYRot) * MAX_SIZE;
		float fZPos = cosf(-fRot) * sinf(fYRot) * MAX_SIZE;
		D3DXVECTOR3 pos = D3DXVECTOR3(fXPos, fYPos, fZPos);

		// 頂点座標の設定
		pVtx[i + 1].pos = pos;

		D3DXVECTOR3 vec;

		// 正規化する ( 大きさ 1 のベクトルにする )
		D3DXVec3Normalize(&vec, &pos);

		// 各頂点の法線の設定
		pVtx[i + 1].nor = vec;

		// 頂点カラーの設定
		pVtx[i + 1].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[i + 1].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	float fYRot = ((D3DX_PI * 0.25f) / m_nVertical);

	float fYPos = cosf(fYRot) * MAX_SIZE;

	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, fYPos, 0.0f);

	// 頂点座標の設定
	pVtx[0].pos = pos;

	D3DXVECTOR3 vec;

	// 正規化する ( 大きさ 1 のベクトルにする )
	D3DXVec3Normalize(&vec, &pos);

	// 各頂点の法線の設定
	pVtx[0].nor = vec;

	// 頂点カラーの設定
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuffCone->Unlock();
}
