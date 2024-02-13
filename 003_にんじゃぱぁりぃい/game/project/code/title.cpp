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
#include "item.h"
#include "objectX.h"
#include "title_enter.h"
#include "meshdome.h"
#include "object3D.h"
#include "texture.h"
#include "enemy.h"

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
	// enterインスタンスを生成
	m_pEnter = new CTitleEnter;
	m_pEnter->SetPosition(D3DXVECTOR3(0.0f, 700.0f, -700.0f));
	m_pEnter->SetRotation(D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, D3DX_PI * 0.4f));

	//外部ファイル読み込みの生成
	if (m_pFileLoad == nullptr)
	{// 使用していない場合
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != nullptr)
		{
			m_pFileLoad->Init();
			m_pFileLoad->SetTitleEnter(m_pEnter);
			m_pFileLoad->OpenFile("data\\TXT\\title_model.txt");
		}
	}

	//タイトル生成
	CObject3D* pObj3D = CObject3D::Create(D3DXVECTOR3(-1000.0f, 900.0f, -600.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	pObj3D->SetRotation(D3DXVECTOR3(0.0f, -0.8f, 0.1f));
	pObj3D->SetSize(624.0f, 288.0f);
	pObj3D->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\title00.png"));

	//カメラ初期化
	{
		CManager::GetInstance()->GetCamera()->SetLength(2200.0f);
		CManager::GetInstance()->GetCamera()->SetPositionV(D3DXVECTOR3(387.0f, 793.0f, -2328.0f));
		CManager::GetInstance()->GetCamera()->SetPositionR(D3DXVECTOR3(-400.0f, 441.0f, -304.0f));
		CManager::GetInstance()->GetCamera()->SetRotation(D3DXVECTOR3(1.0f, -1.20f, 1.41f));

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

	//演出用敵生成
	m_appEnemy = new CEnemy*[ENEMY_NUM];
	for (int cnt = 0; cnt < ENEMY_NUM; cnt++)
	{
		m_appEnemy[cnt] = CEnemy::Create(D3DXVECTOR3(0.0f, 10.0f, -1600.0f -(150.0f * cnt)), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), NULL, NULL);
	}

	//ドーム追加
	CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 6000.0f, 6000.0f,3,8,8);

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

	if (m_pEnter != nullptr) 
	{
		m_pEnter->Uninit();
		delete m_pEnter;
		m_pEnter = nullptr;
	}

	if (m_appEnemy != nullptr)
	{
		delete m_appEnemy;
		m_appEnemy = nullptr;
	}
}

//===============================================
// 更新処理
//===============================================
void CTitle::Update(void)
{
	CInputPad *pInputPad = CManager::GetInstance()->GetInputPad();
	CInputKeyboard *pInputKey = CManager::GetInstance()->GetInputKeyboard();

	if (m_pEnter != nullptr) {
		m_pEnter->Update();
	}

	// 入力遷移
	if (pInputKey->GetTrigger(DIK_RETURN) || pInputPad->GetTrigger(CInputPad::BUTTON_START, 0))
	{
		CItem *pItem = CItem::GetTop();

		while (pItem != nullptr)
		{// 使用されていない状態まで

			CItem *pItemNext = pItem->GetNext();	// 次のオブジェクトへのポインタを取得

			D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			//移動量の設定
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.6f;
			move.y = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.3f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.6f;
			pItem->SetMove(move);

			//タイプの変更											
			pItem->SetState(CItem::STATE_CRASH);

			pItem = pItemNext;	// 次のオブジェクトに移動
		}

		if (m_bPush == false)
		{//1回だけ音鳴らす・移動量設定
			CManager::GetInstance()->GetSound()->Play(CSound::LABEL_SE_CLICK);

			if (m_appEnemy != nullptr) {
				for (int cnt = 0; cnt < ENEMY_NUM; cnt++)
				{
					m_appEnemy[cnt]->SetPointID(ExPattern::POINTID_TITLE);
				}
			}
		}

		m_bPush = true;		//ボタンを押した
	}

	if (m_bPush == true)
	{//スタートに対応するボタンが押された（チュートリアル画面遷移準備）
		m_nCounterTutorial++;		//チュートリアル遷移カウンター加算

		if (m_nCounterTutorial >= MOVE_TUTORIAL)
		{//チュートリアル画面遷移
			CManager::GetInstance()->GetFade()->Set(CScene::MODE_TUTORIAL);
		}
	}
	else
	{//スタートに対応するボタンが押されてない（ランキング遷移準備）
		m_nCounterRanking++;		//ランキング遷移カウンター加算

		//ランキング画面自動遷移
		if (m_nCounterRanking >= AUTOMOVE_RANKING)
		{//ランキング遷移
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
