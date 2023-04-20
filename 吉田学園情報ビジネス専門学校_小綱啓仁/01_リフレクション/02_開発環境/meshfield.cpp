//==================================================
// meshfilde.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "manager.h"
#include "input.h"

#include "game.h"
#include "utility.h"
#include "debug_proc.h"

#include "meshfield.h"

//**************************************************
// マクロ定義
//**************************************************
#define SIN_SIZE				(-500.0f)
#define SIN_ROT					(3.5f)

#define ACCEL_FACTOR			(9.8f / 4.0f)

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
CMeshField::CMeshField(int nPriority /* =1 */) : CObject(nPriority)
{
	// タイプ設定
	SetType(TYPE_ROAD);
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CMeshField::~CMeshField()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CMeshField::Init()
{
	{// 初期化
	 // ワールドマトリックス
		D3DXMatrixIdentity(&m_mtxWorld);
		// 頂点バッファへのポインタ
		m_pVtxBuff = nullptr;
		// インデックスバッファへのポインタ
		m_pIdxBuff = nullptr;
		// 位置
		m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 移動量
		m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 回転
		m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// サイズ
		m_size = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 高さ
		m_height = 0.0f;
		// 回転角度
		m_rotationAngle = 0.0f;
		// 目的の回転角度
		m_rotationAngleDest = 0.0f;
		// 姿勢回転軸ベクトル
		m_postureVec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 目的の姿勢回転軸ベクトル
		m_postureVecDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		// 一辺のの面数
		m_nMeshOneSideNumber = 0;
		// メッシュの頂点数
		m_nMeshVtx = 0;
		// メッシュのインデックス数
		m_nMeshIdx = 0;
		// メッシュのポリゴン数
		m_nMeshPrimitive = 0;
		// メッシュの大きさX
		m_fMeshSizeX = 0.0f;
		// メッシュの大きさY
		m_fMeshSizeY = 0.0f;
		// メッシュの大きさZ
		m_fMeshSizeZ = 0.0f;
		// 一辺の個数（おなか）
		m_nStomachMeshNumber = 0;
	}

	SetTexture(CTexture::TEXTURE_NONE);

	SetStomach(201, 2.0f, 2.0f);

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CMeshField::Uninit()
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
void CMeshField::Update()
{
	NorCalculation_();

	ShakeMeigen();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CMeshField::Draw(DRAW_MODE /*drawMode*/)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();
	CTexture* pTexture = CManager::GetManager()->GetTexture();

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// カリングを両面
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映								↓rotの情報を使って回転行列を作る
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);		//行列掛け算関数		第二引数 * 第三引数 を　第一引数に格納

	// 位置を反映								↓posの情報を使って移動行列を作る
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定VERTEX_3D
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, pTexture->GetTexture(m_texture));

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nMeshVtx, 0, m_nMeshPrimitive);

	// ステンシルバッファ -> 有効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);

	// ステンシルバッファと比較する参照値設定 -> ref
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);

	// ステンシルバッファの値に対してのマスク設定 -> 0xff(全て真)
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	// ステンシルテストの比較方法 ->
	// （参照値 >= ステンシルバッファの参照値）なら合格
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// ステンシルテストの結果に対しての反映設定
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);			// Zとステンシル成功
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);			// Zとステンシル失敗
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);			// Zのみ失敗
	
	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, m_nMeshVtx, 0, m_nMeshPrimitive);

	// テクスチャの解除
	pDevice->SetTexture(0, NULL);

	// カリングを両面
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ステンシルバッファ -> 無効
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CMeshField* CMeshField::Create()
{
	CMeshField *pMeshField;
	pMeshField = new CMeshField;

	if (pMeshField != nullptr)
	{
		pMeshField->Init();
	}
	else
	{
		assert(false);
	}

	return pMeshField;
}

