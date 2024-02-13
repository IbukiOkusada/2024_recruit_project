//==========================================================
//
// 風の術 [air.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "air.h"
#include "player.h"
#include "enemy.h"
#include "manager.h"
#include "debugproc.h"
#include "meshcylinder.h"
#include "texture.h"
#include "item.h"
#include "particle.h"

// 無名名前空間
namespace {
	const float MAX_LENGTH = (500.0f);	// 最大の範囲
	const float RANGE_UPSPEED = (5.0f);	// 1フレーム間の範囲増加量
	const float FLYAWAY_SPEED = (100.0f);	// 吹っ飛ぶ速度(かっけえ変数名)
	const float ITEMAWAY_SPEED = (18.0f);	// アイテムの吹っ飛ぶ速度
	const float ITEMAWAY_JUMP = (15.0f);
	const float AIROBJ_HEIGHT = (40.0f);	// オブジェクトの高さ
	const int OBJ_NUMWIDTH = (10);		// 横枚数
	const float FLYAWAY_JUMP = (20.0f);	// 吹っ飛ぶ高さ
	const int PARTICLE_CT = (1);		// パーティクルのカウンター
	const char* FILENAME[CAir::TYPE_MAX] = {	// テクスチャファイル名
		"data\\TEXTURE\\wind000.png",
		"data\\TEXTURE\\wind001.png"
	};
}

//==========================================================
// コンストラクタ
//==========================================================
CAir::CAir()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		m_apObject[nCnt] = nullptr;
	}
	m_Info.fRange = 0.0f;
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_Info.mtxWorld);
}

//==========================================================
// デストラクタ
//==========================================================
CAir::~CAir()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CAir::Init(void)
{
	// オブジェクトの生成
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		m_apObject[nCnt] = CMeshCylinder::Create(m_Info.pos, m_Info.rot, m_Info.fRange, AIROBJ_HEIGHT, 5, OBJ_NUMWIDTH);
		m_apObject[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->Regist(FILENAME[nCnt]));
	}

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CAir::Uninit(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		if (m_apObject[nCnt] != nullptr) {
			m_apObject[nCnt]->Uninit();
			m_apObject[nCnt] = nullptr;
		}
	}

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CAir::Update(void)
{
	m_Info.fRange += RANGE_UPSPEED;

	if (m_Info.fRange > MAX_LENGTH) {	// 最大範囲を超えた
		Uninit();
	}
	else
	{
		m_nParticleCounter--;

		// パーティクルの生成
		if (m_nParticleCounter <= 0) {
			float fRange = (rand() % 629 - 314) * 0.01f;
			D3DXVECTOR3 pos = D3DXVECTOR3(m_Info.pos.x + sinf(fRange) * m_Info.fRange, 0.0f, m_Info.pos.z + cosf(fRange) * m_Info.fRange);
			CParticle::Create(pos, CEffect::TYPE_AIR);
			m_nParticleCounter = PARTICLE_CT;
		}

		// 当たり判定を取る
		Collision();
	}

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		if (m_apObject[nCnt] != nullptr) {
			m_apObject[nCnt]->SetLength(m_Info.fRange);
			m_apObject[nCnt]->SetPosition(m_Info.pos);

			// 現在の割合を求める
			float fRate = m_Info.fRange / MAX_LENGTH;
			float fAdd = 0.1f;
			float fMulti = 0.15f;

			if (nCnt == TYPE_ANOTHER) {
				fAdd *= -1.0f;
				fMulti *= -1.0f;
			}

			D3DXVECTOR3 rot = m_apObject[nCnt]->GetRotation();
			rot.y += fAdd + (fRate * fMulti);
			m_apObject[nCnt]->SetRotation(rot);
			m_apObject[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (fRate * 0.75f)));
		}
	}
}

//==========================================================
// 生成
//==========================================================
CAir *CAir::Create(const D3DXVECTOR3& pos, const int nId)
{
	CAir *pAir = nullptr;

	pAir = new CAir;

	if (pAir != nullptr)
	{
		// 初期化処理
		pAir->Init();

		// 座標の設定
		pAir->SetPosition(pos);

		// IDの設定
		pAir->BindId(nId);
	}

	return pAir;
}

