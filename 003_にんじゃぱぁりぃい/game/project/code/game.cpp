//===============================================
//
// ゲーム画面の管理処理 [game.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "game.h"
#include "manager.h"
#include "renderer.h"
#include "sound.h"
#include "camera.h"
#include "light.h"
#include "texture.h"
#include "objectX.h"
#include "Xfile.h"
#include "input.h"
#include "fade.h"
#include "result.h"
#include "sound.h"
#include "fileload.h"
#include "debugproc.h"
#include "time.h"
#include "meshdome.h"
#include "tcp_client.h"
#include <thread>
#include "protocol_online.h"
#include "object2D.h"
#include "player.h"
#include "enemy.h"
#include "enemymanager.h"
#include "item.h"
#include "spike.h"
#include "itembox.h"
#include "gimmick_button.h"
#include "gimmick_pitfall.h"
#include "gimmick_rotatedoor.h"
#include "gimmick_startdoor.h"
#include "gimmick_lever.h"
#include "gimmick_spear.h"
#include "goal.h"
#include "minimap.h"
#include "ui.h"
#include "score.h"
#include "gimmick_multidoor.h"
#include "minimap.h"
#include "gimmick_pull.h"
#include "pause.h"
#include "quataui.h"
#include <assert.h>
#include "particle.h"
#include "meshdome.h"

// 無名名前空間を定義
namespace {
    const D3DXVECTOR3 STARTDOORPOS = { -1160.0f, 0.0f, 950.0f };	// スタート地点ドア基本座標
	const D3DXVECTOR3 PLAYERSTARTPOS = { -2500.0f, 0.0f, 990.0f };  // プレイヤーのスタート位置
    const D3DXVECTOR3 LEVERPOS[4] =
    {
        D3DXVECTOR3(130.0f, 100.0f, -5130.0f),
        D3DXVECTOR3(-1000.0f, 100.0f, -4440.0f),
        D3DXVECTOR3(470.0f, 100.0f, -560.0f),
        D3DXVECTOR3(360.0f, 100.0f, -1900.0f),
    };

    const D3DXVECTOR3 LEVERROT[4] =
    {
        D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
        D3DXVECTOR3(0.0f, D3DX_PI, 0.0f),
        D3DXVECTOR3(0.0f, 0.0f, 0.0f),
        D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f),
    };


	const char* TEXPASS[4] =
	{
		"data\\TEXTURE\\opendoor00.png",
		"data\\TEXTURE\\opendoor01.png",
		"data\\TEXTURE\\opendoor02.png",
		"data\\TEXTURE\\opendoor03.png",
	};

	const D3DXVECTOR3 OPEN_SETPOS = { SCREEN_WIDTH * 1.3f, SCREEN_HEIGHT * 0.5f, 0.0f };  // スタートドア開いたときのUIの生成位置
	const D3DXVECTOR3 OPEN_SETROT = { 0.0f, 0.0f, D3DX_PI * 0.0f };                       // 向き
	const D3DXVECTOR2 OPEN_SIZE = { 470.0f, 150.0f };                                     // サイズ
	const float OPEN_MOVESPEED = (-3.3f);                                                 // 移動スピード
	const float OPEN_MOVESIN = (0.05f);
	const float OPEN_MOVESIZE = (30.0f);
    const D3DXVECTOR2 QUATAUI_SIZE = { 100.0f, 50.0f };	// ノルマのUIのサイズ
    const D3DXVECTOR2 SCORE_SIZE = { 14.0f, 18.0f };	// スコアのサイズ
    const float DOOR_SPACE = (-20.0f);			// 各スタート地点ドアの間
    const char* FILEPASS = "data\\TXT\\player";	// ファイルのパス
    const char* FILEEXT = ".txt";				// ファイルの拡張子
    const int FILEPASS_SIZE = (200);			// ファイルのパスサイズ
    const int START_TIMER = (210);				// 開始制限時間
    const int START_WAITCNT = (430);            // スタート時の走ってる時間
	const int PLAYER_MOVESTART = (180);
	const int CAMERA_ROTATESTART = (240);
	const D3DXVECTOR3 START_CAMERAROT = {0.0f, D3DX_PI * 0.0f, D3DX_PI * 0.38f};
    const int SCORE = (15000);                   // 初期のスコア
    const int UNINITCOUNT = (120);              // ノルマのUIが消えるまでの時間
	const int PLAYER_SPWANSTART = (240);
}

