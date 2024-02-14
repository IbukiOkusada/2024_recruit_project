//==========================================================
//
// 体力UI [life.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "life.h"
#include "object2D.h"
#include "object_manager.h"
#include "texture.h"
#include "manager.h"
#include "slow.h"

// 名前空間
namespace {
	const char* TEXTURENAME = "data\\TEXTURE\\life.png";	// テクスチャファイル名
	const float MOVE = (-8.0f);
	const float GRAVITY = (0.55f);
	const float TIMER = (120.0f);
	const D3DXCOLOR DAMAGECOL = { 1.0f, 0.0f, 0.0f, 1.0f };
}

//==========================================================
// コンストラクタ
//==========================================================
CLife::CLife()
{
	// 値のクリア
	m_pObj = nullptr;
	m_bEnd = false;
	m_fMoveY = 0.0f;
	m_fTimer = 0.0f;
}

//==========================================================
// デストラクタ
//==========================================================
CLife::~CLife()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CLife::Init(void)
{
	m_pObj = CObject2D::Create(NUM_PRIORITY);
	m_pObj->BindTexture(CManager::GetInstance()->GetTexture()->Regist(TEXTURENAME));
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CLife::Uninit(void)
{
	if (m_pObj != nullptr) {
		m_pObj->Uninit();
		m_pObj = nullptr;
	}

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CLife::Update(void)
{
	if (!m_bEnd) {
		return;
	}

	// 消滅までのカウントを進める
	float fSlow = CManager::GetInstance()->GetSlow()->Get();
	m_fTimer -= fSlow;

	// 2Dポリゴンを移動
	m_fMoveY += GRAVITY * fSlow;
	if (m_pObj != nullptr) {	// 使用されている
		// 座標の移動
		D3DXVECTOR3 pos = m_pObj->GetPosition();
		pos.y += m_fMoveY * fSlow;
		m_pObj->SetPosition(pos);
		m_pObj->SetSize(m_pObj->GetWidth(), m_pObj->GetHeight());

		// 色の調整
		D3DXCOLOR col = m_pObj->GetCol();
		col.a = m_fTimer / TIMER;
		m_pObj->SetCol(col);
	}

	if (m_fTimer <= 0.0f) {	// タイマーが終了
		Uninit();
	}
}

//==========================================================
// 生成
//==========================================================
CLife *CLife::Create(const D3DXVECTOR3& pos, const float fWidth, const float fHeight)
{
	CLife *pLife = nullptr;

	pLife = new CLife;

	if (pLife != nullptr)
	{
		// 初期化処理
		pLife->Init();

		pLife->m_pObj->SetPosition(pos);
		pLife->m_pObj->SetSize(fWidth, fHeight);
	}

	return pLife;
}

//==========================================================
// 消滅
//==========================================================
void CLife::Lost(void)
{
	// 終了していないときのみ
	if (m_bEnd) {
		return;
	}

	m_bEnd = true;	// フラグ立てる
	m_fMoveY = MOVE;
	m_fTimer = TIMER;

	if (m_pObj != nullptr) {	// 使用されている
		m_pObj->SetCol(DAMAGECOL);
	}
}