//==========================================================
// 当たり判定
//==========================================================
void CAir::Collision(void)
{
	// プレイヤーとの判定
	{
		CPlayer *pPlayer = CPlayer::GetTop();

		while (pPlayer != nullptr) 
		{
			CPlayer *pPlayerNext = pPlayer->GetNext();	// 次を保持

			if (pPlayer->GetId() == m_nId) {	// 出した本人
				pPlayer = pPlayerNext;
				continue;
			}

			// 距離を取る
			D3DXVECTOR3 ObjPos = pPlayer->GetPosition();
			float fLength = sqrtf((m_Info.pos.x - ObjPos.x) * (m_Info.pos.x - ObjPos.x)
				+ (m_Info.pos.z - ObjPos.z) * (m_Info.pos.z - ObjPos.z));

			if (fLength > m_Info.fRange) {	// 風の範囲内ではない場合
				pPlayer = pPlayerNext;
				continue;
			}

			D3DXVECTOR3 move = pPlayer->GetMove();
			float fRot = atan2f(ObjPos.x - m_Info.pos.x, ObjPos.z - m_Info.pos.z);	//目標までの移動差分

			// 移動方向を設定
			move.x = sinf(fRot) * FLYAWAY_SPEED;
			if (move.y <= 0.0f)
			{
				move.y = FLYAWAY_JUMP;
			}
			move.z = cosf(fRot) * FLYAWAY_SPEED;

			// 移動量を反映
			pPlayer->SetMove(move);
			pPlayer->Blow();

			pPlayer = pPlayerNext;	// 次に移動
		}
	}

	// 敵との判定
	{
		CEnemy *pEnemy = CEnemy::GetTop();

		while (pEnemy != nullptr)
		{
			CEnemy *pEnemyNext = pEnemy->GetNext();	// 次を保持

			// 距離を取る
			D3DXVECTOR3 ObjPos = pEnemy->GetPosition();
			float fLength = sqrtf((m_Info.pos.x - ObjPos.x) * (m_Info.pos.x - ObjPos.x)
				+ (m_Info.pos.z - ObjPos.z) * (m_Info.pos.z - ObjPos.z));

			if (fLength > m_Info.fRange) {	// 風の範囲内ではない場合
				pEnemy = pEnemyNext;
				continue;
			}

			D3DXVECTOR3 move = pEnemy->GetMove();
			float fRot = atan2f(ObjPos.x - m_Info.pos.x, ObjPos.z - m_Info.pos.z);	//目標までの移動差分

			// 移動方向を設定
			move.x = sinf(fRot) * FLYAWAY_SPEED;
			if (move.y <= 0.0f)
			{
				move.y = FLYAWAY_JUMP;
			}
			move.z = cosf(fRot) * FLYAWAY_SPEED;

			// 移動量を反映
			pEnemy->SetMove(move);
			pEnemy->Blow();

			pEnemy = pEnemyNext;	// 次に移動
		}
	}

	// アイテムとの判定
	{
		CItem *pItem = CItem::GetTop();

		while (pItem != nullptr)
		{
			CItem *pItemNext = pItem->GetNext();	// 次を保持

													// 距離を取る
			D3DXVECTOR3 ObjPos = pItem->GetPosition();
			float fLength = sqrtf((m_Info.pos.x - ObjPos.x) * (m_Info.pos.x - ObjPos.x)
				+ (m_Info.pos.z - ObjPos.z) * (m_Info.pos.z - ObjPos.z));

			if (fLength > m_Info.fRange || m_Info.pos.y + AIROBJ_HEIGHT <= ObjPos.y) {	// 風の範囲内ではない場合
				pItem = pItemNext;
				continue;
			}

			D3DXVECTOR3 move = pItem->GetMove();
			float fRot = atan2f(ObjPos.x - m_Info.pos.x, ObjPos.z - m_Info.pos.z);	//目標までの移動差分

			// 移動方向を設定
			move.x = sinf(fRot) * ITEMAWAY_SPEED;
			if (move.y <= 0.0f)
			{
				move.y = ITEMAWAY_JUMP;
			}
			move.z = cosf(fRot) * ITEMAWAY_SPEED;

			// 移動量を反映
			pItem->SetMove(move);
			pItem->SetState(CItem::STATE_DROP);

			pItem = pItemNext;	// 次に移動
		}
	}
}