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
#include "slow.h"

namespace {
	const float ROT_MULTI = (0.1f);	// 向き補正倍率
	const float BUMP_SIZE = (25.0f);
}

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
	m_nLife = 8;
	m_fRotMove = 0.0f;
	m_fRotDiff = 0.0f;
	m_fMoveIner = 0.0f;
	m_fRotMulti = ROT_MULTI;
	m_bLockOn = true;
	m_fBumpSize = BUMP_SIZE;

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
	// 現在の向きを獲得
	m_fRotMove = m_Info.rot.y;
	Adjust();

	// マトリックス設定
	SetMatrix();

	if (m_Info.pos.y <= -100.0f) {
		Uninit();
	}
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
bool CEnemy::Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage)
{
	return false;
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

//===============================================
// 調整
//===============================================
void CEnemy::Adjust(void)
{
	while (1)
	{
		if (m_fRotDiff > D3DX_PI || m_fRotDiff < -D3DX_PI)
		{//-3.14～3.14の範囲外の場合
			if (m_fRotDiff > D3DX_PI)
			{
				m_fRotDiff += (-D3DX_PI * 2);
			}
			else if (m_fRotDiff < -D3DX_PI)
			{
				m_fRotDiff += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}

	float fRotDest = m_fRotDiff - m_fRotMove;	//目標までの移動方向の差分

	while (1)
	{
		if (fRotDest > D3DX_PI || fRotDest < -D3DX_PI)
		{//-3.14～3.14の範囲外の場合
			if (fRotDest > D3DX_PI)
			{
				fRotDest += (-D3DX_PI * 2);
			}
			else if (fRotDest < -D3DX_PI)
			{
				fRotDest += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}

	m_Info.rot.y += fRotDest * m_fRotMulti;

	while (1)
	{
		if (m_Info.rot.y > D3DX_PI || m_Info.rot.y < -D3DX_PI)
		{//-3.14～3.14の範囲外の場合
			if (m_Info.rot.y > D3DX_PI)
			{
				m_Info.rot.y += (-D3DX_PI * 2);
			}
			else if (m_Info.rot.y < -D3DX_PI)
			{
				m_Info.rot.y += (D3DX_PI * 2);
			}
		}
		else
		{
			break;
		}
	}
}

//===============================================
// 移動量加算
//===============================================
void CEnemy::AddMove(void)
{
	m_Info.move.x += (0.0f - m_Info.move.x) * m_fMoveIner;	//x座標
	m_Info.move.z += (0.0f - m_Info.move.z) * m_fMoveIner;	//x座標

	m_Info.pos.x += m_Info.move.x * CManager::GetInstance()->GetSlow()->Get();
	m_Info.pos.z += m_Info.move.z * CManager::GetInstance()->GetSlow()->Get();
}
