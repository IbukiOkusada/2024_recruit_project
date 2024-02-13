//==========================================================
//
// タイトルエンター [title_enter.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "title_enter.h"
#include "manager.h"
#include "item.h"
#include "model.h"

// 静的メンバ変数宣言

//==========================================================
// コンストラクタ
//==========================================================
CTitleEnter::CTitleEnter()
{
	// 値のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//==========================================================
// デストラクタ
//==========================================================
CTitleEnter::~CTitleEnter()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CTitleEnter::Init(void)
{
	
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CTitleEnter::Uninit(void)
{
	
}

//==========================================================
// 更新処理
//==========================================================
void CTitleEnter::Update(void)
{
	// マトリックス設定
	SetMatrix();
}

//==========================================================
// マトリックス更新
//==========================================================
void CTitleEnter::SetMatrix(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}

//==========================================================
// アイテムの親設定
//==========================================================
void CTitleEnter::SetItemParent(CItem *pItem)
{
	if (pItem == nullptr) {	// オブジェクト無し
		return;
	}

	if (pItem->GetModel() == nullptr) {	// 所持モデル無し
		return;
	}

	// 自身のマトリックスを渡す
	pItem->GetModel()->SetParent(&m_mtxWorld);
}