//===============================================
// マクロ定義
//===============================================
#define START_TIME	(60 * 4)	// 制限時間
#define START_SCORE	(0)		// 開始スコア
#define MAP_SIZE	(100.0f)	// マップサイズ
#define STARTSET_NUMENEMY	(3)	// 開始時に配置する敵の数
#define MORNING_TIME	(60 * 6)	// 早朝終了時刻
#define AM_COMRUSH	(60 * 9)	// 通勤ラッシュ
#define RELAX_TIME	(60 * 11)	// リラックスタイム
#define NOON_TIME	(60 * 12.5)	// 昼休み
#define EVENT_RUSH	(60 * 15.5)	// イベント
#define PM_RELAX_TIME	(60 * 16)	// 帰宅開始時間
#define PM_GOHOME_RUSH	(60 * 19)	// 帰宅ラッシュ
#define MAX_TIME	(60 * 20 + 10)	// 最大時間
#define SPEED_UP	(30.0f)
#define DEF_PORT	(22333)	// ポート番号
#define MAX_STRING	(2048)
#define ADDRESSFILE	"data\\TXT\\address.txt"
#define WIDTH_NUM		(2)		// 横の分割数
#define HEIGHT_NUM	(2)		// 縦の分割数
#define STANDARDSCORE  (1000)   // ノルマの基準点

//===============================================
// 静的メンバ変数
//===============================================
CGame::STATE CGame::m_state = CGame::STATE_LOCAL;	// 状態
int CGame::m_nNumPlayer = 0;

//===============================================
// コンストラクタ
//===============================================
CGame::CGame()
{
    // 値のクリア
    m_ppCamera = nullptr;
    m_ppPlayer = nullptr;
    m_pFileLoad = nullptr;
    m_pMeshDome = nullptr;
    m_pClient = nullptr;
    m_pTimer = nullptr;
    m_QuataScore = nullptr;
    m_QuataUI = nullptr;
	m_ppLever = nullptr;
	m_ppMultiDoor = nullptr;
    m_nSledCnt = 0;
    m_bEnd = false;
    m_nStartCnt = 0;
    m_nCntLostQuataUI = 0;
	m_nCntGoal = 0;
	m_fOpenDoorUISin = 0.0f;
    m_bPause = false;
    m_pPause = nullptr;
    m_bQuota = false;
    m_bDispQuataUI = false;
	m_bOpenStartDoor = false;
	m_pEnemy = nullptr;
}

//===============================================
// コンストラクタ(人数設定)
//===============================================
CGame::CGame(int nNumPlayer)
{
    // 値のクリア
    m_ppCamera = nullptr;
    m_ppPlayer = nullptr;
    m_pFileLoad = nullptr;
    m_pMeshDome = nullptr;
    m_pClient = nullptr;
    m_pTimer = nullptr;
    m_QuataScore = nullptr;
    m_QuataUI = nullptr;
	m_ppLever = nullptr;
    m_nSledCnt = 0;
    m_bEnd = false;
    m_nStartCnt = 0;
    m_nCntLostQuataUI = 0;
	m_nCntGoal = 0;
	m_fOpenDoorUISin = 0.0f;
    m_bPause = false;
    m_pPause = nullptr;
    m_bQuota = false;
    m_bDispQuataUI = false;
	m_bOpenStartDoor = false;

    // 人数設定
    m_nNumPlayer = nNumPlayer;
}

//===============================================
// デストラクタ
//===============================================
CGame::~CGame()
{
   
}