//--------------------------------------------------
// 当たり判定
//--------------------------------------------------
bool CMeshField::CollisionMesh(D3DXVECTOR3 *pPos)
{
	bool IsLanding = false;

	int primitive = m_nMeshPrimitive;
	VERTEX_3D* pVtx = NULL;			// 頂点情報へのポインタ
	WORD* pIdx;
	const int nTri = 3;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < primitive; nCnt++)
	{
		D3DXVECTOR3 posPoly[nTri];

		// 頂点座標の取得
		posPoly[0] = pVtx[pIdx[nCnt + 0]].pos;
		posPoly[1] = pVtx[pIdx[nCnt + 1]].pos;
		posPoly[2] = pVtx[pIdx[nCnt + 2]].pos;

		if ((pIdx[nCnt + 0] == pIdx[nCnt + 1]) ||
			(pIdx[nCnt + 0] == pIdx[nCnt + 2]) ||
			(pIdx[nCnt + 2] == pIdx[nCnt + 1]))
		{// 縮退ポリゴンを飛ばす
			continue;
		}

		// ローカルからワールドに変換
		D3DXVec3TransformCoord(&posPoly[0], &posPoly[0], &m_mtxWorld);
		D3DXVec3TransformCoord(&posPoly[1], &posPoly[1], &m_mtxWorld);
		D3DXVec3TransformCoord(&posPoly[2], &posPoly[2], &m_mtxWorld);

		D3DXVECTOR3 vecLine[nTri];

		// 頂点座標の取得
		vecLine[0] = posPoly[1] - posPoly[0];
		vecLine[1] = posPoly[2] - posPoly[1];
		vecLine[2] = posPoly[0] - posPoly[2];

		D3DXVECTOR3 vecPlayer[nTri];

		// 頂点座標の取得
		vecPlayer[0] = *pPos - posPoly[0];
		vecPlayer[1] = *pPos - posPoly[1];
		vecPlayer[2] = *pPos - posPoly[2];

		float InOut[nTri];

		InOut[0] = Vec2Cross(&vecLine[0], &vecPlayer[0]);
		InOut[1] = Vec2Cross(&vecLine[1], &vecPlayer[1]);
		InOut[2] = Vec2Cross(&vecLine[2], &vecPlayer[2]);

		if ((InOut[0] >= 0 && InOut[1] >= 0 && InOut[2] >= 0)
			|| (InOut[0] <= 0 && InOut[1] <= 0 && InOut[2] <= 0))
		{
			D3DXVECTOR3 V1 = posPoly[1] - posPoly[0];
			D3DXVECTOR3 V2 = posPoly[2] - posPoly[0];

			// 結果の箱
			D3DXVECTOR3 vecNormal;
			// メッシュの法線を求める
			D3DXVec3Cross(&vecNormal, &V1, &V2);
			// 大きさを１にする
			D3DXVec3Normalize(&vecNormal, &vecNormal);

			if (nCnt % 2 == 1)
			{// 法線ベクトルの向きを正す
				vecNormal *= -1;
			}

			// 当たったオブジェクトの位置を設定
			float meshHeight = posPoly[0].y - (vecNormal.x * (pPos->x - posPoly[0].x) + vecNormal.z * (pPos->z - posPoly[0].z)) / vecNormal.y;

			if (pPos->y < meshHeight)
			{// メッシュの高さよりプレイヤーの高さのほうが下のとき
				pPos->y = meshHeight;
			}
			else
			{
				return IsLanding;
			}

			IsLanding = true;
		}
	}

	// インデックスバッファのアンロック
	m_pVtxBuff->Unlock();
	// 頂点バッファをアンロックする
	m_pIdxBuff->Unlock();

	return IsLanding;
}

