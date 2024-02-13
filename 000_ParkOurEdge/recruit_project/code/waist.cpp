//==========================================================
//
// 仮想の腰処理全般 [waist.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "waist.h"
#include "manager.h"
#include "renderer.h"

//==========================================================
// コンストラクタ
//==========================================================
CWaist::CWaist()
{
	// 値のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 位置
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);		// 向き
	m_pParentMtx = NULL;						// 親のワールドマトリックス
}

//==========================================================
// デストラクタ
//==========================================================
CWaist::~CWaist()
{

}

//==========================================================
// マトリックスの設定
//==========================================================
void CWaist::SetMatrix(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイスへのポインタを取得
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス
	D3DXMATRIX mtxParent;			// 親のマトリックス情報

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if (m_pParentMtx != NULL)
	{// 覚えている場合

		mtxParent = *m_pParentMtx;

		//パーツのマトリックスと親のマトリックスをかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld, &mtxParent);
	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
}

//==========================================================
// 生成
//==========================================================
CWaist *CWaist::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CWaist *pWaist = NULL;	// 腰のポインタ

	// 動的確保
	pWaist = new CWaist;

	if (pWaist != NULL)
	{// 確保された場合
		pWaist->SetPosition(pos);
		pWaist->SetRotation(rot);
	}

	return pWaist;
}

//==========================================================
// 腰の高さを設定
//==========================================================
void CWaist::SetHeight(D3DXVECTOR3& pPos)
{
	// 座標を設定
	m_SetPos = pPos;

	pPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}
