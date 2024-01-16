//===============================================
//
// タイトル画面の管理処理 [title.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "title.h"
#include "manager.h"
#include "input.h"
#include "fade.h"
#include "fileload.h"
#include "sound.h"
#include "camera.h"
#include "objectX.h"
#include "meshdome.h"
#include "object3D.h"
#include "texture.h"

//===============================================
// 無名名前空間
//===============================================
namespace {
	const D3DXVECTOR3 ENTERPOS = { 0.0f, 0.0f, 0.0f };	// ENTER 座標
	const D3DXVECTOR3 ENTERROT = { 0.0f, 0.0f, 0.0f };	// ENTER 向き
	const int AUTOMOVE_RANKING = 640;	// ランキング自動遷移時間
	const int MOVE_TUTORIAL = 110;		// チュートリアルに遷移するまでの時間
	const int ENEMY_NUM = 3;			//演出用敵出現数
}

//===============================================
// コンストラクタ
//===============================================
CTitle::CTitle()
{
	m_nCounterRanking = 0;
	m_nCounterTutorial = 0;
	m_nCounterRanking = 0;
	m_bPush = false;
	m_pFileLoad = nullptr;
	m_pEnter = nullptr;
	m_appEnemy = nullptr;
}

//===============================================
// デストラクタ
//===============================================
CTitle::~CTitle()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CTitle::Init(void)
{
	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TITLE);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CTitle::Uninit(void)
{
	if (m_pFileLoad != nullptr)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// メモリの開放
		m_pFileLoad = nullptr;
	}
}

//===============================================
// 更新処理
//===============================================
void CTitle::Update(void)
{
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();

	CScene::Update();
}

//===============================================
// 描画処理
//===============================================
void CTitle::Draw(void)
{
	CScene::Draw();
}
