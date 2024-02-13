//==========================================================
//
// ノルマのUI [quataui.cpp]
// Author : Ryosuke Ohara
//
//==========================================================
#include "quataui.h"
#include "object2D.h"
#include "manager.h"
#include "texture.h"
#include "life.h"
#include "slow.h"

// 無名名前空間
namespace {
	const int DEF_PRI = (7);                        // 優先順位
	const D3DXVECTOR2 MOVESPEED = { 8.0f, 2.5f };	// 動く速さ
	const float STATEUP_COUNTER = (120.0f);
	const char* FILENAME[CQuataUI::TYPE_MAX] = {	// テクスチャファイル名
		"data\\TEXTURE\\quataui000.png",
		"data\\TEXTURE\\quataui001.png",
	};
}

//==========================================================
// コンストラクタ
//==========================================================
CQuataUI::CQuataUI(int nPriority)
{
	// 値のクリア
	m_Info.m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_Info.m_state = STATE_NONE;
	m_Info.m_type = TYPE_START;
	m_Info.m_fHeight = 0.0f;
	m_Info.m_fWidht = 0.0f;
	m_Info.fStateCounter = 0.0f;
	m_nNumPlayer = 0;
}

//==========================================================
// デストラクタ
//==========================================================
CQuataUI::~CQuataUI()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CQuataUI::Init(void)
{
	if (m_pObject == nullptr)
	{// 使用されていない場合

	}

	return S_OK;
}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CQuataUI::Init(float fWidth, float fHeight)
{
	if (m_pObject == nullptr)
	{// 使用されていない場合

		m_pObject = CObject2D::Create(6);
		m_pObject->BindTexture(CManager::GetInstance()->GetTexture()->Regist(FILENAME[m_Info.m_type]));
		m_pObject->SetPosition(m_Info.m_pos);
		m_pObject->SetSize(m_Info.m_fWidht, m_Info.m_fHeight);
	}

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CQuataUI::Uninit(void)
{
	if (m_pObject != nullptr)
	{
		m_pObject->Uninit();
		m_pObject = nullptr;
	}

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CQuataUI::Update(void)
{
	switch (m_Info.m_state)
	{
	case STATE_NONE:
		break;

	case STATE_MOVE:

		m_Info.m_pos.x -= MOVESPEED.x;

		m_pObject->SetPosition(m_Info.m_pos);

		if (m_Info.m_pos.x <= SCREEN_WIDTH * 0.5f)
		{
			m_Info.m_state = STATE_SCALING;
		}

		break;

	case STATE_SCALING:

		float fDestWidth, fDestHeight;

		fDestWidth = 100.0f - (m_Info.m_fWidht * 0.5f);

		fDestHeight = 50.0f - (m_Info.m_fHeight * 0.5f);

		m_Info.m_fWidht += fDestWidth;

		m_Info.m_fHeight += fDestHeight;

		if (m_Info.m_fHeight >= 100.0f && m_Info.m_fWidht >= 200.0f)
		{
			m_Info.m_state = STATE_UP;
			m_Info.fStateCounter = STATEUP_COUNTER;
		}
		break;

	case STATE_UP:
	{

		m_Info.fStateCounter -= CManager::GetInstance()->GetSlow()->Get();

		if (m_Info.fStateCounter <= 0.0f)
		{
			m_Info.m_pos.y -= MOVESPEED.y;
			m_Info.m_fWidht -= 0.45f;
			m_Info.m_fHeight -= 0.35f;
			m_pObject->SetPosition(m_Info.m_pos);
			if (m_Info.m_pos.y <= SCREEN_HEIGHT * 0.2f)
			{
				m_Info.m_state = STATE_CLEAR;

			}
		}
	}
		break;

	case STATE_CLEAR:

		m_Info.m_col.a -= 0.05f;

		m_pObject->SetCol(m_Info.m_col);

		if (m_Info.m_col.a <= 0.5f)
		{
			m_Info.m_state = STATE_CHANGE;
		}

		break;

	case STATE_CHANGE:

		m_pObject->BindTexture(CManager::GetInstance()->GetTexture()->Regist(FILENAME[TYPE_FRAME]));

		m_Info.m_state = STATE_SET;
		break;

	case STATE_SET:

		m_Info.m_pos.y -= MOVESPEED.y * 0.13f;
		m_Info.m_fWidht -= 0.35f;
		m_Info.m_fHeight -= 0.35f;
		m_pObject->SetPosition(m_Info.m_pos);


		if (m_Info.m_fHeight <= 30.0f)
		{
			m_Info.m_state = STATE_NONE;
		}
		break;

	case STATE_MAX:
		break;

	default:
		break;
	}

	// サイズ設定
	m_pObject->SetSize(m_Info.m_fWidht, m_Info.m_fHeight);
}

//==========================================================
// 生成
//==========================================================
CQuataUI *CQuataUI::Create(D3DXVECTOR3 pos, QUATATYPE type, STATE state, float fWidth, float fHeight)
{
	CQuataUI *pUI = nullptr;

	pUI = new CQuataUI;

	if (pUI != nullptr)
	{
		pUI->SetPosition(pos);         // 位置設定
		pUI->SetWight(fWidth);         // 横幅設定
		pUI->SetHeight(fHeight);       // 高さ設定
		pUI->SetType(type);            // 種類設定
		pUI->SetState(state);          // 状態設定
		pUI->Init(fWidth, fHeight);    // 初期化処理
	}

	return pUI;
}