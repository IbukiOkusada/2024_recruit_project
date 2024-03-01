//===============================================
//
// リザルト画面の管理処理 [result.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "result.h"
#include "object2D.h"
#include "texture.h"
#include "manager.h"
#include "main.h"
#include "input.h"
#include "objectX.h"
#include "fade.h"
#include "ranking.h"
#include "sound.h"
#include "result.h"
#include "fileload.h"
#include "meshdome.h"
#include "character.h"
#include "motion.h"
#include "camera.h"
#include "player.h"
#include "ranking.h"
#include "object_manager.h"
#include "particle.h"
#include "player_manager.h"
#include "time.h"

// マクロ定義
#define RANKING_FILE	"data\\FILE\\ranking.bin"	// ランキングファイル
#define MOVE_TIMER	(660)

// 無名名前空間
namespace {
	const float PLAYER_MOVESIZE = (100.0f);	// プレイヤーの横移動サイズ
	const float PLAYER_SPACE = (200.0f);	// プレイヤー同士の間の距離
	const float PLAYER_POSY = (2000.0f);	// プレイヤー配置基本Y座標
	const float PLAYER_RANK_POSY = (-350.0f);	// プレイヤーの順位ごとに上げるY座標
	const float PLAYER_UPPOSY = (300.0f);	// 最大人数によって変わるプレイヤーの設置Y座標
	const float PLAYER_RANKMOVEPOS_Y = (900.0f);	// ランクとスコアがついてくる確認Y座標
	const float CAMERA_MOVESTARTPOSY = (1400.0f);	// カメラ動き始めY座標
	const float PLAYER_GRAVITY = (-15.0f);	// プレイヤーの落下速度
	const float RANK_DOWNSPEED = (20.0f);	// ランクとスコアの落下速度
	const D3DXVECTOR3 TOTALSCORE_POS = {SCREEN_WIDTH * 0.375f, SCREEN_HEIGHT * 0.9f, 0.0f};	// 合計スコアの設置座標
	const D3DXVECTOR3 SCORE_POS = {SCREEN_WIDTH * 0.43f, -SCREEN_HEIGHT * 1.4f, 0.0f};		// 個人スコアの設置座標
	const float SCORE_MOVESIZE = (130.0f);	// スコアの配置移動サイズ
	const float SCORE_SPACE = (270.0f);		// スコア間の空間
	const float TOTALSCORE_SPACE = (1.0f);	// 合計スコアの数字間
	const float SCORE_NUMSPACE = (1.0f);
	const D3DXVECTOR3 RANK_POS = { SCREEN_WIDTH * 0.5f, -SCREEN_HEIGHT * 1.3f, 0.0f };		// 合計スコアの設置座標
	const float RANK_MOVESIZE = (135.0f);	// ランクの配置移動サイズ
	const float RANK_SPACE = (270.0f);		// ランク間の空間
	const D3DXVECTOR2 RANK_SIZE = { 50.0f, 30.0f };	// ランクポリゴンサイズ
	const D3DXVECTOR3 CLEAR_SETPOS = { -SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.9f, 0.0f};	// clearの配置座標
	const D3DXVECTOR2 CLEAR_SIZE = {500.0f, 100.0f };	// クリアのサイズ
	const D3DXVECTOR3 CLEAR_POS = { SCREEN_WIDTH * 0.35f, SCREEN_HEIGHT * 0.9f, 0.0f };	// clearの配置座標
	const float SCORE_MOVEX = (SCREEN_WIDTH * 0.2f);
	const float FAILED_ROTATE = (D3DX_PI * 0.125f);
	const float CLEAR_MOVE = (20.0f);
	const float CLEAR_ROTMOVEZ = (D3DX_PI * 0.015f);
	const float FAILED_UPY = (-2.5f);
	const float FAILED_NOBINOBISPEED = (0.025f);
	const float FAILED_NOBINOBISIZE = (10.0f);
	const float CLEAR_NOBINOBISPEED = (0.075f);	// クリアした際の伸び縮みする速度
	const float CLEAR_NOBINOBISIZE = (50.0f);	// 伸びる倍率
	const int PLAYER_MAXMOTION = (13);			// プレイヤーの最大モーション数
	const int SCORE_MINTORURURURU = (60);		// とぅるとぅるの差分最低値
	const int SCORE_TORURURUSPEED = (13);		// とぅるとぅるの差分補正速度
	const D3DXCOLOR CLEAR_COL = { 0.0f, 0.0f, 1.0f, 1.0f };
	const D3DXCOLOR FAILED_COL = { 1.0f, 0.0f, 0.0f, 1.0f };
}

// 静的メンバ変数
int CResult::m_nScore = 0;

//===============================================
// コンストラクタ
//===============================================
CResult::CResult()
{
	m_pMeshSky = NULL;
	m_pTime = NULL;
	m_pObjClear = nullptr;
}

//===============================================
// デストラクタ
//===============================================
CResult::~CResult()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CResult::Init(void)
{
	CMeshDome::Create(D3DXVECTOR3(0.0f, -1000.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10000.0f, 10.0f, 3, 10, 10);

	if (m_pFileLoad == NULL)
	{// 使用していない場合
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != NULL)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\result.txt");
		}
	}

	//カメラ初期化
	{
		CManager::GetInstance()->GetCamera()->SetLength(300.0f);
		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(1.0f, -D3DX_PI * 0.5f, 2.63f));
		CManager::GetInstance()->GetCamera()->SetPositionR(D3DXVECTOR3(0.0f, 137.77f, -301.94f));
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

	CManager::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_RESULT);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CResult::Uninit(void)
{
	if (m_pTime != nullptr) {
		m_pTime->Uninit();
		delete m_pTime;
		m_pTime = nullptr;
	}
	if (m_pFileLoad != nullptr)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// メモリの開放
		m_pFileLoad = nullptr;
	}

	CRanking::SetScore(m_nScore);
	m_nScore = 0;
	CManager::GetInstance()->GetCamera()->SetActive(true);

	CManager::GetInstance()->GetSound()->Stop();
}

//===============================================
// 更新処理
//===============================================
void CResult::Update(void)
{

	if (CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_A, 0) ||
		CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_START, 0))
	{
		CManager::GetInstance()->GetFade()->Set(CScene::MODE_RANKING);
	}

	CScene::Update();
}

//===============================================
// 描画処理
//===============================================
void CResult::Draw(void)
{
	CScene::Draw();
}