//===============================================
// 初期化処理
//===============================================
HRESULT CGame::Init(void)
{
    memset(&m_aAddress[0], '\0', sizeof(m_aAddress));
    int nErr = WSAStartup(WINSOCK_VERSION, &m_wsaData);	// winsockの初期化関数

    if (nErr != 0)
    {// 初期化に失敗した場合
        
    }

    // 外部ファイル読み込みの生成
    if (nullptr == m_pFileLoad)
    {// 使用していない場合
        m_pFileLoad = new CFileLoad;

        if (m_pFileLoad != NULL)
        {
            m_pFileLoad->Init();
            m_pFileLoad->OpenFile("data\\TXT\\model.txt");			//モデル類
            m_pFileLoad->OpenFile("data\\TXT\\enemy_point.txt");	//敵周回ポイント
			m_pFileLoad->OpenFile("data\\TXT\\setitem.txt");	    // アイテムの配置
        }
    }

    switch (m_state)
    {
    case STATE_LOCAL:
    {// ローカルの場合
        if (m_nNumPlayer <= 0)
        {// 人数が指定されていない
            m_nNumPlayer = 1;
        }

        // 人数分ポインタ生成
        m_ppPlayer = new CPlayer*[m_nNumPlayer];

        for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
        {
            m_ppPlayer[nCnt] = nullptr;

            char aBodyPass[FILEPASS_SIZE] = "";		// 胴体パス
            char aLegPass[FILEPASS_SIZE] = "";		// 下半身パス

            sprintf(&aBodyPass[0], "%s%d\\motion_ninjabody%s", FILEPASS, nCnt, FILEEXT);
            sprintf(&aLegPass[0], "%s%d\\motion_ninjaleg%s", FILEPASS, nCnt, FILEEXT);

            m_ppPlayer[nCnt] = CPlayer::Create(D3DXVECTOR3(PLAYERSTARTPOS.x, PLAYERSTARTPOS.y, PLAYERSTARTPOS.z - nCnt * 25.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),&aBodyPass[0], &aLegPass[0]);
            m_ppPlayer[nCnt]->BindId(nCnt);

            //スコアとUIの高さと間隔の調整用
            float fData = 0.0f;
            float fData1 = 0.0f;
            float fData2 = 0.0f;

            if (nCnt == 1 || nCnt == 3)
            {
                fData = 1035.0f;
            }
            else
            {
                fData = 0.0f;
            }

            if (nCnt == 2 || nCnt == 3)
            {
                fData1 = 620.0f;
                fData2 = 50.0f;

            }
            else
            {
                fData1 = 0.0f;
                fData2 = 0.0f;
            }

            // UIの生成
            CUI *pUI = CUI::Create(D3DXVECTOR3(130.0f + fData, 50.0f + fData1, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), nCnt, nCnt, CUI::TYPE_LEFTUP);
            m_ppPlayer[nCnt]->BindUI(pUI);
            
            // スコアの生成
            CScore * pScore = CScore::Create(D3DXVECTOR3(40.0f + fData, 23.0f + fData1 + fData2, 0.0f), 6, 0.9f, SCORE_SIZE.x, SCORE_SIZE.y);
            m_ppPlayer[nCnt]->BindScore(pScore);
			m_ppPlayer[nCnt]->SetDraw(false);
        }

		if (m_pFileLoad != NULL)
		{
			m_pFileLoad->OpenFile("data\\TXT\\gimmick.txt");	//敵周回ポイント
		}

        // ギミックの生成

		m_ppLever = new CGimmickLever*[m_nNumPlayer];
		m_ppMultiDoor = new CGimmickStartDoor*[m_nNumPlayer];
        // 開始扉(人数分)
        for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++) {
			m_ppLever[nCnt] = CGimmickLever::Create(LEVERPOS[nCnt]);
			m_ppLever[nCnt]->SetRotation(LEVERROT[nCnt]);
			m_ppMultiDoor[nCnt] = CGimmickStartDoor::Create(D3DXVECTOR3(STARTDOORPOS.x + nCnt * DOOR_SPACE, STARTDOORPOS.y, STARTDOORPOS.z));
            m_ppMultiDoor[nCnt]->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
            m_ppMultiDoor[nCnt]->SetLever(m_ppLever[nCnt]);
        }

        // ゴール
        CGoal::Create(D3DXVECTOR3(STARTDOORPOS.x + PLAYER_MAX * DOOR_SPACE, 2.0f, STARTDOORPOS.z), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f), 100.0f);

    }
        break;

    case STATE_ONLINE:
    {// オンライン通信の場合
        m_pClient = new CClient;
        AddressLoad(&m_aAddress[0]);

        if (m_pClient->Init(&m_aAddress[0], DEF_PORT))
        {// 初期接続成功
            // オンライン関数をマルチスレッド
            std::thread th(&CGame::Online, this);
            th.detach();
        }
    }
        break;

    default:

        break;
    }

    //敵マネージャ生成（投げっぱ）
    CEnemyManager::Create();

    //壺
    CItemBox::Create(D3DXVECTOR3(-800.0f, 0.0f, -4250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CItemBox::Create(D3DXVECTOR3(-1000.0f, 0.0f, -4250.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CItemBox::Create(D3DXVECTOR3(-500.0f, 0.0f, -3400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CItemBox::Create(D3DXVECTOR3(-680.0f, 0.0f, 1775.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CItemBox::Create(D3DXVECTOR3(-880.0f, 0.0f, -2610.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
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
    }

    // 分割カメラ生成
    {
        // defaultカメラオフ
        CManager::GetInstance()->GetCamera()->SetDraw(false);

        // 人数分ポインタ生成
        m_ppCamera = new CMultiCamera*[m_nNumPlayer];

        for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
        {
            m_ppCamera[nCnt] = new CMultiCamera;
            m_ppCamera[nCnt]->Init();
            m_ppCamera[nCnt]->SetPositionV(D3DXVECTOR3(-874.3f, 1124.15f, 1717.2f));
            m_ppCamera[nCnt]->SetPositionR(D3DXVECTOR3(-320.3f, 1.0f, -91.6f));
            m_ppCamera[nCnt]->SetLength(400.0f);
            m_ppCamera[nCnt]->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI * 1.0f, D3DX_PI * 0.41f));

            D3DVIEWPORT9 viewport;
            //プレイヤー追従カメラの画面位置設定
            viewport.X = (DWORD)((SCREEN_WIDTH * 0.5f) * (nCnt % WIDTH_NUM));
            viewport.Y = (DWORD)((SCREEN_HEIGHT * 0.5f) * (nCnt / WIDTH_NUM));

            if (m_nNumPlayer < WIDTH_NUM) {
                viewport.Width = (DWORD)(SCREEN_WIDTH * 1.0f);
            }
            else
            {
                viewport.Width = (DWORD)(SCREEN_WIDTH * 0.5f);
            }

            if (m_nNumPlayer <= WIDTH_NUM) {
                viewport.Height = (DWORD)(SCREEN_HEIGHT * 1.0f);
            }
            else
            {
                viewport.Height = (DWORD)(SCREEN_HEIGHT * 0.5f);
            }
            
            viewport.MinZ = 0.0f;
            viewport.MaxZ = 1.0f;
            m_ppCamera[nCnt]->SetViewPort(viewport);

            CPlayer *pPlayer = CPlayer::GetTop();

            while (pPlayer != nullptr) {
                CPlayer *pPlayerNext = pPlayer->GetNext();

                if (pPlayer->GetId() == nCnt) {
                    pPlayer->SetCamera(m_ppCamera[nCnt]);
                    m_ppCamera[nCnt]->BindId(pPlayer->GetId());
                    break;
                }

                pPlayer = pPlayerNext;
            }
        }
    }

	// 演出用の敵を追加
	m_pEnemy = CEnemy::Create(PLAYERSTARTPOS, D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL, NULL, ExPattern::POINTID_GAMESTART);;

    // タイムの生成
    m_pTimer = CTime::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.4375f, SCREEN_HEIGHT * 0.05f, 0.0f));
    m_pTimer->Set(START_TIMER);
    m_pTimer->SetActive(true);

    // スポットライトをオン
    CManager::GetInstance()->GetLight()->EnablePointLight(true);

    CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_GAME);

    //ミニマップ生成
    if (m_pMiniMap == nullptr)
    {
        m_pMiniMap = CMiniMap::Create(m_nNumPlayer, 10, 10);	//生成
        m_pMiniMap->DrawTexture();	//ミニマップテクスチャの描画
    }

	if (m_QuataUI == nullptr)
	{
		m_QuataUI = CQuataUI::Create(D3DXVECTOR3(SCREEN_WIDTH + 100.0f, SCREEN_HEIGHT * 0.5f, 0.0f), CQuataUI::TYPE_START, CQuataUI::STATE_MOVE, QUATAUI_SIZE.x, QUATAUI_SIZE.y);
		m_bDispQuataUI = true;
	}

	// ノルマの設定
	int QuataScore = STANDARDSCORE + (m_nNumPlayer * SCORE);
	CResult::SetQuata(QuataScore);

	// ノルマの点数
	if (m_QuataScore == nullptr)
	{
		m_QuataScore = CScore::Create(D3DXVECTOR3(SCREEN_WIDTH + 35.0f, SCREEN_HEIGHT * 0.5f, 0.0f), 6, 0.75f, 16.0f, 20.0f);
		m_QuataScore->SetScore(QuataScore);
	}

    // ポーズの生成
    m_pPause = CPause::Create();
    if (m_pPause != nullptr) {
        m_pPause->SetDraw(m_bPause);
    }

    CGimmick::SwitchOn();

    m_nCntLostQuataUI = UNINITCOUNT;

	//ドーム追加
	CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 6000.0f, 6000.0f, 3, 8, 8);

    return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CGame::Uninit(void)
{
    m_bEnd = true;

    while (1)
    {
        OnlineEnd();
        if (m_nSledCnt <= 0)
        {
            break;
        }
    }

    if (m_QuataScore != nullptr)
    {
        m_QuataScore->Uninit();
        delete m_QuataScore;
        m_QuataScore = nullptr;
    }

    if (m_pPause != nullptr) {
        m_pPause->Uninit();
        delete m_pPause;
        m_pPause = nullptr;
    }

    if (m_pFileLoad != nullptr)
    {
        m_pFileLoad->Uninit();

        delete m_pFileLoad;		// メモリの開放
        m_pFileLoad = nullptr;
    }

    if (m_pClient != nullptr)
    {
        m_pClient->Uninit();
        delete m_pClient;
        m_pClient = nullptr;
    }

    if (m_QuataScore != nullptr) {
        m_QuataScore->Uninit();
        delete m_QuataScore;
        m_QuataScore = nullptr;
    }

    if (m_ppPlayer != nullptr) { // 使用していた場合
        for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
        {
            // 終了処理
            m_ppPlayer[nCnt]->Uninit();
            m_ppPlayer[nCnt] = nullptr;	// 使用していない状態にする
        }

        delete[] m_ppPlayer;	// ポインタの開放
        m_ppPlayer = nullptr;	// 使用していない状態にする
    }

    if (m_ppCamera != nullptr) { // 使用していた場合
        for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
        {
            // 終了処理
            m_ppCamera[nCnt]->Uninit();
            delete m_ppCamera[nCnt];
            m_ppCamera[nCnt] = nullptr;	// 使用していない状態にする
        }

        delete[] m_ppCamera;	// ポインタの開放
        m_ppCamera = nullptr;	// 使用していない状態にする
    }

    if (m_pTimer != nullptr) {
        m_pTimer->Uninit();
        delete m_pTimer;
        m_pTimer = nullptr;
    }

	if (m_ppLever != nullptr)
	{// 使用されている場合

		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			// 終了処理
			m_ppLever[nCnt]->Uninit();

			// 使用されいない状態にする
			m_ppLever[nCnt] = nullptr;
		}

		// ポインタの開放
		delete[] m_ppLever;

		// 使用されていない状態にする
		m_ppLever = nullptr;
	}

	if (m_ppMultiDoor != nullptr)
	{// 使用されている場合

		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			// 終了処理
			m_ppMultiDoor[nCnt]->Uninit();

			// 使用されいない状態にする
			m_ppMultiDoor[nCnt] = nullptr;
		}

		// ポインタの開放
		delete[] m_ppMultiDoor;

		// 使用されていない状態にする
		m_ppMultiDoor = nullptr;
	}

    // defaultカメラオン
    CManager::GetInstance()->GetCamera()->SetDraw(true);

    //Winsock終了処理
    WSACleanup();	// WSACleanup関数 : winsockの終了処理

    m_state = STATE_LOCAL;
}

//===============================================
// 更新処理
//===============================================
void CGame::Update(void)
{

	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();

	if (pInputKey->GetTrigger(DIK_P) == true || pInputPad->GetTrigger(CInputPad::BUTTON_START, 0))
	{//ポーズキー(Pキー)が押された
		m_bPause = m_bPause ? false : true;

		if (m_pPause != nullptr) {
			m_pPause->SetDraw(m_bPause);
		}
	}

	if (m_bPause == true)
	{
		if (m_pPause != nullptr) {
			if (m_pPause->Update()) {
				m_bPause = false;
				m_pPause->SetDraw(m_bPause);
			}
		}
		return;
	}

	int nCntOpen = 0;
	int nCntNoPressLever = m_nNumPlayer;

	if (m_bOpenStartDoor == false)
	{
		for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
		{
			if (m_ppPlayer[nCnt]->GetType() == CPlayer::TYPE_ACTIVE)
			{
				if (m_ppLever[nCnt] != nullptr)
				{
					if (m_ppLever[nCnt]->GetState() == CGimmickLever::STATE_PRESS)
					{
						nCntOpen++;
					}
				}
			}
		}

		nCntNoPressLever -= nCntOpen;
	}

	if ((m_bOpenStartDoor == true && m_bAllOpenUI == false) || nCntOpen > m_nOldOpenDoor)
	{
		m_nOldOpenDoor = nCntOpen;

		if (m_pAllOpen == nullptr)
		{
			m_pAllOpen = CObject2D::Create(OPEN_SETPOS, OPEN_SETROT, 6);

			if (m_pAllOpen != nullptr) {	// 生成できた
				m_pAllOpen->SetLength(OPEN_SIZE.x, OPEN_SIZE.y);
				m_pAllOpen->BindTexture(CManager::GetInstance()->GetTexture()->Regist(TEXPASS[nCntNoPressLever]));
			}
		}
	}

	if (m_pAllOpen != nullptr)
	{
		D3DXVECTOR3 pos = m_pAllOpen->GetPosition();

		// 移動
		if (pos.x > SCREEN_WIDTH * 0.75f)
		{
			pos.x -= 40.0f;
		}
		else if (pos.x < SCREEN_WIDTH * 0.35f)
		{
			pos.x -= 50.0f;
			m_pAllOpen->SetLength(m_pAllOpen->GetWidth() * 1.7f, m_pAllOpen->GetHeight() * 1.7f);
		}
		else
		{
			pos.x += OPEN_MOVESPEED;

			// 大きくしたり小さくする
			m_fOpenDoorUISin += OPEN_MOVESIN;
			float fSin = sinf(m_fOpenDoorUISin);
			if (fSin >= 0.0f) {
				m_pAllOpen->SetLength(OPEN_SIZE.x + fSin * OPEN_MOVESIZE, OPEN_SIZE.y + fSin * OPEN_MOVESIZE);
			}
		}

		m_pAllOpen->SetPosition(pos);
		m_pAllOpen->SetVtx();

		if (pos.x < -SCREEN_WIDTH * 0.7f)
		{
			m_pAllOpen->Uninit();
			m_pAllOpen = nullptr;
			m_bAllOpenUI = true;
		}

		if (nCntOpen >= m_nNumPlayer)
		{
			return;
		}
	}

	for (int nCount = 0; nCount < m_nNumPlayer; nCount++)
	{
		if (m_ppCamera[nCount]->GetMode() == CCamera::MODE_STARTDOOR)
		{
			m_ppCamera[nCount]->AllOpenCamera(m_nCntLookGoal);

			m_nCntLookGoal++;

			if (m_nCntLookGoal >= 400 * m_nNumPlayer)
			{
				m_ppCamera[nCount]->SetMode(CCamera::MODE_NORMAL);
			}
		}
	}

	if (m_ppCamera[0]->GetMode() == CCamera::MODE_STARTDOOR)
	{
		if (m_nCntLookGoal < 200 * m_nNumPlayer) {
			return;
		}
		// ドアを開けてー
		if (m_ppMultiDoor != nullptr)
		{// 使用されている場合

			for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
			{
				// 更新処理
				m_ppMultiDoor[nCnt]->Update();
			}
		}

		return;
	}

	if (m_QuataScore != nullptr)
	{
		if (m_QuataUI != nullptr)
		{
			if (m_QuataUI->GetState() == CQuataUI::STATE_MOVE)
			{
				D3DXVECTOR3 pos = m_QuataScore->GetPosition();
				pos.x -= 8.0f;
				m_QuataScore->SetPosition(pos);
			}

			if (m_QuataUI->GetState() == CQuataUI::STATE_UP)
			{
				if (m_QuataUI->GetCounter() <= 0.0f) {
					D3DXVECTOR3 pos = m_QuataScore->GetPosition();
					pos.y -= 2.5f;
					m_QuataScore->SetPosition(pos);
				}
			}

			if (m_QuataUI->GetState() == CQuataUI::STATE_SET)
			{
				D3DXVECTOR3 pos = m_QuataScore->GetPosition();
				pos.y -= 2.5f * 0.13f;
				m_QuataScore->SetPosition(pos);
			}
		}
	}

	// 開始タイマー
	if (!StartDirection())
	{	// 時間切れ
		if (m_state != STATE_END) {	// 終了状態以外
			if (EndCheck()) {	// 全員ゴールしている
				CManager::GetInstance()->GetFade()->Set(CScene::MODE_RESULT);
				m_state = STATE_END;
			}
			else
			{
				if (m_pTimer != nullptr) {

					if (m_QuataUI != nullptr) {
						if (m_QuataUI->GetState() == CQuataUI::STATE_NONE) {
							m_pTimer->Update();
						}
					}
					else {
						m_pTimer->Update();
					}

					if (m_pTimer->GetNum() <= 0) {	// タイムオーバー
						CManager::GetInstance()->GetFade()->Set(CScene::MODE_RESULT);
						CResult::SetNumPlayer(m_nNumPlayer);
						CResult::SetScore(m_ppPlayer);
						m_state = STATE_END;
					}
				}
			}
		}
	}

	if (nCntOpen >= m_nNumPlayer && m_bOpenStartDoor == false)
	{
		for (int nCount = 0; nCount < m_nNumPlayer; nCount++)
		{
			if (m_ppCamera[nCount] != nullptr)
			{
				m_ppCamera[nCount]->SetMode(CCamera::MODE_STARTDOOR);
			}
		}

		m_bOpenStartDoor = true;
	}

	if (CManager::GetInstance()->GetMode() == CScene::MODE_GAME)
	{
		CScene::Update();
	}

	CGimmick::Buttonoff();
}

//===============================================
// 描画処理
//===============================================
void CGame::Draw(void)
{
    if (m_pMiniMap != nullptr) {
        m_pMiniMap->ExploredMap();
    }

    CScene::Draw();
}

//===================================================
// プレイヤーの取得
//===================================================
CPlayer *CGame::GetPlayer(void)
{
    return *m_ppPlayer;
}

//===================================================
// ファイル読み込みの取得
//===================================================
CFileLoad *CGame::GetFileLoad(void)
{
    return m_pFileLoad;
}

//===================================================
// 終了状態を確認して取得
//===================================================
bool CGame::EndCheck(void)
{
    CPlayer *pPl = CPlayer::GetTop();	// プレイヤー
    int nNumGoal = 0;

    // ゴールしている人数を判定
    while (pPl != nullptr) {

        CPlayer *pPlNext = pPl->GetNext();	// 次を覚える

        if (!pPl->GetEnd()) {	// ゴールしていない
            break;
        }

        nNumGoal++;
        pPl = pPlNext;	// 次に移動
    }

    if (nNumGoal >= CPlayer::GetNum()) {	// 全員ゴール

        CResult::SetNumPlayer(m_nNumPlayer);
        CResult::SetScore(m_ppPlayer);

        return true;
    }

    return false;
}

//===================================================
// オンライン通信
//===================================================
void CGame::Online(void)
{
    m_nSledCnt = 1;

    while (1)
    {
        if (m_ppPlayer == NULL || m_bEnd == true)
        {
            break;
        }

        char aRecvData[MAX_STRING] = {};	// 受信用

        // 受信
        int nRecvByte = m_pClient->Recv(&aRecvData[0], MAX_STRING);

        // マルチスレッド
        std::thread th(&CGame::ByteCheck, this, &aRecvData[0], nRecvByte);
        th.detach();
    }

    m_nSledCnt--;
}

//===================================================
// 受信確認
//===================================================
void CGame::ByteCheck(char *pRecvData, int nRecvByte)
{
    m_mutex.lock();

    m_nSledCnt++;
    D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    D3DXCOLOR col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
    int nType = COMMAND_TYPE_NONE;	// コマンド番号
    int nId = -1;		// ID
    int nByte = 0;	// バイト数
    int nDamage = 0;	// ダメージ数
    CPlayer *pPlayer = NULL;		// 先頭を取得
    CPlayer *pPlayerNext = NULL;	// 次を保持

    if (nRecvByte <= 0)
    {
        m_nSledCnt--;
        m_mutex.unlock();
        return;
    }

    pPlayer = CPlayer::GetTop();	// 先頭を取得

    // 終端文字まで確認する
    while (nByte < nRecvByte)
    {
        bool bIn = false;	// 既にいるかどうか
        memcpy(&nId, &pRecvData[nByte], sizeof(int));
        nByte += sizeof(int);

        // コマンド取得
        memcpy(&nType, &pRecvData[nByte], sizeof(int));
        nByte += sizeof(int);

        if (nId != -1)
        {
            // コマンドごとに分ける
            switch (nType)
            {

            case COMMAND_TYPE_SETPOSITION:

                memcpy(&pos, &pRecvData[nByte], sizeof(D3DXVECTOR3));
                nByte += sizeof(D3DXVECTOR3);
                break;

            case COMMAND_TYPE_SETROTATION:

                memcpy(&pos, &pRecvData[nByte], sizeof(D3DXVECTOR3));
                nByte += sizeof(D3DXVECTOR3);
                break;

            case COMMAND_TYPE_SETLIFE:

                memcpy(&nDamage, &pRecvData[nByte], sizeof(int));
                nByte += sizeof(int);
                break;

            case COMMAND_TYPE_START_OK:

                break;

            case COMMAND_TYPE_CREATE:
                break;

            case COMMAND_TYPE_DAMAGE:

                memcpy(&nDamage, &pRecvData[nByte], sizeof(int));
                nByte += sizeof(int);
                break;

            case COMMAND_TYPE_DELETE:
                break;

            case COMMAND_TYPE_GETID:
                break;
            }
        }

        if (m_ppPlayer == NULL)
        {
            m_nSledCnt--;
            m_mutex.unlock();
            return;
        }

        if (nId != -1 && (*m_ppPlayer)->GetId() != -1)
        {// IDを受信できた

            pPlayer = CPlayer::GetTop();	// 先頭を取得

            while (pPlayer != NULL)
            {// 使用されている間繰り返し
                pPlayerNext = pPlayer->GetNext();	// 次を保持

                if (nId == pPlayer->GetId() && (*m_ppPlayer)->GetId() != nId)
                {// 自分以外かつ操作していない

                 // コマンドごとに分ける
                    switch (nType)
                    {
                    case COMMAND_TYPE_SETPOSITION:

                        pPlayer->SetDiffPosition(pos);
                        break;

                    case COMMAND_TYPE_SETROTATION:

                        pPlayer->SetRotation(pos);
                        break;

                    case COMMAND_TYPE_SETLIFE:

                        pPlayer->SetLife(nDamage);
                        break;

                    case COMMAND_TYPE_START_OK:

                        break;

                    case COMMAND_TYPE_CREATE:

                        break;

                    case COMMAND_TYPE_DAMAGE:

                        pPlayer->Damage(nDamage);
                        break;

                    case COMMAND_TYPE_GOAL:

                        break;

                    case COMMAND_TYPE_DELETE:

                        pPlayer->Uninit();
                        break;

                    case COMMAND_TYPE_GETID:

                        break;
                    }

                    bIn = true;	// いる状態にする
                    break;
                }
                else if (nId == pPlayer->GetId() && (*m_ppPlayer)->GetId() == nId)
                {// 自分以外かつ操作キャラ

                    bIn = true;	// いる状態にする
                    break;
                }

                pPlayer = pPlayerNext;	// 次に移動
            }

            if (bIn == false && (*m_ppPlayer)->GetId() != -1 && nType > COMMAND_TYPE_NONE && nType < COMMAND_TYPE_MAX)
            {// まだ存在していない場合
                pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL, NULL);
                pPlayer->BindId(nId);
                pPlayer->SetType(CPlayer::TYPE_NONE);
            }
        }
        else if (nId == -1 && (*m_ppPlayer)->GetId() == -1)
        {// IDが受信できていないかつ自分自身のIDも存在していない
            nId = nType;

            // 自分のIDを設定
            (*m_ppPlayer)->BindId(nId);

            break;
        }
    }

    m_nSledCnt--;
    m_mutex.unlock();
}