//--------------------------------------------------
// 影の高さ設定
//--------------------------------------------------
bool CMeshField::CollisionShadow(D3DXVECTOR3 *pPos, float *height)
{
	bool bIsLanding = false;

	int primitive = m_nMeshPrimitive;
	VERTEX_3D* pVtx = NULL;			// 頂点情報へのポインタ
	WORD* pIdx;
	const int nTri = 3;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < primitive; nCnt++)
	{
		D3DXVECTOR3 posPoly[nTri];

		// 頂点座標の取得
		posPoly[0] = pVtx[pIdx[nCnt + 0]].pos;
		posPoly[1] = pVtx[pIdx[nCnt + 1]].pos;
		posPoly[2] = pVtx[pIdx[nCnt + 2]].pos;

		if ((pIdx[nCnt + 0] == pIdx[nCnt + 1]) ||
			(pIdx[nCnt + 0] == pIdx[nCnt + 2]) ||
			(pIdx[nCnt + 2] == pIdx[nCnt + 1]))
		{// 縮退ポリゴンを飛ばす
			continue;
		}

		// ローカルからワールドに変換
		D3DXVec3TransformCoord(&posPoly[0], &posPoly[0], &m_mtxWorld);
		D3DXVec3TransformCoord(&posPoly[1], &posPoly[1], &m_mtxWorld);
		D3DXVec3TransformCoord(&posPoly[2], &posPoly[2], &m_mtxWorld);

		D3DXVECTOR3 vecLine[nTri];

		// 頂点座標の取得
		vecLine[0] = posPoly[1] - posPoly[0];
		vecLine[1] = posPoly[2] - posPoly[1];
		vecLine[2] = posPoly[0] - posPoly[2];

		D3DXVECTOR3 vecPlayer[nTri];

		// 頂点座標の取得
		vecPlayer[0] = *pPos - posPoly[0];
		vecPlayer[1] = *pPos - posPoly[1];
		vecPlayer[2] = *pPos - posPoly[2];

		float InOut[nTri];

		InOut[0] = Vec2Cross(&vecLine[0], &vecPlayer[0]);
		InOut[1] = Vec2Cross(&vecLine[1], &vecPlayer[1]);
		InOut[2] = Vec2Cross(&vecLine[2], &vecPlayer[2]);

		if ((InOut[0] >= 0 && InOut[1] >= 0 && InOut[2] >= 0)
			|| (InOut[0] <= 0 && InOut[1] <= 0 && InOut[2] <= 0))
		{
			D3DXVECTOR3 V1 = posPoly[1] - posPoly[0];
			D3DXVECTOR3 V2 = posPoly[2] - posPoly[0];

			// 結果の箱
			D3DXVECTOR3 vecNormal;
			// メッシュの法線を求める
			D3DXVec3Cross(&vecNormal, &V1, &V2);
			// 大きさを１にする
			D3DXVec3Normalize(&vecNormal, &vecNormal);

			if (nCnt % 2 == 1)
			{// 法線ベクトルの向きを正す
				vecNormal *= -1;
			}

			// 当たったオブジェクトの位置を設定
			float meshHeight = posPoly[0].y - (vecNormal.x * (pPos->x - posPoly[0].x) + vecNormal.z * (pPos->z - posPoly[0].z)) / vecNormal.y;
			*height = meshHeight;

			bIsLanding = true;
		}
	}

	// インデックスバッファのアンロック
	m_pVtxBuff->Unlock();
	// 頂点バッファをアンロックする
	m_pIdxBuff->Unlock();

	return bIsLanding;
}

//--------------------------------------------------
// おなかタッチ
//--------------------------------------------------
void CMeshField::Touch()
{
	// ランダムで出す
	float randVtxX = FloatRandam(200.0f, -200.0f);
	float randVtxY = FloatRandam(200.0f, -200.0f);

	// タッチした座標の保存
	m_touchPos = D3DXVECTOR3(randVtxX, randVtxY, 0.0f);
}

//--------------------------------------------------
// 読み込み
//--------------------------------------------------
void CMeshField::Load(const char *pFileName)
{
	// ファイルオープン
	std::ifstream ifs(pFileName);

	if (ifs)
	{// 開けたら
		ifs >> m_JMesh;

		// 位置
		D3DXVECTOR3 pos(0.0f, 0.0f, 0.0f);

		// 位置の読み込み
		m_pos = D3DXVECTOR3(m_JMesh["WORLD_POS"]["POS"]["X"], m_JMesh["WORLD_POS"]["POS"]["Y"], m_JMesh["WORLD_POS"]["POS"]["Z"]);

		// 角度の読み込み
		m_rot = D3DXVECTOR3(m_JMesh["WORLD_ROT"]["ROT"]["X"], m_JMesh["WORLD_ROT"]["ROT"]["Y"], m_JMesh["WORLD_ROT"]["ROT"]["Z"]);

		// 一辺の面数の読み込み
		m_nMeshOneSideNumber = m_JMesh["MESH_ONESIDE_NUMBER"];

		// 面のサイズ X Z
		m_fMeshSizeX = m_JMesh["MESH_SIZEX"];
		m_fMeshSizeY = m_JMesh["MESH_SIZEZ"];

		MeshVtxAndIdxCalculation_();

		VERTEX_3D* pVtx = NULL;
		// 頂点座標をロック
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCnt = 0; nCnt < m_nMeshVtx; nCnt++)
		{
			// JSONファイルタグ設定
			std::string name = "VTX";
			std::string Number = std::to_string(nCnt);
			name += Number;

			// 位置の読み込み
			pos = D3DXVECTOR3(m_JMesh[name]["POS"]["X"], m_JMesh[name]["POS"]["Y"], m_JMesh[name]["POS"]["Z"]);

			// 座標の設定
			pVtx[nCnt].pos = D3DXVECTOR3(pos.x, pos.y, pos.z);

			// 各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
			pVtx[nCnt].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// 頂点カラーの設定
			pVtx[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// 頂点座標をアンロック
		m_pVtxBuff->Unlock();
	}

	//計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映								↓rotの情報を使って回転行列を作る
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);		//行列掛け算関数		第二引数 * 第三引数 を　第一引数に格納

	// 位置を反映								↓posの情報を使って移動行列を作る
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// 法線の計算
	NorCalculation_();
}

