//==========================================================
//
// ポーズの処理 [pause.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "pause.h"
#include "input.h"
#include "fade.h"
#include "texture.h"
#include "manager.h"
#include "camera.h"
#include "time.h"
#include "object_manager.h"
#include "object2D.h"

//マクロ定義
#define BUTTONWIDTH		(140.0f)		//ポーズ画面ボタン幅
#define BUTTONHEIGHT		(45.0f)			//ポーズ画面ボタン高さ
#define BUTTONX			(640.0f)		//ポーズ画面X座標
#define BUTTONY			(250.0f)		//ポーズ画面Y座標
#define BUTTONSPACE		(130.0f)		//ポーズ画面縦スペース
#define MOVECOLA		(-0.02f)			//毎フレーム変わるボタンの透明度
#define BGSIZE			(270.0f)		//サイズ
#define BGHEIGHT		(270.0f)			//縦幅

//===============================================
// テクスチャファイル名
//===============================================
const char* CPause::m_apTexPass[TYPE_MAX]
{
	"data\\TEXTURE\\pause000.png",
	"data\\TEXTURE\\pause001.png",
	"data\\TEXTURE\\pause002.png",
	"data\\TEXTURE\\pause_bg000.png",
};

//===============================================
// コンストラクタ
//===============================================
CPause::CPause()
{
	// 値のクリア
	m_SelectMenu = MENU_CONTINUE;
	m_bSelect = false;
	m_fMoveCol_a = 0.0f;
	m_pBg = NULL;

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		m_aMenu[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		m_aMenu[nCnt].pObject2D = NULL;
	}
	
	m_nStartDeltaTime = 0;
	m_nPauseTimer = 0;
}

//===============================================
// デストラクタ
//===============================================
CPause::~CPause()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CPause::Init(void)
{
	// 背景の生成
	m_pBg = CObject2D::Create(NUM_PRIORITY - 1);

	if (m_pBg != NULL)
	{
		m_pBg->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTexPass[TYPE_BG]));
		m_pBg->SetPosition(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.49f, 0.0f));
		m_pBg->SetSize(BGSIZE, BGHEIGHT);
		m_pBg->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pBg->SetDraw(false);
	}

	// 選択肢
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		m_aMenu[nCnt].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

		// 2Dオブジェクトの生成
		m_aMenu[nCnt].pObject2D = CObject2D::Create(NUM_PRIORITY - 1);

		if (m_aMenu[nCnt].pObject2D != NULL)
		{// 生成された
			// テクスチャの設定
			m_aMenu[nCnt].pObject2D->BindTexture(CManager::GetInstance()->GetTexture()->Regist(m_apTexPass[nCnt]));
			m_aMenu[nCnt].pObject2D->SetPosition(D3DXVECTOR3(BUTTONX, BUTTONY + nCnt * BUTTONSPACE, 0.0f));
			m_aMenu[nCnt].pObject2D->SetSize(BUTTONWIDTH, BUTTONHEIGHT);
			m_aMenu[nCnt].pObject2D->SetDraw(false);
		}
	}

	ColorSet();

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CPause::Uninit(void)
{
	if (m_pBg != nullptr) {
		m_pBg->Uninit();
		m_pBg = nullptr;
	}

	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		m_aMenu[nCnt].pObject2D->Uninit();
		m_aMenu[nCnt].pObject2D = NULL;
	}

	m_pBg = NULL;
}

