//==========================================================
//
// 敵の処理 [enemy.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "enemy.h"
#include "enemy_manager.h"
#include "debugproc.h"
#include "manager.h"

//==========================================================
// コンストラクタ
//==========================================================
CEnemy::CEnemy()
{
	// 値のクリア
	m_Info.pos = { 0.0f, 0.0f, 0.0f };
	m_Info.rot = { 0.0f, 0.0f, 0.0f };
	m_Info.move = { 0.0f, 0.0f, 0.0f };
	m_Info.posOld = { 0.0f, 0.0f, 0.0f };
	m_nLife = 3;

	// リストに挿入
	CEnemyManager::GetInstance()->ListIn(this);
}

//==========================================================
// デストラクタ
//==========================================================
CEnemy::~CEnemy()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CEnemy::Init(void)
{
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CEnemy::Uninit(void)
{
	// リストから外す
	CEnemyManager::GetInstance()->ListOut(this);
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CEnemy::Update(void)
{
	// マトリックス設定
	SetMatrix();

	CManager::GetInstance()->GetDebugProc()->Print("敵の体力 [ %d ]\n", m_nLife);
}

//===============================================
// マトリックス設定
//===============================================
void CEnemy::SetMatrix(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);
}

//===============================================
// ヒット処理
//===============================================
void CEnemy::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage)
{

}

//===============================================
// 基本情報リセット
//===============================================
void CEnemy::InfoReset(void)
{
	// 値のクリア
	m_Info.pos = { 0.0f, 0.0f, 0.0f };
	m_Info.rot = { 0.0f, 0.0f, 0.0f };
	m_Info.move = { 0.0f, 0.0f, 0.0f };
	m_Info.posOld = { 0.0f, 0.0f, 0.0f };
}