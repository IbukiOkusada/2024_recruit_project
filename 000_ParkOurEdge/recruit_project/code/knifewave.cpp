//==========================================================
//
// 弾 [bullet.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "knifewave.h"
#include "enemy.h"
#include "player.h"
#include "objectX.h"
#include "particle.h"
#include "enemy_manager.h"
#include "player_manager.h"
#include "manager.h"
#include "slow.h"
#include "meshwall.h"

// 無名名前空間
namespace {
	const char* FILENAME = "data\\MODEL\\bullet.x";	// ファイル名
	const int SETLIFE = (240);	// 弾の設定寿命
	const int SETBOSSLIFE = (300);
	const float COLLRANGE = (25.0f);
}

//==========================================================
// コンストラクタ
//==========================================================
CKnifeWave::CKnifeWave()
{
	// 値のクリア
	m_pObject = nullptr;
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fLife = 0;
	m_nId = -1;
}

//==========================================================
// デストラクタ
//==========================================================
CKnifeWave::~CKnifeWave()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CKnifeWave::Init(void)
{
	// オブジェクトの生成
	m_pObject = CObjectX::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), "data\\MODEL\\knifewave.x");
	//m_pObject->SetCurrent(&m_Info.mtxWorld);

	// 寿命の設定
	m_fLife = SETLIFE;

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CKnifeWave::Uninit(void)
{
	// オブジェクトの終了
	if (m_pObject != nullptr) {
		m_pObject->Uninit();
		m_pObject = nullptr;
	}

	// 開放
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CKnifeWave::Update(void)
{
	m_Info.posOld = m_Info.pos;

	// 操作関連
	Controller();

	// マトリックス反映
	SetMatrix();

	m_pObject->SetPosition(m_Info.pos);
	m_pObject->SetRotation(m_Info.rot);

	// 寿命確認
	if (m_fLife <= 0) {	// 寿命がなくなった
		Uninit();	// 終了
	}
	else
	{
		m_fLife-= CManager::GetInstance()->GetSlow()->Get();

		// 当たり判定
		Hit();
	}
}

//==========================================================
// 生成
//==========================================================
CKnifeWave *CKnifeWave::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &move, const TYPE type)
{
	CKnifeWave *pKnifeWave = nullptr;

	pKnifeWave = new CKnifeWave;

	if (pKnifeWave != nullptr)
	{
		// 初期化処理
		pKnifeWave->Init();

		// 座標反映
		pKnifeWave->SetPosition(pos);

		// 向き反映
		pKnifeWave->SetRotation(rot);

		// 移動量反映
		pKnifeWave->SetMove(move);

		// 種類反映
		pKnifeWave->SetType(type);
	}

	return pKnifeWave;
}

//==========================================================
// マトリックス設定
//==========================================================
void CKnifeWave::SetMatrix(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_Info.mtxWorld);

	// 向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Info.rot.y, m_Info.rot.x, m_Info.rot.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxRot);

	// 位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_Info.pos.x, m_Info.pos.y, m_Info.pos.z);
	D3DXMatrixMultiply(&m_Info.mtxWorld, &m_Info.mtxWorld, &mtxTrans);
}

//==========================================================
// 操作関連
//==========================================================
void CKnifeWave::Controller(void)
{
	// 移動
	m_Info.pos += m_Info.move * CManager::GetInstance()->GetSlow()->Get();
}

//==========================================================
// ヒット処理
//==========================================================
void CKnifeWave::Hit(void)
{
	bool bHit = false;
	
	// プレイヤーとの判定
	if (m_type == TYPE_ENEMY) {
		CPlayer* pPlay = CPlayerManager::GetInstance()->GetTop();

		while (pPlay != nullptr) {
			CPlayer* pPlayNext = pPlay->GetNext();

			if (m_nId != pPlay->GetId()) {	// 自分のではない
				if (pPlay->HitCheck(m_Info.pos, COLLRANGE, 200.0f, 1)) {	// 当たっている
					bHit = true;
				}
			}

			pPlay = pPlayNext;
		}
	}

	// オブジェクトとの判定
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(COLLRANGE * 0.5f, COLLRANGE * 0.15f, COLLRANGE * 0.5f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-COLLRANGE * 0.5f, -COLLRANGE * 0.15f, -COLLRANGE * 0.5f);

	D3DXVECTOR3 moveOld = m_Info.move;
	D3DXVECTOR3 posOld = m_Info.pos;
	CObjectX::COLLISION_AXIS axis = CObjectX::TYPE_X;
	CObjectX::Touch(m_Info.pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax);
	CMeshWall::Collision(m_Info.pos, m_Info.posOld, m_Info.move, vtxMax, vtxMin, axis);

	if (m_Info.pos.x != posOld.x || m_Info.pos.z != posOld.z) {	// 当たって移動量がなくなった
		bHit = true;
	}

	D3DXVECTOR3 moveTemp = m_Info.move;

	if (bHit) {	// 当たった
		Uninit();
	}
}