//===================================================
// 座標送信
//===================================================
void CGame::SendPosition(D3DXVECTOR3& pos)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// 送信用
        int nProt = COMMAND_TYPE_SETPOSITION;

        // protocolを挿入
        memcpy(&aSendData[0], &nProt, sizeof(int));

        // 座標を挿入
        memcpy(&aSendData[sizeof(int)], &pos, sizeof(D3DXVECTOR3));

        // 送信
        m_pClient->Send(&aSendData[0], sizeof(int) + sizeof(D3DXVECTOR3));
    }
}

//===================================================
// 向き送信
//===================================================
void CGame::SendRotation(D3DXVECTOR3& rot)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// 送信用
        int nProt = COMMAND_TYPE_SETROTATION;

        // protocolを挿入
        memcpy(&aSendData[0], &nProt, sizeof(int));

        // 座標を挿入
        memcpy(&aSendData[sizeof(int)], &rot, sizeof(D3DXVECTOR3));

        // 送信
        m_pClient->Send(&aSendData[0], sizeof(int) + sizeof(D3DXVECTOR3));
    }
}

//===================================================
// ダメージ送信
//===================================================
void CGame::SendDamage(int nDamage)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// 送信用
        int nProt = COMMAND_TYPE_DAMAGE;

        // protocolを挿入
        memcpy(&aSendData[0], &nProt, sizeof(int));

        // ダメージを挿入
        memcpy(&aSendData[sizeof(int)], &nDamage, sizeof(int));

        // 送信
        m_pClient->Send(&aSendData[0], sizeof(int) + sizeof(int));
    }
}

