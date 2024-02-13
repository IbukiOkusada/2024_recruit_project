//==========================================================
//
// ギミック基底 [gimmick.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "gimmick.h"
#include "manager.h"
#include "renderer.h"

// 静的メンバ変数宣言
CGimmick *CGimmick::m_pTop = nullptr;	// 先頭のオブジェクトへのポインタ
CGimmick *CGimmick::m_pCur = nullptr;	// 最後尾のオブジェクトへのポインタ

//==========================================================
// コンストラクタ
//==========================================================
CGimmick::CGimmick()
{
	// 値のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_type = TYPE_NONE;

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	m_pNext = nullptr;
	m_pPrev = nullptr;

	// 自分自身をリストに追加
	if (m_pTop != nullptr)
	{// 先頭が存在している場合
		m_pCur->m_pNext = this;	// 現在最後尾のオブジェクトのポインタにつなげる
		m_pPrev = m_pCur;
		m_pCur = this;	// 自分自身が最後尾になる
	}
	else
	{// 存在しない場合
		m_pTop = this;	// 自分自身が先頭になる
		m_pCur = this;	// 自分自身が最後尾になる
	}
}

//==========================================================
// デストラクタ
//==========================================================
CGimmick::~CGimmick()
{
	// リストから外す
	ListOut();
}

//==========================================================
// マトリックスの設定
//==========================================================
void CGimmick::SetMtxWorld(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxParent;			// 親のマトリックス情報

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}

//==========================================================
// 当たり判定
//==========================================================
bool CGimmick::Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand)
{
	CGimmick *pObj = m_pTop;	// 先頭取得
	bool bValue = false;	// 着地したか否か

	while (pObj != nullptr)
	{
		CGimmick *pObjNext = pObj->m_pNext;
		if (pObj->CollisionCheck(pos, posOld, move, SetPos, vtxMin, vtxMax, nAction, ppGimmick, bLand))
		{
			bValue = true;
		}

		pObj = pObjNext;
	}

	return bValue;
}

//==========================================================
// 外積当たり判定
//==========================================================
bool CGimmick::CollisionCloss(D3DXVECTOR3 & pos, D3DXVECTOR3 & posOld, D3DXVECTOR3 * posCollisioned)
{
	CGimmick *pObj = m_pTop;	// 先頭取得
	D3DXVECTOR3 posNear = D3DXVECTOR3(FLT_MAX, 0.0f, 0.0f);
	bool bValue = false;	// ごっつんしたか

	while (pObj != nullptr)
	{
		CGimmick *pObjNext = pObj->m_pNext;
		D3DXVECTOR3 posObjColl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (pObj->CollisionCheckCloss(pos, posOld, &posObjColl))
		{
			float fLength = D3DXVec3Length(&(posOld - posObjColl));

			if (D3DXVec3Length(&(posOld - posNear)) > fLength)
			{
				posNear = posObjColl;
			}

			bValue = true;
		}

		pObj = pObjNext;
	}

	if (posCollisioned != nullptr)
	{
		*posCollisioned = posNear;
	}

	return bValue;
}

//==========================================================
// リストから外す
//==========================================================
void CGimmick::ListOut(void)
{
	// リストから自分自身を削除する
	if (m_pTop == this)
	{// 自身が先頭
		if (m_pNext != nullptr)
		{// 次が存在している
			m_pTop = m_pNext;	// 次を先頭にする
			m_pNext->m_pPrev = nullptr;	// 次の前のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else if (m_pCur == this)
	{// 自身が最後尾
		if (m_pPrev != nullptr)
		{// 次が存在している
			m_pCur = m_pPrev;			// 前を最後尾にする
			m_pPrev->m_pNext = nullptr;	// 前の次のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = nullptr;	// 先頭がない状態にする
			m_pCur = nullptr;	// 最後尾がない状態にする
		}
	}
	else
	{
		if (m_pNext != nullptr)
		{
			m_pNext->m_pPrev = m_pPrev;	// 自身の次に前のポインタを覚えさせる
		}
		if (m_pPrev != nullptr)
		{
			m_pPrev->m_pNext = m_pNext;	// 自身の前に次のポインタを覚えさせる
		}
	}
}

//==========================================================
// スイッチをオフにする
//==========================================================
void CGimmick::SwitchOff(void)
{
	CGimmick *pObj = m_pTop;	// 先頭取得

	while (pObj != nullptr)
	{
		CGimmick *pObjNext = pObj->m_pNext;
		pObj->Switch(false);
		pObj = pObjNext;
	}
}

//==========================================================
// スイッチをオンにする
//==========================================================
void CGimmick::SwitchOn(void)
{
	CGimmick *pObj = m_pTop;	// 先頭取得

	while (pObj != nullptr)
	{
		CGimmick *pObjNext = pObj->m_pNext;
		pObj->Switch(true);
		pObj = pObjNext;
	}
}

//=========================================================
// ボタンをオフにする
//==========================================================
void CGimmick::Buttonoff(void)
{
	CGimmick *pObj = m_pTop;	// 先頭取得

	while (pObj != nullptr)
	{
		CGimmick *pObjNext = pObj->m_pNext;

		if (pObj->GetButton() != nullptr) {
			pObj->Switch(false);
		}
		pObj = pObjNext;
	}
}
