//==================================================
// utility.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include "utility.h"
#include "manager.h"
#include "camera.h"

//--------------------------------------------------
// ワールド座標からスクリーン座標に変更する関数
//--------------------------------------------------
D3DXVECTOR3 GetWorldToScreenPos(const D3DXVECTOR3& pos)
{
	// https://dxlib.xsrv.jp/cgi/patiobbs/patio.cgi?mode=view&no=3145

	D3DXMATRIX mtx;

	// 位置を反映
	D3DXMatrixTranslation(&mtx, pos.x, pos.y, pos.z);

	// ビュー変換とプロジェクション変換
	mtx = mtx * CManager::GetManager()->GetCamera()->GetViewMatrix() * CManager::GetManager()->GetCamera()->GetProjMatrix();

	// 座標を取得
	D3DXVECTOR3 pOut(mtx.m[3]);

	// zで割って-1~1の範囲に収める
	pOut /= pOut.z;

	// スクリーン行列
	D3DXMATRIX viewport;
	D3DXMatrixIdentity(&viewport);
	viewport._11 = CManager::SCREEN_WIDTH / 2.0f; 
	viewport._22 = -CManager::SCREEN_HEIGHT / 2.0f;
	viewport._41 = CManager::SCREEN_WIDTH / 2.0f;
	viewport._42 = CManager::SCREEN_HEIGHT / 2.0f;

	// スクリーン変換
	D3DXVec3TransformCoord(&pOut, &pOut, &viewport);

	return pOut;

#if 0
	// http://yamatyuu.net/computer/program/directx9/3dview/index.html

	D3DVIEWPORT9 viewport;
	viewport.X = 0.0f;
	viewport.Y = 0.0f;

	// ビューポートの幅
	viewport.Width = CManager::SCREEN_WIDTH;
	// ビューポートの高さ
	viewport.Height = CManager::SCREEN_HEIGHT;
	// ビューポート深度設定
	viewport.MinZ = 0.0f;
	viewport.MaxZ = 1.0f;

	D3DXMATRIX view, proj, mtxWorld;

	// ビュー行列と射影行列の取得
	view = CManager::GetManager()->GetCamera()->GetViewMatrix();
	proj = CManager::GetManager()->GetCamera()->GetProjMatrix();
	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&mtxWorld);

	D3DXVECTOR3 pOut;
	D3DXVec3Project(&pOut, &pos, &viewport, &proj, &view, &mtxWorld);

	return pOut;
#endif
}

//--------------------------------------------------
// 2Dベクトルの外積
//--------------------------------------------------
float Vec2Cross(D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	return v1->x * v2->z - v1->z * v2->x;
}

//--------------------------------------------------
// 2Dベクトルの内積
//--------------------------------------------------
float D3DXVec2Dot(D3DXVECTOR3* v1, D3DXVECTOR3* v2)
{
	return v1->x * v2->x + v1->z * v2->z;
}

//---------------------------------------------------------------------------
// 小数点のランダム
//---------------------------------------------------------------------------
float FloatRandam(float fMax, float fMin)
{
	return ((rand() / (float)RAND_MAX) * (fMax - fMin)) + fMin;
}

//---------------------------------------------------------------------------
// 反射ベクトル（front : 進行ベクトル  normal : 衝突点での法線ベクトル）
//---------------------------------------------------------------------------
D3DXVECTOR3 CalcReflectVector(const D3DXVECTOR3 &front, const D3DXVECTOR3 &normal)
{
	// 保存用の箱
	D3DXVECTOR3 nor;
	D3DXVec3Normalize(&nor, &normal);

	// 反射ベクトルの保存
	D3DXVECTOR3 reflectVector = (front - 2.0f * D3DXVec3Dot(&front, &nor) * nor);

	// 反射の計算
	D3DXVec3Normalize(&reflectVector, &reflectVector);

	return reflectVector;
}