//===============================================
// 更新処理
//===============================================
bool CPause::Update(void)
{
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	CFade *pFade = CManager::GetInstance()->GetFade();
	bool bValue = false;

	if (m_bSelect == true || CManager::GetInstance()->GetFade()->GetState() != CFade::STATE_NONE)
	{// 押された状態の場合
		return bValue;
	}

	if (pInputKey->GetTrigger(DIK_P)|| pInputPad->GetTrigger(CInputPad::BUTTON_START, 0) == true)
	{// 上移動キーが入力された

		CObject::TYPE type = CObject::TYPE_NONE;

		if (m_pBg != NULL)
		{
			m_pBg->SetType(type);
		}

		// 選択肢
		for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
		{
			if (m_aMenu[nCnt].pObject2D != NULL)
			{// 生成された
				m_aMenu[nCnt].pObject2D->SetType(type);
			}
		}
	}

	m_aMenu[m_SelectMenu].col.a += m_fMoveCol_a;

	if (m_aMenu[m_SelectMenu].col.a >= 1.0f || m_aMenu[m_SelectMenu].col.a <= 0.3f)
	{// 完全に不透明、または透明
		m_fMoveCol_a *= -1.0f;	// 反転
	}

	if (pInputKey->GetTrigger(DIK_W) || pInputKey->GetRepeat(DIK_W) || pInputPad->GetTrigger(CInputPad::BUTTON_UP, 0) == true)
	{// 上移動キーが入力された
		m_SelectMenu = (MENU)((m_SelectMenu - 1 + MENU_MAX) % MENU_MAX);
		ColorSet();
	}
	else if (pInputKey->GetTrigger(DIK_S) || pInputKey->GetRepeat(DIK_S) || pInputPad->GetTrigger(CInputPad::BUTTON_DOWN, 0) == true)
	{// 下移動キーが入力された
		
		m_SelectMenu = (MENU)((m_SelectMenu + 1) % MENU_MAX);
		ColorSet();
	}
	else if (pInputKey->GetTrigger(DIK_RETURN) || pInputPad->GetTrigger(CInputPad::BUTTON_A, 0) == true)
	{// 決定キーが押された

		if (pFade == NULL)
		{
			return bValue;
		}
		switch (m_SelectMenu)
		{
		case MENU_CONTINUE:
			break;
		case MENU_RETRY:
			pFade->Set(CScene::MODE_GAME);
			break;
		case MENU_QUIT:
			pFade->Set(CScene::MODE_TITLE);
			break;
		}

		if (m_SelectMenu != MENU_CONTINUE)
		{// 何もせず戻るとき以外
			m_bSelect = true;		//押された状態に変更
		}
		else {
			bValue = true;
		}
	}

	// 選択肢の色変更
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_aMenu[nCnt].pObject2D != NULL)
		{// 使用されている
			// 色設定
			m_aMenu[nCnt].pObject2D->SetCol(m_aMenu[nCnt].col);
		}
	}

	return bValue;
}

//===============================================
// 描画処理
//===============================================
void CPause::Draw(void)
{
	
}

//===============================================
// 色設定
//===============================================
void CPause::ColorSet(void)
{
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_SelectMenu == nCnt)
		{// 選択されている
			m_aMenu[nCnt].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else
		{// 選択外
			m_aMenu[nCnt].col = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
		}

		if (m_aMenu[nCnt].pObject2D != NULL)
		{// 使用されている
			// 色設定
			m_aMenu[nCnt].pObject2D->SetCol(m_aMenu[nCnt].col);
		}
	}

	// 透明度移動量設定
	m_fMoveCol_a = MOVECOLA;
}

//===============================================
// 生成
//===============================================
CPause *CPause::Create(void)
{
	CPause *pPause = NULL;

	pPause = new CPause;

	if (pPause != NULL)
	{
		pPause->Init();
	}
	else
	{
		return NULL;
	}

	return pPause;
}

//===============================================
// 描画設定
//===============================================
void CPause::SetDraw(const bool bValue)
{
	if (m_pBg != nullptr) {
		m_pBg->SetDraw(bValue);
	}

	// 選択肢の色変更
	for (int nCnt = 0; nCnt < MENU_MAX; nCnt++)
	{
		if (m_aMenu[nCnt].pObject2D != NULL)
		{// 使用されている
		 // 色設定
			m_aMenu[nCnt].pObject2D->SetDraw(bValue);
		}
	}
}