//===================================================
// 体力送信
//===================================================
void CGame::SendLife(int nLife)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// 送信用
        int nProt = COMMAND_TYPE_SETLIFE;

        // protocolを挿入
        memcpy(&aSendData[0], &nProt, sizeof(int));

        // ダメージを挿入
        memcpy(&aSendData[sizeof(int)], &nLife, sizeof(int));

        // 送信
        m_pClient->Send(&aSendData[0], sizeof(int) + sizeof(int));
    }
}

//===================================================
// 終了送信
//===================================================
void CGame::OnlineEnd(void)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// 送信用
        int nProt = COMMAND_TYPE_DELETE;

        // protocolを挿入
        memcpy(&aSendData[0], &nProt, sizeof(int));

        // 送信
        m_pClient->Send(&aSendData[0], sizeof(int));
    }
}

//===============================================
// 体力設定
//===============================================
void CGame::SendSetUp(void)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// 送信用
        int nProt = COMMAND_TYPE_START_OK;

        // protocolを挿入
        memcpy(&aSendData[0], &nProt, sizeof(int));

        // 送信
        m_pClient->Send(&aSendData[0], sizeof(int));
    }
}

//===============================================
// ゴール送信
//===============================================
void CGame::SendGoal(void)
{
    if (m_pClient != nullptr)
    {
        char aSendData[MAX_STRING] = {};	// 送信用
        int nProt = COMMAND_TYPE_GOAL;

        // protocolを挿入
        memcpy(&aSendData[0], &nProt, sizeof(int));


        // 送信
        m_pClient->Send(&aSendData[0], sizeof(int));
    }
}

