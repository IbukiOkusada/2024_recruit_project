//===============================================
//
// マネージャーの処理 [manager.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "manager.h"
#include "renderer.h"
#include "object.h"
#include "object2D.h"
#include "player.h"
#include "input.h"
#include "debugproc.h"
#include "time.h"
#include "camera.h"
#include "light.h"
#include "object3D.h"
#include "objectX.h"
#include "Xfile.h"
#include "character.h"
#include "texture.h"
#include "meshfield.h"
#include "meshwall.h"
#include "meshcylinder.h"
#include "meshdome.h"
#include "fileload.h"
#include "edit.h"

//===============================================
// 静的メンバ変数
//===============================================
CRenderer *CManager::m_pRenderer = NULL;		// レンダラーのポインタ
CInputKeyboard *CManager::m_pInputKeyboard = NULL;	// 入力デバイス(キーボード)へのポインタ
CInputMouse *CManager::m_pInputMouse = NULL;	// 入力デバイス(マウス)のポインタ
CDebugProc *CManager::m_pDebugProc = NULL;		// デバッグ表示のポインタ
CPlayer *CManager::m_pPlayer = NULL;	// プレイヤーのポインタ
CCamera *CManager::m_pCamera = NULL;	// カメラのポインタ
CLight *CManager::m_pLight = NULL;		// ライトのポインタ
CTexture *CManager::m_pTexture = NULL;	// テクスチャのポインタ
CXFile *CManager::m_pModelFile = NULL;	// Xファイル情報のポインタ
CFileLoad *CManager::m_pFileLoad = NULL;	// ファイル読み込みのポインタ
CEdit *CManager::m_pEdit = NULL;		// エディットのポインタ

//===================================================
// コンストラクタ
//===================================================
CManager::CManager()
{
	
}

//===================================================
// デストラクタ
//===================================================
CManager::~CManager()
{

}

//===================================================
// 初期化処理
//===================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// レンダラーの生成
	if (m_pRenderer == NULL)
	{// 使用していない場合
		m_pRenderer = new CRenderer;
	}

	//初期化処理
	if (m_pRenderer != NULL)
	{// 使用している場合
		if (FAILED(m_pRenderer->Init(hWnd, TRUE)))
		{//初期化が失敗した場合
			return E_FAIL;
		}
	}

	// 入力デバイス(キーボード)の生成
	if (m_pInputKeyboard == NULL)
	{// 使用していない場合
		m_pInputKeyboard = new CInputKeyboard;

		//初期化処理
		if (m_pInputKeyboard != NULL)
		{// 使用している場合
			if (FAILED(m_pInputKeyboard->Init(hInstance, hWnd)))
			{//初期化が失敗した場合
				return E_FAIL;
			}
		}
	}

	// 入力デバイス(マウス)の生成
	if (m_pInputMouse == NULL)
	{// 使用していない場合
		m_pInputMouse = new CInputMouse;

		//初期化処理
		if (m_pInputMouse != NULL)
		{// 使用している場合
			if (FAILED(m_pInputMouse->Init(hInstance, hWnd)))
			{//初期化が失敗した場合
				return E_FAIL;
			}
		}
	}

	// デバッグ表示の生成
	if (m_pDebugProc == NULL)
	{// 使用していない場合
		m_pDebugProc = new CDebugProc;
	}

	//初期化処理
	if (m_pDebugProc != NULL)
	{// 使用している場合
		m_pDebugProc->Init();
	}

	// カメラの生成
	if (m_pCamera == NULL)
	{// 使用していない場合
		m_pCamera = new CCamera;

		// 初期化
		if (m_pCamera != NULL)
		{
			m_pCamera->Init();
		}
	}

	// ライトの生成
	if (m_pLight == NULL)
	{// 使用していない場合
		m_pLight = new CLight;

		// 初期化
		if (m_pLight != NULL)
		{
			m_pLight->Init();
		}
	}

	//テクスチャの生成
	if (m_pTexture == NULL)
	{// 使用していない場合
		m_pTexture = new CTexture;

		// 初期読み込み
		if (m_pTexture != NULL)
		{
			m_pTexture->Load();
		}
	}

	// Xファイル情報の生成
	if (m_pModelFile == NULL)
	{// 使用していない場合
		m_pModelFile = new CXFile;
	}

	// 外部ファイル読み込みの生成
	if (m_pFileLoad == NULL)
	{// 使用していない場合
		m_pFileLoad = new CFileLoad;

		if (m_pFileLoad != NULL)
		{
			m_pFileLoad->Init();
			m_pFileLoad->OpenFile("data\\TXT\\system.txt");
		}
	}

	// エディットの生成
	if (m_pEdit == NULL)
	{// 使用していない場合
		m_pEdit = new CEdit;

		if (m_pEdit != NULL)
		{
			m_pEdit->Init();
		}
	}

	char aBodyScr[128] = {};
	char aLegSct[128] = {};

	// systemファイル読み込み
	SystemLoad(&aBodyScr[0], &aLegSct[0]);

	// オブジェクト生成
	CMeshDome::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5000.0f, 10.0f, 3, 10, 10);
	CMeshCylinder::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), 3000.0f, 50.0f, 3, 10, 4);

	m_pPlayer = CPlayer::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		&aBodyScr[0], &aLegSct[0], 1);

	m_pEdit->SetBody(m_pPlayer->GetBody()); 
	m_pEdit->SetInfo(m_pPlayer->GetBody()->GetMotion()->GetKeyInfo(0, 0));

	return S_OK;
}