//--------------------------------------------------
// おなかメッシュの作成
//--------------------------------------------------
void CMeshField::SetStomach(int nStomachMeshNumber, float fMeshSizeX, float fMeshSizeY)
{
	StomachVtxAndIdxCalculation_(nStomachMeshNumber, fMeshSizeX, fMeshSizeY);
	// 法線の計算
	NorCalculation_();
}
//--------------------------------------------------
// メッシュの頂点とインデックスの計算
//--------------------------------------------------
void CMeshField::ShakeMeigen()
{
	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int centerLine = m_nStomachMeshNumber / 2;

	for (int y = 0; y < (m_nStomachMeshNumber + 1); y++)
	{
		float radiusY = ((m_fMeshSizeY * centerLine) * (m_fMeshSizeY * centerLine));
		float baseY = (float)(((y - centerLine) * m_fMeshSizeY) * ((y - centerLine) * m_fMeshSizeY));

		float heightY = sqrtf(radiusY - baseY);

		for (int x = 0; x < (m_nStomachMeshNumber + 1); x++)
		{
			float radiusX = ((m_fMeshSizeX * centerLine) * (m_fMeshSizeX * centerLine));
			float baseX = (float)(((x - centerLine) * m_fMeshSizeX) * ((x - centerLine) * m_fMeshSizeX));

			float heightX = sqrtf(radiusX - baseX);

			float answer = -(heightX * heightY / m_nStomachMeshNumber);

			pVtx[x + y * (m_nStomachMeshNumber + 1)].pos.x = (x - m_nStomachMeshNumber * 0.5f) * m_fMeshSizeX;
			pVtx[x + y * (m_nStomachMeshNumber + 1)].pos.y = (-y + m_nStomachMeshNumber * 0.5f) * m_fMeshSizeY;
			pVtx[x + y * (m_nStomachMeshNumber + 1)].pos.z = answer;
			pVtx[x + y * (m_nStomachMeshNumber + 1)].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[x + y * (m_nStomachMeshNumber + 1)].col = D3DXCOLOR(1.0f, 0.8f, 0.6f, 1.0f);
			pVtx[x + y * (m_nStomachMeshNumber + 1)].tex = D3DXVECTOR2((float)10.0f / m_nStomachMeshNumber * x, (float)10.0f / m_nStomachMeshNumber * y);


		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//--------------------------------------------------
// メッシュの頂点とインデックスの計算
//--------------------------------------------------
void CMeshField::MeshVtxAndIdxCalculation_()
{
	// メッシュの頂点数
	m_nMeshVtx = (((m_nMeshOneSideNumber)+1) * ((m_nMeshOneSideNumber)+1));
	// メッシュのインデックス数
	m_nMeshIdx = ((m_nMeshOneSideNumber + 1) * 2 * m_nMeshOneSideNumber + (m_nMeshOneSideNumber - 1) * 2);
	// メッシュのポリゴン数 
	m_nMeshPrimitive = ((m_nMeshOneSideNumber) * (m_nMeshOneSideNumber) * 2 + 4 * ((m_nMeshOneSideNumber)-1));

	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nMeshVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nMeshIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int z = 0; z < (m_nMeshOneSideNumber + 1); z++)
	{
		for (int x = 0; x < (m_nMeshOneSideNumber + 1); x++)
		{
			pVtx[x + z * (m_nMeshOneSideNumber + 1)].pos = D3DXVECTOR3(x * m_fMeshSizeX, 0.0f, -z * m_fMeshSizeZ);
			pVtx[x + z * (m_nMeshOneSideNumber + 1)].pos.x -= m_nMeshOneSideNumber * m_fMeshSizeX * 0.5f;
			//pVtx[x + z * (m_nMeshOneSideNumber + 1)].pos.y = sinf(x * D3DXToRadian(SIN_ROT) + m_height) * SIN_SIZE;
			pVtx[x + z * (m_nMeshOneSideNumber + 1)].pos.z += m_nMeshOneSideNumber * m_fMeshSizeZ * 0.5f;
			pVtx[x + z * (m_nMeshOneSideNumber + 1)].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[x + z * (m_nMeshOneSideNumber + 1)].col = D3DXCOLOR(1.0f, 1.0f, 0.7f, 1.0f);
			pVtx[x + z * (m_nMeshOneSideNumber + 1)].tex = D3DXVECTOR2((float)10.0f / m_nMeshOneSideNumber * x, (float)10.0f / m_nMeshOneSideNumber * z);
		}
	}

	WORD* pIdx;

	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int z = 0; z < m_nMeshOneSideNumber; z++)
	{// zの方向
		int nLineTop = z * ((m_nMeshOneSideNumber + 1) * 2 + 2);
		for (int x = 0; x < m_nMeshOneSideNumber + 1; x++)
		{// xの方向
		 // 偶数番目
			pIdx[(x * 2) + nLineTop] = (WORD)((m_nMeshOneSideNumber + 1) + x + z * (m_nMeshOneSideNumber + 1));
			// 奇数番目
			pIdx[(x * 2 + 1) + nLineTop] = (WORD)(pIdx[(x * 2) + nLineTop] - (m_nMeshOneSideNumber + 1));
		}
		// 縮退ポリゴン(数が連続する時)
		if (z < m_nMeshOneSideNumber - 1)
		{
			pIdx[(m_nMeshOneSideNumber + 1) * 2 + nLineTop] = (WORD)(m_nMeshOneSideNumber + (m_nMeshOneSideNumber + 1) * z);
			pIdx[(m_nMeshOneSideNumber + 1) * 2 + 1 + nLineTop] = (WORD)((m_nMeshOneSideNumber + 1) * (2 + z));
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	// インデックスバッファのアンロック
	m_pIdxBuff->Unlock();
}

//--------------------------------------------------
// おなかのメッシュ
//--------------------------------------------------
void CMeshField::StomachVtxAndIdxCalculation_(int nStomachMeshNumber, float fMeshSizeX, float fMeshSizeY)
{
	m_nStomachMeshNumber = nStomachMeshNumber;
	m_fMeshSizeX = fMeshSizeX;
	m_fMeshSizeY = fMeshSizeY;

	// メッシュの頂点数
	m_nMeshVtx = (((m_nStomachMeshNumber) + 1) * ((m_nStomachMeshNumber) + 1));
	// メッシュのインデックス数
	m_nMeshIdx = ((m_nStomachMeshNumber + 1) * 2 * m_nStomachMeshNumber + (m_nStomachMeshNumber - 1) * 2);
	// メッシュのポリゴン数 
	m_nMeshPrimitive = ((m_nStomachMeshNumber) * (m_nStomachMeshNumber) * 2 + 4 * ((m_nStomachMeshNumber)-1));

	// デバイスのポインタ
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * m_nMeshVtx,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * m_nMeshIdx,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	int centerLine = m_nStomachMeshNumber / 2;

	for (int y = 0; y < (m_nStomachMeshNumber + 1); y++)
	{
		float radiusY = ((m_fMeshSizeY * centerLine) * (m_fMeshSizeY * centerLine));
		float baseY = (float)(((y - centerLine) * m_fMeshSizeY) * ((y - centerLine) * m_fMeshSizeY));

		float heightY = sqrtf(radiusY - baseY);

		for (int x = 0; x < (m_nStomachMeshNumber + 1); x++)
		{
			float radiusX = ((m_fMeshSizeX * centerLine) * (m_fMeshSizeX * centerLine));
			float baseX = (float)(((x - centerLine) * m_fMeshSizeX) * ((x - centerLine) * m_fMeshSizeX));

			float heightX = sqrtf(radiusX - baseX);

			float answer = - (heightX * heightY / m_nStomachMeshNumber);

			pVtx[x + y * (m_nStomachMeshNumber + 1)].pos.x = (x - m_nStomachMeshNumber * 0.5f) * m_fMeshSizeX;
			pVtx[x + y * (m_nStomachMeshNumber + 1)].pos.y = (-y + m_nStomachMeshNumber * 0.5f) * m_fMeshSizeY;
			pVtx[x + y * (m_nStomachMeshNumber + 1)].pos.z = answer;
			pVtx[x + y * (m_nStomachMeshNumber + 1)].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pVtx[x + y * (m_nStomachMeshNumber + 1)].col = D3DXCOLOR(1.0f, 0.8f, 0.6f, 1.0f);
			pVtx[x + y * (m_nStomachMeshNumber + 1)].tex = D3DXVECTOR2((float)10.0f / m_nStomachMeshNumber * x, (float)10.0f / m_nStomachMeshNumber * y);
		}
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	WORD* pIdx;

	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int y = 0; y < m_nStomachMeshNumber; y++)
	{// zの方向
		int nLineTop = y * ((m_nStomachMeshNumber + 1) * 2 + 2);
		for (int x = 0; x < m_nStomachMeshNumber + 1; x++)
		{// xの方向
		 // 偶数番目
			pIdx[(x * 2) + nLineTop] = (WORD)((m_nStomachMeshNumber + 1) + x + y * (m_nStomachMeshNumber + 1));
			// 奇数番目
			pIdx[(x * 2 + 1) + nLineTop] = (WORD)(pIdx[(x * 2) + nLineTop] - (m_nStomachMeshNumber + 1));
		}
		// 縮退ポリゴン(数が連続する時)
		if (y < m_nStomachMeshNumber - 1)
		{
			pIdx[(m_nStomachMeshNumber + 1) * 2 + nLineTop] = (WORD)(m_nStomachMeshNumber + (m_nStomachMeshNumber + 1) * y);
			pIdx[(m_nStomachMeshNumber + 1) * 2 + 1 + nLineTop] = (WORD)((m_nStomachMeshNumber + 1) * (2 + y));
		}
	}
	// インデックスバッファのアンロック
	m_pIdxBuff->Unlock();
}

//--------------------------------------------------
// 法線の計算
//--------------------------------------------------
void CMeshField::NorCalculation_()
{
	// 三角形の頂点数
	const int nTri = 3;
	// 名前思いつかない
	D3DXVECTOR3 posCorner[nTri];
	// primitiveを保存
	int primitive = m_nMeshPrimitive;

	// 頂点情報へのポインタ
	VERTEX_3D* pVtx = NULL;
	WORD* pIdx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	// インデックスバッファをロック
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	for (int i = 0; i < m_nMeshVtx; i++)
	{// 頂点数分初期化
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}
	for (int nCnt = 0; nCnt < primitive; nCnt++)
	{// 各頂点の座標を取得
		posCorner[0] = pVtx[pIdx[nCnt + 0]].pos;
		posCorner[1] = pVtx[pIdx[nCnt + 1]].pos;
		posCorner[2] = pVtx[pIdx[nCnt + 2]].pos;

		if ((pIdx[nCnt + 0] == pIdx[nCnt + 1]) ||
			(pIdx[nCnt + 0] == pIdx[nCnt + 2]) ||
			(pIdx[nCnt + 2] == pIdx[nCnt + 1]))
		{// 縮退ポリゴンをとばす
			continue;
		}

		// ポリドンから二つのベクトルを取る
		D3DXVECTOR3 V1 = posCorner[1] - posCorner[0];
		D3DXVECTOR3 V2 = posCorner[2] - posCorner[0];

		// 求めた法線を格納する箱
		D3DXVECTOR3 vecNormal;

		if (nCnt % 2 == 0)
		{
			// メッシュの法線を求める
			D3DXVec3Cross(&vecNormal, &V1, &V2);
		}
		else
		{
			// メッシュの法線を求める
			D3DXVec3Cross(&vecNormal, &V2, &V1);
		}

		// 大きさを１にする
		D3DXVec3Normalize(&vecNormal, &vecNormal);

		for (int i = 0; i < nTri; i++)
		{// 求めた法線を各頂点に設定
			pVtx[pIdx[nCnt + i]].nor += vecNormal;
		}
	}

	for (int nCnt = 0; nCnt < m_nMeshVtx; nCnt++)
	{// すべての法線を正規化する
		D3DXVec3Normalize(&pVtx[nCnt].nor, &pVtx[nCnt].nor);
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	// インデックスバッファのアンロック
	m_pIdxBuff->Unlock();
}