//===================================================
// アドレス読み込み
//===================================================
void CGame::AddressLoad(char *pAddrss)
{
    FILE *pFile;	// ファイルへのポインタ

    pFile = fopen(ADDRESSFILE, "r");

    if (pFile != NULL)
    {//ファイルが開けた場合
        
        //テキスト読み込み
        fscanf(pFile, "%s", pAddrss);

        //ファイルを閉じる
        fclose(pFile);
    }
    else
    {//ファイルが開けなかった場合
        return;
    }
}

//===================================================
// 開始演出
//===================================================
bool CGame::StartDirection(void)
{
	bool bDirection = false;

	if (m_nStartCnt < START_WAITCNT) {	// 規定値未満
		m_nStartCnt++;
		bDirection = true;

		// タイミングごとに動かす
		if (m_nStartCnt == START_WAITCNT - PLAYER_SPWANSTART) {	// プレイヤー移動開始
			if (m_ppPlayer != nullptr) { // 使用していた場合
				for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++) {
					if (m_ppPlayer[nCnt] != nullptr) {
						m_ppPlayer[nCnt]->SetDraw(true);
						// 煙のパーティクル生成
						CParticle::Create(m_ppPlayer[nCnt]->GetPosition(), CEffect::TYPE_SMAKE);
					}
				}
			}
		}
		else if (m_nStartCnt == START_WAITCNT - PLAYER_MOVESTART) {	// プレイヤー移動開始
			if (m_ppPlayer != nullptr) { // 使用していた場合
				for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++) {
					if (m_ppPlayer[nCnt] != nullptr) {
						m_ppPlayer[nCnt]->SetType(CPlayer::TYPE_AUTOMOVE);	// 自動移動状態に変更
					}
				}
			}
		}
		else if (m_nStartCnt >= START_WAITCNT - PLAYER_MOVESTART) {	// プレイヤーが移動している
			if (m_ppCamera != nullptr) { // 使用していた場合
				for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++) {	// 全てのカメラの距離を離していく
					m_ppCamera[nCnt]->SetLength(m_ppCamera[nCnt]->GetLength() + 1.5f);

					// カメラを目標の向きまで回転させる
					D3DXVECTOR3 rotDest = START_CAMERAROT - m_ppCamera[nCnt]->GetRotation();
					m_ppCamera[nCnt]->SetRotation(m_ppCamera[nCnt]->GetRotation() + rotDest * 0.1f);
				}
			}
		}

			if (m_nStartCnt == START_WAITCNT - 30) {	// 規定値
				CGimmick::SwitchOff();

				if (m_pEnemy != nullptr) {
					m_pEnemy->Uninit();
					m_pEnemy = nullptr;
				}
			}
			else if (m_nStartCnt >= START_WAITCNT) {	// 規定値以上
				if (m_ppPlayer != nullptr) { // 使用していた場合
					for (int nCnt = 0; nCnt < m_nNumPlayer; nCnt++)
					{
						if (m_ppPlayer[nCnt] != nullptr) {
							m_ppPlayer[nCnt]->SetType(CPlayer::TYPE_ACTIVE);
						}
					}
				}
			}
	}

	return bDirection;
}