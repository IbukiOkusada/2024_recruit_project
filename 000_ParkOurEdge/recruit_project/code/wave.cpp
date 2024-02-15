//==========================================================
//
// 風の術 [wave.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "wave.h"
#include "player.h"
#include "enemy.h"
#include "manager.h"
#include "debugproc.h"
#include "meshcylinder.h"
#include "texture.h"
#include "particle.h"
#include "player_manager.h"

// 無名名前空間
namespace {
	const float RANGE_UPSPEED = (15.0f);	// 1フレーム間の範囲増加量
	const float FLYAWAY_SPEED = (100.0f);	// 吹っ飛ぶ速度(かっけえ変数名)
	const float ITEMAWAY_SPEED = (18.0f);	// アイテムの吹っ飛ぶ速度
	const float ITEMAWAY_JUMP = (15.0f);
	const float AIROBJ_HEIGHT = (40.0f);	// オブジェクトの高さ
	const int OBJ_NUMWIDTH = (10);		// 横枚数
	const float FLYAWAY_JUMP = (20.0f);	// 吹っ飛ぶ高さ
	const int PARTICLE_CT = (1);		// パーティクルのカウンター
	const char* FILENAME[CWave::TYPE_MAX] = {	// テクスチャファイル名
		"data\\TEXTURE\\wind000.png",
		"data\\TEXTURE\\wind001.png"
	};
}

//==========================================================
// コンストラクタ
//==========================================================
CWave::CWave()
{
	// 値のクリア
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		m_apObject[nCnt] = nullptr;
	}
	m_Info.fRange = 0.0f;
	m_fLength = 0.0f;
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_Info.mtxWorld);
}

//==========================================================
// デストラクタ
//==========================================================
CWave::~CWave()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CWave::Init(void)
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
void CWave::Uninit(void)
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
void CWave::Update(void)
{
	float fRate = m_Info.fRange / m_fLength;
	m_Info.fRangeOld = m_Info.fRange;
	m_Info.fRange += RANGE_UPSPEED;

	if (m_Info.fRange > m_fLength) {	// 最大範囲を超えた
		Uninit();
	}
	else
	{
		m_nParticleCounter--;

		// パーティクルの生成
		if (m_nParticleCounter <= 0) {
			float fRange = (rand() % 629 - 314) * 0.01f;
			D3DXVECTOR3 pos = D3DXVECTOR3(m_Info.pos.x + sinf(fRange) * m_Info.fRange, m_Info.pos.y, m_Info.pos.z + cosf(fRange) * m_Info.fRange);
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
			float fAdd = 0.1f;
			float fMulti = 0.15f;

			if (nCnt == TYPE_NORMAL) {
				fAdd *= -1.0f;
				fMulti *= -1.0f;
			}

			D3DXVECTOR3 rot = m_apObject[nCnt]->GetRotation();
			rot.y += fAdd + (fRate * fMulti);
			m_apObject[nCnt]->SetRotation(rot);
			m_apObject[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (fRate)));
		}
	}
}

//==========================================================
// 生成
//==========================================================
CWave *CWave::Create(const D3DXVECTOR3& pos, const int nId, const float fLength)
{
	CWave *pWave = nullptr;

	pWave = new CWave;

	if (pWave != nullptr)
	{
		// 初期化処理
		pWave->Init();

		// 座標の設定
		pWave->SetPosition(pos);

		// IDの設定
		pWave->BindId(nId);

		pWave->m_fLength = fLength;
	}

	return pWave;
}

//==========================================================
// 当たり判定
//==========================================================
void CWave::Collision(void)
{
	// プレイヤーとの判定
	{
		CPlayer *pPlayer = CPlayerManager::GetInstance()->GetTop();

		while (pPlayer != nullptr) 
		{
			CPlayer *pPlayerNext = pPlayer->GetNext();	// 次を保持

			// 距離を取る
			D3DXVECTOR3 ObjPos = pPlayer->GetPosition();
			D3DXVECTOR3 ObjPosOld = pPlayer->GetOldPosition();
			float fLength = sqrtf((m_Info.pos.x - ObjPos.x) * (m_Info.pos.x - ObjPos.x)
				+ (m_Info.pos.z - ObjPos.z) * (m_Info.pos.z - ObjPos.z));
			float fLengthOld = sqrtf((m_Info.pos.x - ObjPosOld.x) * (m_Info.pos.x - ObjPosOld.x)
				+ (m_Info.pos.z - ObjPosOld.z) * (m_Info.pos.z - ObjPosOld.z));

			if (fLength > m_Info.fRange) {	// 風の範囲内ではない場合
				pPlayer = pPlayerNext;
				continue;
			}

			// 高さが当たらない場合
			if (ObjPos.y + PLAYER::COLLIMIN.y > m_Info.pos.y + AIROBJ_HEIGHT * 2
				|| ObjPos.y + PLAYER::COLLIMAX.y < m_Info.pos.y) {
				pPlayer = pPlayerNext;
				continue;
			}

			if (fLengthOld >= m_Info.fRangeOld && fLength <= m_Info.fRange) {	// 風を跨いだ
				pPlayer->Damage(1);
			}

			pPlayer = pPlayerNext;	// 次に移動
		}
	}
}