//===================================================
// 終了処理
//===================================================
void CManager::Uninit(void)
{

	if (m_pInputKeyboard != NULL)
	{// 使用している場合
		// 終了処理
		m_pInputKeyboard->Uninit();

		delete m_pInputKeyboard;	// メモリの開放

		m_pInputKeyboard = NULL;	// 使用していない状態にする
	}

	if (m_pDebugProc != NULL)
	{// 使用している場合
		// 終了処理
		m_pDebugProc->Uninit();

		delete m_pDebugProc;	// メモリの開放

		m_pDebugProc = NULL;	// 使用していない状態にする
	}

	if (m_pRenderer != NULL)
	{// 使用している場合
		// 終了処理
		m_pRenderer->Uninit();

		delete m_pRenderer;	// メモリの開放

		m_pRenderer = NULL;	// 使用していない状態にする
	}

	// テクスチャの廃棄
	if (m_pTexture != NULL)
	{// 使用している場合
		// 終了処理
		m_pTexture->Unload();

		delete m_pTexture;	// メモリの開放

		m_pTexture = NULL;	// 使用していない状態にする
	}

	// Xファイル情報の廃棄
	if (m_pModelFile != NULL)
	{// 使用している場合
		// 終了処理
		m_pModelFile->Unload();

		delete m_pModelFile;	// メモリの開放

		m_pModelFile = NULL;	// 使用していない状態にする
	}

	// ファイル読み込みの廃棄
	if (m_pFileLoad != NULL)
	{
		m_pFileLoad->Uninit();

		delete m_pFileLoad;		// メモリの開放
		m_pFileLoad = NULL;
	}

	// エディットの廃棄
	if (m_pEdit != NULL)
	{
		m_pEdit->Uninit();

		delete m_pEdit;		// メモリの開放
		m_pEdit = NULL;
	}
}

//===================================================
// 更新処理
//===================================================
void CManager::Update(void)
{
	// デバッグ表示の更新処理
	if (m_pDebugProc != NULL)
	{// 使用している場合
		m_pDebugProc->Update();
	}

	// キーボードの更新処理
	if (m_pInputKeyboard != NULL)
	{// 使用している場合
		m_pInputKeyboard->Update();
	}

	// マウスの更新処理
	if (m_pInputMouse != NULL)
	{// 使用している場合
		m_pInputMouse->Update();
	}


	// カメラの更新処理
	if (m_pCamera != NULL)
	{
		m_pCamera->Update();
	}

	// レンダラーの更新処理
	if (m_pRenderer != NULL)
	{// 使用している場合
		m_pRenderer->Update();
	}

	// エディットの更新
	if (m_pEdit != NULL)
	{
		m_pEdit->Update();
	}

	if (m_pInputKeyboard != NULL)
	{// 使用している場合
		if (CObject::GetItemNumAll() <= 0)
		{// 一つも配置されていない場合
			m_pDebugProc->Print("敵再配置[F3]\n");
			if (m_pInputKeyboard->GetTrigger(DIK_F3) == true)
			{
				ItemReset();
			}
		}
	}
}

