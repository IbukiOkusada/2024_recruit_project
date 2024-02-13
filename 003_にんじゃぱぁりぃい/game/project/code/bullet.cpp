//==========================================================
//
// 弾 [bullet.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "bullet.h"
#include "model.h"
#include "enemy.h"
#include "player.h"
#include "objectX.h"
#include "particle.h"
#include "gimmick.h"

// 無名名前空間
namespace {
	const char* FILENAME = "data\\MODEL\\bullet.x";	// ファイル名
	const int SETLIFE = (240);	// 弾の設定寿命
	const float COLLRANGE = (25.0f);
}

//==========================================================
// コンストラクタ
//==========================================================
CBullet::CBullet()
{
	// 値のクリア
	m_pObject = nullptr;
	m_bMove = true;
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_nLife = 0;
	m_nId = -1;
}

//==========================================================
// デストラクタ
//==========================================================
CBullet::~CBullet()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CBullet::Init(void)
{
	// オブジェクトの生成
	m_pObject = CModel::Create(FILENAME);
	m_pObject->SetParent(&m_Info.mtxWorld);

	// 寿命の設定
	m_nLife = SETLIFE;

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CBullet::Uninit(void)
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
void CBullet::Update(void)
{
	m_Info.posOld = m_Info.pos;

	// 操作関連
	Controller();

	// マトリックス反映
	SetMatrix();

	// 寿命確認
	if (m_nLife <= 0) {	// 寿命がなくなった
		Uninit();	// 終了
	}
	else
	{
		m_nLife--;

		// 当たり判定
		Hit();

		// 移動している
		if (m_bMove) {
			CParticle::Create(m_Info.pos, CEffect::TYPE_KUNAI);
		}
	}
}

//==========================================================
// 生成
//==========================================================
CBullet *CBullet::Create(const D3DXVECTOR3 &pos, const D3DXVECTOR3 &rot, const D3DXVECTOR3 &move)
{
	CBullet *pBullet = nullptr;

	pBullet = new CBullet;

	if (pBullet != nullptr)
	{
		// 初期化処理
		pBullet->Init();

		// 座標反映
		pBullet->SetPosition(pos);

		// 向き反映
		pBullet->SetRotation(rot);

		// 移動量反映
		pBullet->SetMove(move);
	}

	return pBullet;
}

//==========================================================
// マトリックス設定
//==========================================================
void CBullet::SetMatrix(void)
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
void CBullet::Controller(void)
{
	// 移動
	m_Info.pos += m_Info.move;
}

//==========================================================
// ヒット処理
//==========================================================
void CBullet::Hit(void)
{
	bool bHit = false;

	// 敵との判定
	{
		CEnemy *pEnem = CEnemy::GetTop();
		while (pEnem != nullptr) {
			CEnemy *pEnemNext = pEnem->GetNext();

			if (pEnem->HitCheck(m_Info.pos, COLLRANGE)) {	// 当たっている
				bHit = true;
			}

			pEnem = pEnemNext;
		}
	}

	// プレイヤーとの判定
	{
		CPlayer *pPlay = CPlayer::GetTop();

		while (pPlay != nullptr) {
			CPlayer *pPlayNext = pPlay->GetNext();

			if (m_nId != pPlay->GetId()) {	// 自分のではない
				if (pPlay->HitCheck(m_Info.pos, COLLRANGE)) {	// 当たっている
					bHit = true;
				}
			}

			pPlay = pPlayNext;
		}
	}

	// オブジェクトとの判定
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(COLLRANGE * 0.5f, COLLRANGE * 0.15f, COLLRANGE * 0.5f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-COLLRANGE * 0.5f, -COLLRANGE * 0.15f, -COLLRANGE * 0.5f);
	if (m_bMove) {
		D3DXVECTOR3 moveOld = m_Info.move;
		D3DXVECTOR3 posOld = m_Info.pos;
		CObjectX::Touch(m_Info.pos, m_Info.posOld, m_Info.move, vtxMin, vtxMax);

		if (m_Info.pos.x != posOld.x || m_Info.pos.z != posOld.z) {	// 当たって移動量がなくなった
			m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			m_Info.pos = posOld;
			m_bMove = false;
		}
	}

	D3DXVECTOR3 moveTemp = m_Info.move;
	CGimmick::Collision(m_Info.pos, m_Info.posOld, m_Info.move, D3DXVECTOR3(0.0f, 0.0f, 0.0f), vtxMin, vtxMax, 0);
	if (moveTemp != m_Info.move)
	{//クナイは当たったら停止
		m_Info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_bMove = false;
	}

	if (bHit) {	// 当たった
		Uninit();
	}
}