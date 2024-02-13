//==========================================================
//
// アイテムボックス [itembox.cpp]
// Author : Soma Ishihara
//
//==========================================================
#include "itembox.h"
#include "spike.h"
#include "item.h"
#include "player.h"
#include "objectX.h"
#include "particle.h"
#include "Xfile.h"
#include "manager.h"

//静的メンバ変数
CItemBox *CItemBox::m_pTop = nullptr;
CItemBox *CItemBox::m_pCur = nullptr;

// マクロ定義
#define COLLISION_RANGE	(70.0f)
#define EMISSION_CT		(90)
#define PARTICLE_CT		(45)

//==========================================================
// コンストラクタ
//==========================================================
CItemBox::CItemBox()
{
	// 値のクリア
	m_pObj = nullptr;
	m_nCounter = 0;
	m_nParticleCounter = 0;

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
}

//==========================================================
// デストラクタ
//==========================================================
CItemBox::~CItemBox()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CItemBox::Init(void)
{
	m_pObj = CObjectX::Create(GetPosition(), GetRotation(), "data\\MODEL\\pot.x");
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CItemBox::Uninit(void)
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

	if (m_pObj != nullptr) {
		m_pObj->Uninit();
		m_pObj = nullptr;
	}

	ListOut();

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CItemBox::Update(void)
{
	m_nCounter--;
	if (m_nCounter < 0)
	{
		m_nCounter = 0;
	}

	m_nParticleCounter--;

	if (m_nParticleCounter <= 0) {
		m_nParticleCounter = PARTICLE_CT;
		D3DXVECTOR3 ObjPos = GetPosition();
		CXFile* pFile = CManager::GetInstance()->GetModelFile();
		CParticle::Create(D3DXVECTOR3(ObjPos.x, ObjPos.y + pFile->GetMax(m_pObj->GetIdx()).y, ObjPos.z), CEffect::TYPE_ITEMBOXSTAR);
	}
}

//==========================================================
// 生成
//==========================================================
CItemBox *CItemBox::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CItemBox *pItemBox = nullptr;

	pItemBox = new CItemBox;

	if (pItemBox != nullptr)
	{
		// 初期化処理
		pItemBox->SetPosition(pos);
		pItemBox->SetRotation(rot);
		pItemBox->Init();

		//データ設定
	}

	return pItemBox;
}

//==========================================================
// 排出
//==========================================================
void CItemBox::Emission(void)
{
	int nRand = rand() % 2;

	if (nRand == 0)
	{
		CItem *pItem = CItem::Create(GetPosition(), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_COIN, CItem::STATE_DROP);

		if (nullptr != pItem)
		{
			D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.04f;
			move.y = 18.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.04f;
			pItem->SetMove(move);
		}
	}
	else
	{
		CSpike::Create(GetPosition(), GetRotation(), 10.0f, 20.0f);
	}
}

//==========================================================
// 当たり判定
//==========================================================
bool CItemBox::CollisionCheck(D3DXVECTOR3 & pos, D3DXVECTOR3 & posOld, D3DXVECTOR3 & move, D3DXVECTOR3 & SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick ** ppGimmick, bool* bLand)
{
	D3DXVECTOR3 ObjPos = GetPosition();

	// 範囲内チェック
	float fLength = D3DXVec3Length(&(D3DXVECTOR3((pos.x - ObjPos.x), 0.0f, (pos.z - ObjPos.z))));

	if (fLength < COLLISION_RANGE && nAction == CPlayer::ACTION_ATK && m_nCounter <= 0)
	{// 範囲内
		Emission();
		CXFile* pFile = CManager::GetInstance()->GetModelFile();
		CParticle::Create(D3DXVECTOR3(ObjPos.x, ObjPos.y + pFile->GetMax(m_pObj->GetIdx()).y, ObjPos.z), CEffect::TYPE_ITEMBOX);
		m_nCounter = EMISSION_CT;
	}

	return false;
}