//===================================================
// 描画処理
//===================================================
void CManager::Draw(void)
{
	// 描画処理
	if (m_pRenderer != NULL)
	{// 使用している場合
		m_pRenderer->Draw();
	}
}

//===================================================
// レンダラーの取得
//===================================================
CRenderer *CManager::GetRenderer(void)
{
	return m_pRenderer;
}

//===================================================
// 入力デバイスの取得(キーボード)
//===================================================
CInputKeyboard *CManager::GetInputKeyboard(void)
{
	return m_pInputKeyboard;
}

//===================================================
// 入力デバイスの取得(マウス)
//===================================================
CInputMouse *CManager::GetInputMouse(void)
{
	return m_pInputMouse;
}

//===================================================
// デバッグ表示の取得
//===================================================
CDebugProc *CManager::GetDebugProc(void)
{
	return m_pDebugProc;
}

//===================================================
// プレイヤーの取得
//===================================================
CPlayer *CManager::GetPlayer(void)
{
	return m_pPlayer;
}

//===================================================
// アイテムの再配置
//===================================================
void CManager::ItemReset(void)
{
	CObjectX::Create(D3DXVECTOR3(800.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pFileLoad->GetModelFileName(0));
	CObjectX::Create(D3DXVECTOR3(800.0f, 0.0f, 300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pFileLoad->GetModelFileName(1));
	CObjectX::Create(D3DXVECTOR3(800.0f, 0.0f, -300.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), m_pFileLoad->GetModelFileName(2));
}

//===================================================
// カメラの取得
//===================================================
CCamera *CManager::GetCamera(void)
{
	return m_pCamera;
}

//===================================================
// ライトの取得
//===================================================
CLight *CManager::GetLight(void)
{
	return m_pLight;
}

//===================================================
// テクスチャの取得
//===================================================
CTexture *CManager::GetTexture(void)
{
	return m_pTexture;
}

//===================================================
// Xファイル情報の取得
//===================================================
CXFile *CManager::GetModelFile(void)
{
	return m_pModelFile;
}

//===================================================
// ファイル読み込みの取得
//===================================================
CFileLoad *CManager::GetFileLoad(void)
{
	return m_pFileLoad;
}

//===================================================
// エディットの取得
//===================================================
CEdit *CManager::GetEdit(void)
{
	return m_pEdit;
}

//===================================================
// エディットの取得
//===================================================
void CManager::SystemLoad(char *pBody, char *pLeg)
{
	FILE *pFile;
	char aStr[128] = {};

	// ファイルを開く
	pFile = fopen("data\\system.ini", "r");

	if (pFile == NULL)
	{// 開けなかった場合
		return;
	}

	while (1)
	{
		//テキスト読み込み
		int nResult = fscanf(pFile, "%s", &aStr[0]);

		if (strcmp(&aStr[0], "SCRIPT_BODYFILENAME") == 0)
		{//胴体ファイル確認文字が確認できた場合
			fscanf(pFile, "%s", &aStr[0]);	// (=)
			fscanf(pFile, "%s", pBody);	// 胴体ファイル名読み込み
		}
		else if (strcmp(&aStr[0], "SCRIPT_LEGFILENAME") == 0)
		{//下半身ファイル確認文字が確認できた場合
			fscanf(pFile, "%s", &aStr[0]);	// (=)
			fscanf(pFile, "%s", pLeg);	// 胴体ファイル名読み込み
		}
		else if (nResult == EOF)
		{//ファイルの最後まで読み込んでしまった場合
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);
}