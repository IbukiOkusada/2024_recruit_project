//===============================================
//
// チュートリアル画面の管理処理 [tutorial.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "tutorial.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "time.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "fileload.h"
#include "Xfile.h"
#include "input.h"
#include "fade.h"
#include "result.h"
#include "sound.h"
#include "debugproc.h"
#include "player.h"
#include "game.h"
#include "object3D.h"
#include "particle.h"
#include "sound.h"
#include "player_manager.h"

// 無名名前空間
namespace
{
	const char* FILEPASS = "data\\TXT\\player";	// ファイルのパス
	const char* FILEEXT = ".txt";				// ファイルの拡張子
	const int FILEPASS_SIZE = (200);	// ファイルのパスサイズ
	const D3DXVECTOR2 PORISIZE = D3DXVECTOR2(200.0f, 50.0f);
}

//===============================================
// マクロ定義
//===============================================

//===============================================
// 静的メンバ変数
//===============================================

//===============================================
// コンストラクタ
//===============================================
CTutorial::CTutorial()
{
	// 値のクリア
	m_pFileLoad = NULL;

	for (int i = 0; i < NUM_PLAYER; i++)
	{
		m_apObject[i] = nullptr;
	}

	for (int i = 0; i < NUM_PORI; i++)
	{
		m_pObject3D[i] = nullptr;
	}

	for (int i = 0; i < NUM_ENEMY; i++)
	{
		m_apEnemy[i] = nullptr;
	}
	
	m_nCntRespawn = 0;
	m_nEnemyId = 0;
	m_bEnd = false;
}

//===============================================
// デストラクタ
//===============================================
CTutorial::~CTutorial()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CTutorial::Init(void)
{
	// 外部ファイル読み込みの生成
	if (m_pFileLoad == NULL)
	{// 使用していない場合
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != NULL)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\tutorial.txt");
		}
	}

	//カメラ初期化
	{
		//CManager::GetInstance()->GetCamera()->Init();

		CManager::GetInstance()->GetCamera()->SetPositionV(D3DXVECTOR3(-874.3f, 1124.15f, 1717.2f));
		CManager::GetInstance()->GetCamera()->SetPositionR(D3DXVECTOR3(-320.3f, 1.0f, -91.6f));
		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, D3DX_PI * 0.1f));

		D3DVIEWPORT9 viewport;
		//プレイヤー追従カメラの画面位置設定
		viewport.X = 0;
		viewport.Y = 0;
		viewport.Width = (DWORD)(SCREEN_WIDTH * 1.0f);
		viewport.Height = (DWORD)(SCREEN_HEIGHT * 1.0f);
		viewport.MinZ = 0.0f;
		viewport.MaxZ = 1.0f;
		CManager::GetInstance()->GetCamera()->SetViewPort(viewport);
		CManager::GetInstance()->GetCamera()->SetActive(false);
	}

	// 人数分ポインタ生成
	m_ppPlayer = new CPlayer*[PLAYER_MAX];

	m_ppPlayer[0] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		"data\\TXT\\player0\\motion_ninjabody.txt", "data\\TXT\\player0\\motion_ninjaleg.txt");
	m_ppPlayer[0]->BindId(0);
	m_ppPlayer[0]->SetType(CPlayer::TYPE_ACTIVE);

	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_TUTORIAL);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CTutorial::Uninit(void)
{
	if (m_pFileLoad != NULL)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// メモリの開放
		m_pFileLoad = NULL;
	}

	for (int i = 0; i < NUM_PORI; i++)
	{
		if (m_pObject3D[i] != nullptr)
		{// 使用されている場合

			// 使用していない状態にする
			m_pObject3D[i] = nullptr;
		}
	}

	for (int i = 0; i < NUM_ENEMY; i++)
	{
		if (m_apEnemy[i] != nullptr)
		{// 使用されている場合

		 // 使用していない状態にする
			m_apEnemy[i] = nullptr;
		}
	}

	if (m_ppPlayer != NULL)
	{// 使用していた場合
		int nNum = CPlayerManager::GetInstance()->GetNum();
		for (int nCnt = 0; nCnt < nNum; nCnt++)
		{
			// 終了処理
			m_ppPlayer[nCnt]->Uninit();
			m_ppPlayer[nCnt] = NULL;	// 使用していない状態にする
		}

		delete[] m_ppPlayer;	// ポインタの開放
		m_ppPlayer = NULL;	// 使用していない状態にする
	}

	CManager::GetInstance()->GetCamera()->SetActive(true);
}

//===============================================
// 更新処理
//===============================================
void CTutorial::Update(void)
{
	if (CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_START, 0) || CManager::GetInstance()->GetInputKeyboard()->GetTrigger(DIK_RETURN))
	{
		CManager::GetInstance()->GetFade()->Set(CScene::MODE_GAME);
		CGame::SetNumPlayer(CPlayerManager::GetInstance()->GetNum());

	}

	bool bCreate = false;

	// 更新処理
	CScene::Update();
}

//===============================================
// 描画処理
//===============================================
void CTutorial::Draw(void)
{
	// 描画処理
	CScene::Draw();
}

//===================================================
// ファイル読み込みの取得
//===================================================
CFileLoad *CTutorial::GetFileLoad(void)
{
	return m_pFileLoad;
}
