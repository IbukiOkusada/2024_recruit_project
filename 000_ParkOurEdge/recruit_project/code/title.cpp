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
#include "player.h"

//===============================================
// 無名名前空間
//===============================================
namespace {
	const D3DXVECTOR3 ENTERPOS = { 0.0f, 0.0f, 0.0f };	// ENTER 座標
	const D3DXVECTOR3 ENTERROT = { 0.0f, 0.0f, 0.0f };	// ENTER 向き
	const int AUTOMOVE_RANKING = 640;	// ランキング自動遷移時間
	const int MOVE_TUTORIAL = 120;		// チュートリアルに遷移するまでの時間
	const int PLAYER_WALLKICKTIMER = 40;
	const int ENEMY_NUM = 3;			//演出用敵出現数
	const char* FILEPASS = "data\\TXT\\player";	// ファイルのパス
	const char* FILEEXT = ".txt";				// ファイルの拡張子
	const int FILEPASS_SIZE = (200);			// ファイルのパスサイズ
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
	//ドーム追加
	CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 9000.0f, 3000.0f, 3, 8, 8);
	CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(D3DX_PI, 0.0f, 0.0f), 9000.0f, 3000.0f, 3, 8, 8);

	if (m_pFileLoad == nullptr) {
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != nullptr)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\title_model.txt");			//モデル類
		}
	}

	// カメラの初期位置設定
	CManager::GetInstance()->GetCamera()->SetPositionV(D3DXVECTOR3(-874.3f, 1124.15f, 1717.2f));
	CManager::GetInstance()->GetCamera()->SetPositionR(D3DXVECTOR3(80.0f, 95.0f, 220.0f));
	CManager::GetInstance()->GetCamera()->SetLength(350.0f);
	CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(0.0f, -2.1f, 1.79f));

	// プレイヤーの生成
	char aBodyPass[FILEPASS_SIZE] = "";		// 胴体パス
	char aLegPass[FILEPASS_SIZE] = "";		// 下半身パス
	sprintf(&aBodyPass[0], "%s\\motion_body%s", FILEPASS, FILEEXT);
	sprintf(&aLegPass[0], "%s\\motion_leg%s", FILEPASS, FILEEXT);
	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
		D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), 
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), 
		&aBodyPass[0], &aLegPass[0]);
	m_pPlayer->SetDraw(true);
	m_pPlayer->SetMotion(CPlayer::ACTION_TITLENEUTRAL);

	// 遷移タイマー設定
	m_nCounterTutorial = MOVE_TUTORIAL;
	m_nCounterRanking = AUTOMOVE_RANKING;

	// サウンド再生
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

	if (pInputKey->GetTrigger(DIK_RETURN) || pInputPad->GetTrigger(CInputPad::BUTTON_START, 0)) {
		if (!m_bPush) {
			if (m_pPlayer != nullptr) {
				m_pPlayer->SetMotion(CPlayer::ACTION_WALK);
			}
		}

		m_bPush = true;
	}

	if (m_bPush) {
		m_nCounterTutorial--;

		// 遷移までのプレイヤー移動
		if (m_nCounterTutorial == PLAYER_WALLKICKTIMER) {	// 壁キックタイミング
			if (m_pPlayer != nullptr) {
				m_pPlayer->SetMotion(CPlayer::ACTION_WALLKICK);
				m_pPlayer->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
				D3DXVECTOR3 move = { 0.0f, 16.0f, 0.0f };
				m_pPlayer->SetMove(move);
				m_pPlayer->SetType(CPlayer::TYPE_ACTIVE);
				m_pPlayer->SetRotDiff(0.0f);
			}
		}
		else if (m_nCounterTutorial <= PLAYER_WALLKICKTIMER) {	// 壁キック後
			if (m_pPlayer != nullptr) {
				D3DXVECTOR3 pos = m_pPlayer->GetPosition();
				pos.z += -6.0f;
				m_pPlayer->SetPosition(pos);
			}
		}
		else {	// それまで
			if (m_pPlayer != nullptr) {
				D3DXVECTOR3 pos = m_pPlayer->GetPosition();
				pos.z += 4.0f;
				m_pPlayer->SetPosition(pos);
			}
		}

		if (m_nCounterTutorial <= 0) {
			CManager::GetInstance()->GetFade()->Set(CScene::MODE_GAME);
		}
	}
	else {
		m_nCounterRanking--;
		if (m_nCounterRanking <= 0) {
			CManager::GetInstance()->GetFade()->Set(CScene::MODE_RANKING);
		}
	}

	CScene::Update();
}

//===============================================
// 描画処理
//===============================================
void CTitle::Draw(void)
{
	CScene::Draw();
}
