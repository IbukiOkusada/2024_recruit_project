//==========================================================
//
// メッセージ表示の処理 [message.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "message.h"
#include "billboard.h"
#include "manager.h"
#include "slow.h"
#include "texture.h"

// 無名名前空間
namespace
{
	const float ALPHA_MINUS = (0.015f);
	const float MOVEY = (2.0f);
	const float SETLIFE = (120.0f);
	const float WIDTH = (120.0f);
	const float HEIGHT = (30.0f);
	const char* TEXTUREFILENAME = "data\\TEXTURE\\checkpoint000.png";
}

//==========================================================
// コンストラクタ
//==========================================================
CMessage::CMessage()
{
	// 値のクリア
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_pObj = nullptr;
	m_fLife = 0.0f;
}

//==========================================================
// デストラクタ
//==========================================================
CMessage::~CMessage()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CMessage::Init(void)
{
	m_pObj = CObjectBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5);
	m_pObj->SetSize(WIDTH, HEIGHT);
	m_pObj->SetCol(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	m_pObj->BindTexture(CManager::GetInstance()->GetTexture()->Regist(TEXTUREFILENAME));
	m_pObj->SetAlphaText(true);
	m_pObj->SetFusion(CObjectBillboard::FUSION_NORMAL);
	m_fLife = SETLIFE;
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CMessage::Uninit(void)
{
	// 描画オブジェクトの終了
	if (m_pObj != nullptr) {
		m_pObj->Uninit();
		m_pObj = nullptr;
	}

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CMessage::Update(void)
{
	if (m_pObj == nullptr) { // 使用されていない
		Uninit();
		return;
	}

	// 体力を減らす
	float fSlow = CManager::GetInstance()->GetSlow()->Get();
	m_fLife -= fSlow;

	if (m_fLife <= 0.0f) {	// 体力がなくなった
		Uninit();
		return;
	}

	// 色の変更
	{
		D3DXCOLOR col = m_pObj->GetCol();
		col.a -= ALPHA_MINUS * fSlow;
		m_pObj->SetCol(col);
	}

	// 移動
	{
		D3DXVECTOR3 pos = m_pObj->GetPosition();
		pos.y += MOVEY * fSlow;
		m_pObj->SetPosition(pos);
	}
}

//==========================================================
// 生成
//==========================================================
CMessage *CMessage::Create(const D3DXVECTOR3& pos)
{
	CMessage *pMessage = nullptr;

	pMessage = new CMessage;

	if (pMessage != nullptr)
	{
		// 初期化処理
		pMessage->Init();

		pMessage->SetPosition(pos);
	}

	return pMessage;
}

//==========================================================
// 座標設定
//==========================================================
void CMessage::SetPosition(const D3DXVECTOR3& pos)
{
	m_pos = pos;

	// 使用されていない
	if(m_pObj == nullptr){ return; }

	m_pObj->SetPosition(pos);
}