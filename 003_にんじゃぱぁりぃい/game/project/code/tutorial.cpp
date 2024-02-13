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
#include "goal.h"
#include "gimmick_rotatedoor.h"
#include "gimmick_startdoor.h"
#include "gimmick_lever.h"
#include "gimmick_multidoor.h"
#include "gimmick_button.h"
#include "gimmick_spear.h"
#include "gimmick_pull.h"
#include "gimmick_startdoor.h"
#include "gimmick_lever.h"
#include "object3D.h"
#include "enemy.h"
#include "item.h"
#include "particle.h"
#include "entryicon.h"
#include "sound.h"

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

	// 回転扉
	CGimmickRotateDoor::Create(D3DXVECTOR3(-500.0f, 0.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));

	// ゴール
	CGoal::Create(D3DXVECTOR3(-1350.0f, 0.0f, -1000.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 200.0f);

	// 壺
	CGimmickPull::Create(D3DXVECTOR3(-400.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CGimmickPull::Create(D3DXVECTOR3(-200.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	CGimmickPull::Create(D3DXVECTOR3(0.0f, 0.0f, -800.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 地面からの槍
	CGimmickButton *pButton = CGimmickButton::Create(D3DXVECTOR3(-650.0f, 0.0f, -500.0f));
	CGimmickSpear *pSpear = CGimmickSpear::Create(D3DXVECTOR3(-650.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CGimmickSpear::TYPE_PRESSAUTO);
	pSpear->BindButton(pButton);

	CGimmickLever *l = CGimmickLever::Create(D3DXVECTOR3(-700.0f, 50.0f, -900.0f));
	l->SetRotation(D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	CGimmickStartDoor *p = CGimmickStartDoor::Create(D3DXVECTOR3(-1400.0f, 0.0f, -1000.0f));
	p->SetRotation(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	p->SetLever(l);

	// 人数分ポインタ生成
	m_ppPlayer = new CPlayer*[PLAYER_MAX];

	m_ppPlayer[0] = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		"data\\TXT\\player0\\motion_ninjabody.txt", "data\\TXT\\player0\\motion_ninjaleg.txt");
	m_ppPlayer[0]->BindId(0);
	m_ppPlayer[0]->SetType(CPlayer::TYPE_ACTIVE);

	for (int i = 0; i < NUM_ENEMY; i++)
	{
		if (m_apEnemy[i] == nullptr)
		{// 使用されていない場合

			m_apEnemy[i] = CEnemy::Create(D3DXVECTOR3(200.0f - i * 500.0f, 0.0f, 350.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL, NULL);
		}
	}

	// エントリーアイコン
	{
		for (int i = 0; i < NUM_PLAYER; i++)
		{
			if (m_apObject[i] == nullptr)
			{// 使用されていなかったら

				m_apObject[i] = CEntryIcon::Create(D3DXVECTOR3(190.0f + i * 300.0f, 570.0f, 0.0f), i, 125.0f, 75.0f);
			}
		}
	}

	CObject2D *pObject2D = CObject2D::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 680.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 7);
	pObject2D->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\entrystart.png"));
	pObject2D->SetSize(PORISIZE.x, PORISIZE.y);
	
	// 0番目だけエントリーしている状態にする
	if (m_apObject[0] != nullptr)
	{
		m_apObject[0]->SetbEntry(true);
		m_apObject[0]->SetState(CEntryIcon::STATE_ENTRY);
		m_apObject[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_apObject[0]->Entryed();
	}

	CItem::Create(D3DXVECTOR3(-900.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_BRECELET, NULL);
	CItem::Create(D3DXVECTOR3(-900.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_CUP, NULL);
	CItem::Create(D3DXVECTOR3(-900.0f, 0.0f, -100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_GEM00, NULL);
	CItem::Create(D3DXVECTOR3(-900.0f, 0.0f, 100.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_GEM01, NULL);
	CItem::Create(D3DXVECTOR3(-900.0f, 0.0f, 200.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_GOLDBAR, NULL);
	CItem::Create(D3DXVECTOR3(-1300.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_JAR, NULL);
	CItem::Create(D3DXVECTOR3(-1300.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_KUNAI, NULL);
	CItem::Create(D3DXVECTOR3(-1300.0f, 0.0f, -400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_RING00, NULL);
	CItem::Create(D3DXVECTOR3(-1300.0f, 0.0f, 400.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_SCROLL, NULL);
	CItem::Create(D3DXVECTOR3(-1300.0f, 0.0f, 500.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), CItem::TYPE_SHURIKEN, NULL);

	// 基本操作
	if (m_pObject3D[0] == nullptr)
	{
		m_pObject3D[0] = CObject3D::Create(D3DXVECTOR3(600.0f, 10.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pObject3D[0]->SetRotation(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
		m_pObject3D[0]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pObject3D[0]->SetSize(100.0f, 100.0f);
		m_pObject3D[0]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\tutorial000.png"));
	}
	
	// ギミック
	if (m_pObject3D[1] == nullptr)
	{
		m_pObject3D[1] = CObject3D::Create(D3DXVECTOR3(-1300.0f, 10.0f, -550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pObject3D[1]->SetRotation(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
		m_pObject3D[1]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pObject3D[1]->SetSize(100.0f, 100.0f);
		m_pObject3D[1]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\tutorial004.png"));
	}

	// 忍術関連
	if (m_pObject3D[2] == nullptr)
	{
		m_pObject3D[2] = CObject3D::Create(D3DXVECTOR3(600.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pObject3D[2]->SetRotation(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
		m_pObject3D[2]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pObject3D[2]->SetSize(100.0f, 100.0f);
		m_pObject3D[2]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\tutorial003.png"));
	}

	// お宝関連
	if (m_pObject3D[3] == nullptr)
	{
		m_pObject3D[3] = CObject3D::Create(D3DXVECTOR3(-1200.0f, 10.0f, -50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pObject3D[3]->SetRotation(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
		m_pObject3D[3]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pObject3D[3]->SetSize(100.0f, 100.0f);
		m_pObject3D[3]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\tutorial005.png"));
	}

	//	回転扉
	if (m_pObject3D[4] == nullptr)
	{
		m_pObject3D[4] = CObject3D::Create(D3DXVECTOR3(-300.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pObject3D[4]->SetRotation(D3DXVECTOR3(D3DX_PI * 0.5f, D3DX_PI, 0.0f));
		m_pObject3D[4]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pObject3D[4]->SetSize(100.0f, 100.0f);
		m_pObject3D[4]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\tutorial002.png"));
	}

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

	for (int i = 0; i < NUM_PLAYER; i++)
	{
		if (m_apObject[i] != nullptr)
		{
			m_apObject[i]->Uninit();
			m_apObject[i] = nullptr;
		}
	}

	if (m_ppPlayer != NULL)
	{// 使用していた場合
		int nNum = CPlayer::GetNum();
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
		CGame::SetNumPlayer(CPlayer::GetNum());

	}

	bool bCreate = false;

	if (CPlayer::GetNum() < PLAYER_MAX){ // 人数が最大ではない場合
		if (CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_START, CPlayer::GetNum())) {
			int nId = CPlayer::GetNum();
			char aBodyPass[FILEPASS_SIZE] = "";		// 胴体パス
			char aLegPass[FILEPASS_SIZE] = "";		// 下半身パス

			sprintf(&aBodyPass[0], "%s%d\\motion_ninjabody%s",FILEPASS, nId, FILEEXT);
			sprintf(&aLegPass[0], "%s%d\\motion_ninjaleg%s", FILEPASS, nId, FILEEXT);

			m_ppPlayer[nId] = CPlayer::Create(D3DXVECTOR3(0.0f, 10.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
				D3DXVECTOR3(0.0f, 0.0f, 0.0f), &aBodyPass[0], &aLegPass[0]);
			m_ppPlayer[nId]->BindId(nId);
			m_ppPlayer[nId]->SetType(CPlayer::TYPE_ACTIVE);
			// 煙のパーティクル生成
			CParticle::Create(D3DXVECTOR3(0.0f, 10.0f, 0.0f), CEffect::TYPE_SMAKE);
			bCreate = true;
			m_apObject[nId]->SetbEntry(true);
			m_apObject[nId]->SetState(CEntryIcon::STATE_ENTRY);
			m_apObject[nId]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			m_apObject[nId]->Entryed();

			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_ENTRY);
		}
	}

	if (CPlayer::GetNum() - 1 > 0 && !bCreate) { // 人数が最大ではない場合
		if (CManager::GetInstance()->GetInputPad()->GetTrigger(CInputPad::BUTTON_START, CPlayer::GetNum() - 1)) {
			int nId = CPlayer::GetNum() - 1;
			m_ppPlayer[nId]->Uninit();
			m_ppPlayer[nId] = 0;
			m_apObject[nId]->SetbEntry(false);
			m_apObject[nId]->SetState(CEntryIcon::STATE_STANDBY);
			m_apObject[nId]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, m_apObject[nId]->GetCol()));
			m_apObject[nId]->NoEntry();

			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_ENTRY);
		}
	}

	for (int i = 0; i < NUM_PORI; i++)
	{
		if (m_pObject3D[i] != nullptr)
		{// 使用されている場合

			// 近づくと大きくなる
			m_pObject3D[i]->ZoomSize(m_ppPlayer, 100.0f);
		}
	}

	if (CEnemy::GetNum() <= 0)
	{// 敵が全員死んだら再生成

		for (int i = 0; i < NUM_ENEMY; i++)
		{
			// 使用していない状態にする
			m_apEnemy[i] = nullptr;

			if (m_apEnemy[i] == nullptr)
			{// 使用していなかったら
				
				m_apEnemy[i] = CEnemy::Create(D3DXVECTOR3(200.0f - i * 500.0f, 0.0f, 550.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL, NULL);
			}
		}
	}

	if (EndCheck()) 
	{// 全員ゴールしている

		// ゲームに遷移
		CManager::GetInstance()->GetFade()->Set(CScene::MODE_GAME);
		CGame::SetNumPlayer(CPlayer::GetNum());
	}

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

//===================================================
// ファイル読み込みの取得
//===================================================
bool CTutorial::EndCheck(void)
{
	CPlayer *pPl = CPlayer::GetTop();	// プレイヤー
	int nNumGoal = 0;

	// ゴールしている人数を判定
	while (pPl != nullptr) {

		CPlayer *pPlNext = pPl->GetNext();	// 次を覚える

		if (!pPl->GetGoal()) {	// ゴールしていない
			break;
		}

		nNumGoal++;
		pPl = pPlNext;	// 次に移動
	}

	if (nNumGoal >= CPlayer::GetNum()) {	// 全員ゴール
		return true;
	}

	return false;
}
