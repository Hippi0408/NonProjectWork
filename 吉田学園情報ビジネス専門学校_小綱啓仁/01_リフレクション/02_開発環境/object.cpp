//==================================================
// object.cpp
// Author: Buriya Kota
//==================================================

//**************************************************
// include
//**************************************************
#include <time.h>
#include <assert.h>

#include "object.h"
#include "object2D.h"
#include "renderer.h"

//**************************************************
// 静的メンバ変数
//**************************************************
int CObject::m_nNumAll = 0;
CObject *CObject::m_pTop[] = {};
CObject *CObject::m_pCurrent[] = {};

//--------------------------------------------------
// プライオリティを使ったコンストラクタ
//--------------------------------------------------
CObject::CObject(int nPriority /* PRIORITY_3 */) : m_pPrev(nullptr), m_pNext(nullptr)
{
	m_bDeleted = false;

	if (m_pTop[nPriority] == nullptr)
	{// Topがnullptrの時
		m_pTop[nPriority] = this;
	}
	else
	{// Topがすでに生成されているとき
		m_pCurrent[nPriority]->m_pNext = this;
		this->m_pPrev = m_pCurrent[nPriority];
	}

	m_pCurrent[nPriority] = this;

	m_nPriority = nPriority;
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CObject::~CObject()
{
}

//--------------------------------------------------
// すべての破棄
//--------------------------------------------------
void CObject::ReleaseAll()
{
	for (int nCnt = 0; nCnt < MAX_PRIO; nCnt++)
	{
		CObject *pCurrentObj = m_pTop[nCnt];

		while (pCurrentObj != nullptr)
		{
			CObject *pObjNext = pCurrentObj->m_pNext;

			if (!pCurrentObj->IsDeleted())
			{
				pCurrentObj->Uninit();
			}

			pCurrentObj = pObjNext;
		}
	}
	//deleted == true を delete & null代入
	for (int nCnt = 0; nCnt < MAX_PRIO; nCnt++)
	{
		CObject *pCurrentObj = m_pTop[nCnt];

		while (pCurrentObj != nullptr)
		{
			CObject *pObjNext = pCurrentObj->m_pNext;

			if (pCurrentObj->IsDeleted())
			{
				pCurrentObj->Release();
			}
			else
			{
				assert(false);
			}

			pCurrentObj = pObjNext;
		}

		m_pTop[nCnt] = nullptr;
		m_pCurrent[nCnt] = nullptr;
	}
}

//--------------------------------------------------
// すべての更新
//--------------------------------------------------
void CObject::UpdateAll()
{
	for (int nCnt = 0; nCnt < MAX_PRIO; nCnt++)
	{
		CObject *pCurrentObj = m_pTop[nCnt];

		while (pCurrentObj != nullptr)
		{
			CObject *pObjNext = pCurrentObj->m_pNext;

			if (!pCurrentObj->IsDeleted())
			{
				pCurrentObj->Update();
			}

			pCurrentObj = pObjNext;
		}
	}
	//deleted == true を delete & null代入
	for (int nCnt = 0; nCnt < MAX_PRIO; nCnt++)
	{
		CObject *pCurrentObj = m_pTop[nCnt];

		while (pCurrentObj != nullptr)
		{
			CObject *pObjNext = pCurrentObj->m_pNext;

			if (pCurrentObj->IsDeleted())
			{
				pCurrentObj->Release();
			}
			
			pCurrentObj = pObjNext;
		}
	}
}

//--------------------------------------------------
// すべての描画
//--------------------------------------------------
void CObject::DrawAll()
{
	for (int nCnt = 0; nCnt < MAX_PRIO; nCnt++)
	{
		switch (nCnt)
		{
		case PRIORITY_PLAYER:
		case PRIORITY_OBJECT:
			for (int nCntMode = 0; nCntMode < DRAW_MODE_MAX; nCntMode++)
			{
				if (m_pTop[nCnt] != nullptr)
				{
					CObject *pCurrentObj = m_pTop[nCnt];

					while (pCurrentObj != nullptr && !pCurrentObj->IsDeleted())
					{
						CObject *pObjNext = pCurrentObj->m_pNext;
						pCurrentObj->Draw((DRAW_MODE)nCntMode);
						pCurrentObj = pObjNext;
					}
				}
			}
			break;

		default:
			if (m_pTop[nCnt] != nullptr)
			{
				CObject *pCurrentObj = m_pTop[nCnt];

				while (pCurrentObj != nullptr && !pCurrentObj->IsDeleted())
				{
					CObject *pObjNext = pCurrentObj->m_pNext;
					pCurrentObj->Draw(DRAW_MODE_OBJECT);
					pCurrentObj = pObjNext;
				}
			}

			break;
		}
	}
}

//--------------------------------------------------
// モード以外をリリース
//--------------------------------------------------
void CObject::ReleaseWithoutMode()
{
	for (int nPriority = 0; nPriority < PRIORITY_MAX; nPriority++)
	{
		{
			CObject* pObject = m_pTop[nPriority];

			while (pObject != nullptr)
			{
				// pNextの保存
				CObject* pObjectNext = pObject->m_pNext;

				if (pObject->GetType() != TYPE_MODE)
				{
					pObject->Uninit();
				}

				// pObjectにpObjectのpNextを代入
				pObject = pObjectNext;
			}
		}

		{
			CObject* pObject = m_pTop[nPriority];

			while (pObject != nullptr)
			{
				// pNextの保存
				CObject* pObjectNext = pObject->m_pNext;

				if (pObject->GetType() != TYPE_MODE && pObject->IsDeleted())
				{
					// 終了処理の関数呼び出し
					pObject->Release();
				}

				// pObjectにpObjectのpNextを代入
				pObject = pObjectNext;
			}
		}
	}
}

//--------------------------------------------------
// deleteをtrueにする
//--------------------------------------------------
void CObject::DeletedObj()
{
	m_bDeleted = true;
}

//--------------------------------------------------
// リリース
//--------------------------------------------------
void CObject::Release()
{
	CObject *pDeleteObj = this;

	if (pDeleteObj == nullptr)
	{// 消そうとしてるやつが使われているかどうか
		return;
	}
	else if (m_pPrev == nullptr && m_pNext == nullptr)
	{// オブジェクトが秘湯しかないとき
		m_pTop[m_nPriority] = nullptr;
		m_pCurrent[m_nPriority] = nullptr;
	}
	else if (m_pPrev == nullptr)
	{// Topが消えた時
		m_pTop[m_nPriority] = pDeleteObj->m_pNext;
		m_pTop[m_nPriority]->m_pPrev = nullptr;
	}
	else if (m_pNext == nullptr)
	{// Currentが消えた時
		m_pCurrent[m_nPriority] = pDeleteObj->m_pPrev;
		m_pCurrent[m_nPriority]->m_pNext = nullptr;
	}
	else
	{// 間のオブジェクトが消えた時
		pDeleteObj->m_pPrev->m_pNext = pDeleteObj->m_pNext;
		pDeleteObj->m_pNext->m_pPrev = pDeleteObj->m_pPrev;
	}

	delete pDeleteObj;
}

//--------------------------------------------------
// 球の当たり判定
//--------------------------------------------------
bool IsCollisionSphere(D3DXVECTOR3 targetPos, D3DXVECTOR3 pos, D3DXVECTOR3 targetSize, D3DXVECTOR3 size)
{
	bool bIsLanding = false;

	// 目的の距離
	float fTargetDistance = sqrtf((targetPos.x - pos.x) * (targetPos.x - pos.x) +
		(targetPos.y - pos.y) * (targetPos.y - pos.y) +
		(targetPos.z - pos.z) * (targetPos.z - pos.z));
	// 現在の距離
	float fCurrentDistance = sqrtf((targetSize.x + size.x) * (targetSize.x + size.x) +
		(targetSize.y + size.y) * (targetSize.y + size.y) +
		(targetSize.z + size.z) * (targetSize.z + size.z));

	if (fTargetDistance < fCurrentDistance)
	{
		bIsLanding = true;
	}

	return bIsLanding;
}

//--------------------------------------------------
// 矩形の当たり判定
//--------------------------------------------------
bool IsCollision(D3DXVECTOR3 targetPos, D3DXVECTOR3 pos, D3DXVECTOR3 targetSize, D3DXVECTOR3 size)
{
	bool bIsLanding = false;

	if (pos.x + size.x / 2.0f >= targetPos.x - targetSize.x / 2.0f
		&& pos.x - size.x / 2.0f <= targetPos.x + targetSize.x / 2.0f
		&& pos.z - size.z / 2.0f <= targetPos.z + targetSize.z / 2.0f
		&& pos.z + size.z / 2.0f >= targetPos.z - targetSize.z / 2.0f)
	{
		bIsLanding = true;
	}

	return bIsLanding;
}

//--------------------------------------------------
// 円の当たり判定
//--------------------------------------------------
bool IsCollisionCircle(D3DXVECTOR3 pos1, float fRadius1, D3DXVECTOR3 pos2, float fRadius2)
{
	//位置の保存
	D3DXVECTOR3 Pos1 = pos1;
	D3DXVECTOR3 Pos2 = pos2;

	//２この物体の半径同士の和
	float fDiff = fRadius1 + fRadius2;

	//計算用変数
	float fCalculationX, fCalculationZ;

	//Xの差分
	fCalculationX = Pos1.x - Pos2.x;
	//Yの差分
	fCalculationZ = Pos1.y - Pos2.y;

	//現在の２点の距離
	float fLength = sqrtf(fCalculationX * fCalculationX + fCalculationZ * fCalculationZ);

	//２この物体の半径同士の和より現在の２点の距離が小さいかどうか
	if (fDiff >= fLength)
	{
		//当たった
		return true;
	}

	//当たってない
	return false;
}
