//==========================================================
//
// まきびし [spike.cpp]
// Author : Soma Ishihara
//
//==========================================================
#include "spike.h"
#include "manager.h"
#include "objectX.h"
#include "slow.h"
#include "meshfield.h"
#include "player.h"

// マクロ定義
#define SPIKE_GRAVITY	(-0.9f)		//敵重力
#define INER	(0.12f)		// 慣性
#define SPIKE_LIFE		(180)	//体力（毎フレ1ダメージ）

//静的メンバ変数
CSpike *CSpike::m_pTop = NULL;	// 先頭のオブジェクトへのポインタ
CSpike *CSpike::m_pCur = NULL;	// 最後尾のオブジェクトへのポインタ
int CSpike::m_nNumCount = 0;

//==========================================================
// コンストラクタ
//==========================================================
CSpike::CSpike()
{
	m_info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_info.posOld = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_info.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_info.nLife = 0;

	// 自分自身をリストに追加
	if (m_pTop != NULL)
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

	//個数増やす
	m_nNumCount++;
}

//==========================================================
// デストラクタ
//==========================================================
CSpike::~CSpike()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CSpike::Init(void)
{
	m_pObj = CObjectX::Create(m_info.pos, m_info.rot, "data\\MODEL\\spike.x");
	m_pObj->SetEnableCollision(false);
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CSpike::Uninit(void)
{
	// リストから自分自身を削除する
	if (m_pTop == this)
	{// 自身が先頭
		if (m_pNext != NULL)
		{// 次が存在している
			m_pTop = m_pNext;	// 次を先頭にする
			m_pNext->m_pPrev = NULL;	// 次の前のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = NULL;	// 先頭がない状態にする
			m_pCur = NULL;	// 最後尾がない状態にする
		}
	}
	else if (m_pCur == this)
	{// 自身が最後尾
		if (m_pPrev != NULL)
		{// 次が存在している
			m_pCur = m_pPrev;			// 前を最後尾にする
			m_pPrev->m_pNext = NULL;	// 前の次のポインタを覚えていないようにする
		}
		else
		{// 存在していない
			m_pTop = NULL;	// 先頭がない状態にする
			m_pCur = NULL;	// 最後尾がない状態にする
		}
	}
	else
	{
		if (m_pNext != NULL)
		{
			m_pNext->m_pPrev = m_pPrev;	// 自身の次に前のポインタを覚えさせる
		}
		if (m_pPrev != NULL)
		{
			m_pPrev->m_pNext = m_pNext;	// 自身の前に次のポインタを覚えさせる
		}
	}

	if (nullptr != m_pObj) {
		m_pObj->Uninit();
		m_pObj = NULL;
	}

	//個数減らす
	m_nNumCount--;

	//破棄
	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CSpike::Update(void)
{
	D3DXVECTOR3 pos = m_info.pos;
	m_info.posOld = m_info.pos;

	//移動
	float fGravity = SPIKE_GRAVITY * CManager::GetInstance()->GetSlow()->Get();
	m_info.move.y += fGravity;
	pos.y += m_info.move.y * CManager::GetInstance()->GetSlow()->Get();

	m_info.move.x += (0.0f - m_info.move.x) * INER;	//x座標
	m_info.move.z += (0.0f - m_info.move.z) * INER;	//x座標

	pos.x += m_info.move.x * CManager::GetInstance()->GetSlow()->Get();
	pos.z += m_info.move.z * CManager::GetInstance()->GetSlow()->Get();

	m_info.pos = pos;

	// 起伏との当たり判定
	float fHeight = CMeshField::GetHeight(m_info.pos);
	if (m_info.pos.y <= fHeight)
	{
		m_info.pos.y = fHeight;
		this->Collision();
	}

	//当たり判定
	D3DXVECTOR3 vtxMax = D3DXVECTOR3(20.0f, 0.0f, 20.0f);
	D3DXVECTOR3 vtxMin = D3DXVECTOR3(-20.0f, 0.0f, -20.0f);

	CObjectX::Collision(m_info.pos, m_info.posOld, m_info.move, vtxMin, vtxMax, 0.3f);
	this->CollisionCloss();

	//体力設定
	m_info.nLife--;

	if (m_info.nLife <= 0)
	{//お亡くなり
		Uninit();
	}

	// 使用オブジェクト更新
	if (nullptr != m_pObj) {
		m_pObj->SetPosition(m_info.pos);
		m_pObj->SetRotation(m_info.rot);
		m_pObj->Update();
	}
}

//==========================================================
// 生成
//==========================================================
CSpike *CSpike::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float moveHorizontal, const float moveVertical)
{
	CSpike *pSpike = nullptr;

	pSpike = new CSpike;

	if (pSpike != nullptr)
	{
		// 初期化処理
		pSpike->Init();

		//位置設定
		pSpike->SetPosition(pos);
		pSpike->SetRotation(rot);

		//移動量の設定
		D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		move.x = sinf(rot.y) * moveHorizontal + (rand() % 11 - 5);
		move.y = moveVertical + (rand() % 11 - 5);
		move.z = -cosf(rot.y) * moveHorizontal + (rand() % 11 - 5);
		pSpike->SetMove(move);

		//体力設定
		pSpike->SetLife(SPIKE_LIFE);
	}

	return pSpike;
}

//==========================================================
// 当たり判定(通過中（外積）)
//==========================================================
void CSpike::CollisionCloss(void)
{
	CPlayer* pPlayer = CPlayer::GetTop();

	while (pPlayer != nullptr)
	{
		CPlayer* pPlayerNext = pPlayer->GetNext();
		D3DXVECTOR3 vtxPlayerMax = D3DXVECTOR3(50.0f, 50.0f, 50.0f);
		D3DXVECTOR3 vtxPlayerMin = D3DXVECTOR3(-50.0f, -50.0f, -50.0f);
		D3DXVECTOR3 pos = pPlayer->GetPosition();

		D3DXVECTOR3 posPoint[4] =
		{
			D3DXVECTOR3(pos.x + vtxPlayerMin.x,0.0f,pos.z + vtxPlayerMin.z),
			D3DXVECTOR3(pos.x + vtxPlayerMax.x,0.0f,pos.z + vtxPlayerMin.z),
			D3DXVECTOR3(pos.x + vtxPlayerMax.x,0.0f,pos.z + vtxPlayerMax.z),
			D3DXVECTOR3(pos.x + vtxPlayerMin.x,0.0f,pos.z + vtxPlayerMax.z)
		};

		D3DXVECTOR3 vecMove, vecLine;
		D3DXVECTOR3 vecToPos, vecToPosOld;
		float fAreaA = 1.0f, fAreaB = 1.1f;

		for (int cnt = 0; cnt < 4; cnt++)
		{
			vecMove = m_info.pos - m_info.posOld;
			vecLine = posPoint[(cnt + 1) % 4] - posPoint[cnt];	//境界線ベクトル
			vecToPos = m_info.pos - posPoint[cnt];
			vecToPos.y = 0.0f;
			vecToPosOld = m_info.posOld - posPoint[cnt];
			vecToPosOld.y = 0.0f;

			//面積求める
			fAreaA = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
			fAreaB = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);

			if ((vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z) >= 0.0f && (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < 0.0f)
			{
				if (fAreaA / fAreaB >= 0.0f && fAreaA / fAreaB <= 1.0f)
				{//ごっつん
					pPlayer->Damage(1);
					Uninit();
					return;	//消滅したため終了
				}
			}
		}

		pPlayer = pPlayerNext;
	}
}

//==========================================================
// 当たり判定(落ちてる)
//==========================================================
void CSpike::Collision(void)
{
	CPlayer* pPlayer = CPlayer::GetTop();

	while (pPlayer != nullptr)
	{
		CPlayer* pPlayerNext = pPlayer->GetNext();

		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
		D3DXVECTOR3 vtxPlayerMax = D3DXVECTOR3(50.0f, 50.0f, 50.0f);
		D3DXVECTOR3 vtxPlayerMin = D3DXVECTOR3(-50.0f, -50.0f, -50.0f);
		D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(20.0f, 20.0f, 20.0f);
		D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(-20.0f, -20.0f, -20.0f);

		if (m_info.pos.x + vtxObjMax.x > posPlayer.x + vtxPlayerMin.x
			&& m_info.pos.x + vtxObjMin.x < posPlayer.x + vtxPlayerMax.x
			&& m_info.pos.y + vtxObjMax.y > posPlayer.y + vtxPlayerMin.y
			&& m_info.pos.y + vtxObjMin.y < posPlayer.y + vtxPlayerMax.y
			&& m_info.pos.z + vtxObjMax.z > posPlayer.z + vtxPlayerMin.z
			&& m_info.pos.z + vtxObjMin.z < posPlayer.z + vtxPlayerMax.z)
		{//範囲内にある
			pPlayer->Damage(1);
			Uninit();
		}

		pPlayer = pPlayerNext;
	}
}
