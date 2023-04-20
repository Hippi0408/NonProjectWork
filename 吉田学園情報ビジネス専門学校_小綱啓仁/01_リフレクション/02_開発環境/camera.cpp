//==================================================
// camera.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "debug_proc.h"
#include "manager.h"
#include "game.h"
#include "input.h"
#include "input.h"

#include "camera.h"

//--------------------------------------------------
// コンストラクタ
//--------------------------------------------------
CCamera::CCamera()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CCamera::~CCamera()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void CCamera::Init()
{
	// 視点・注視点・上方向を設定する
	m_localPosV = D3DXVECTOR3(0.0f, 300.0f, -550.0f);
	m_localPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_worldPosV = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 視点
	m_worldPosR = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 注視点
	m_posVDest = D3DXVECTOR3(0.0f, 100.0f, -100.0f);	// 目的の視点
	m_posRDest = D3DXVECTOR3(0.0f, 0.0f, 0.0f);			// 目的の注視点
	m_vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	// 角度の初期化     
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CCamera::Update()
{
	CInput *pInput = CInput::GetKey();

	//視点の上下
	if (pInput->Press(DIK_T) /*|| pInput->VectorMoveJoyStick(0, true).y > 0.5f*/)
	{
		m_rot.x += 0.01f;
	}
	if (pInput->Press(DIK_G) /*|| pInput->VectorMoveJoyStick(0, true).y < -0.5f*/)
	{
		m_rot.x -= 0.01f;
	}

#ifdef _DEBUG
	//視点の旋回
	if (pInput->Press(DIK_Z))
	{
		m_rot.y += 0.01f;
	}
	if (pInput->Press(DIK_C))
	{
		m_rot.y -= 0.01f;
	}

	//視点の近づけ
	if (pInput->Press(DIK_Y) /*|| pInput->VectorMoveJoyStick(0, true).y > 0.5f*/)
	{
		m_localPosV.z += 1.0f;
	}
	if (pInput->Press(DIK_H) /*|| pInput->VectorMoveJoyStick(0, true).y < -0.5f*/)
	{
		m_localPosV.z -= 1.0f;
	}

#endif // DEBUG

	//角度の正規化
	if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	else if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	if (m_rot.x > D3DX_PI)
	{
		m_rot.x -= D3DX_PI * 2.0f;
	}
	else if (m_rot.x < -D3DX_PI)
	{
		m_rot.x += D3DX_PI * 2.0f;
	}

	// 計算用マトリックス
	D3DXMATRIX mtxRot, mtxTrans, mtxWorld;

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	// 向きを反映								↓rotの情報を使って回転行列を作る
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxRot);		//行列掛け算関数		第二引数 * 第三引数 を　第一引数に格納

	// 位置を反映								↓posの情報を使って移動行列を作る
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

	D3DXVec3TransformCoord(&m_worldPosV, &m_localPosV, &mtxWorld);
	D3DXVec3TransformCoord(&m_worldPosR, &m_localPosR, &mtxWorld);

#ifdef _DEBUG
	// デバッグ表示
	CDebugProc::Print("視点 : \n x : %f , y : %f , z : %f \n", m_worldPosV.x, m_worldPosV.y, m_worldPosV.z);
	CDebugProc::Print("注視点 : \n x : %f , y : %f , z : %f \n\n", m_worldPosR.x, m_worldPosR.y, m_worldPosR.z);
#endif // DEBUG
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CCamera::Set()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetManager()->GetRenderer()->GetDevice();

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&m_worldPosV,
		&m_worldPosR,
		&m_vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&m_mtxProjection,
		D3DXToRadian(45.0f),								// 視野角
		(float)CManager::SCREEN_WIDTH / (float)CManager::SCREEN_HEIGHT,			// アスペクト比
		10.0f,												// ニア
		40000.0f);											// ファー

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);
}
