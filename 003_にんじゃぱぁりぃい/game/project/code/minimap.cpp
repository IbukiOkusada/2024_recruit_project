//===============================================
//
// ミニマップの処理 [minimap.cpp]
// Author : Soma Ishihara
//
//===============================================
#include "minimap.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "object_manager.h"
#include "camera.h"
#include "player.h"
#include "object2D.h"
#include "object2DMap.h"
#include "object2DScroll.h"
#include "gimmick.h"
#include <assert.h>

//===================================================
// コンストラクタ
//===================================================
CMiniMap::CMiniMap()
{
	m_pObjMap = nullptr;
	m_pTextureMap = nullptr;
	m_pTextureUnex = nullptr;
	m_pZSurface = nullptr;
	m_ppPlayerIcon = nullptr;
	m_ppExplored = nullptr;
	m_pExitIcon = nullptr;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fWidth = 0.0f;
	m_fHeight = 0.0f;
	m_nElaseWidth = 0;
	m_nElaseHeight = 0;
	m_nPlayerNum = 0;
}

//===================================================
// デストラクタ
//===================================================
CMiniMap::~CMiniMap()
{
}

//===============================================
// 初期化処理
//===============================================
HRESULT CMiniMap::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスへのポインタ
	CTexture* pTexture = CManager::GetInstance()->GetTexture();

	//探索済みドット用フラグ生成
	if (m_ppExplored == nullptr)
	{
		int x = static_cast<int>(m_fWidth);
		int y = static_cast<int>(m_fHeight);

		m_ppExplored = new bool*[x];
		for (int cntX = 0; cntX < x; cntX++)
		{
			m_ppExplored[cntX] = new bool[y];
			for (int cntY = 0; cntY < y; cntY++)
			{
				m_ppExplored[cntX][cntY] = false;
			}
		}
	}

	//プレイヤー人数分アイコン用2Dオブジェ生成
	if (m_nPlayerNum > 0)
	{//1人以上いる
		m_ppPlayerIcon = new CObject2D*[m_nPlayerNum];
		for (int cnt = 0; cnt < m_nPlayerNum; cnt++)
		{
			char path[128];
			sprintf(&path[0], "data\\TEXTURE\\player_icon%d.png", cnt);

			m_ppPlayerIcon[cnt] = CObject2D::Create(4);
			m_ppPlayerIcon[cnt]->BindTexture(pTexture->Regist(&path[0]));
			m_ppPlayerIcon[cnt]->SetLength(12.0f, 12.0f);
			m_ppPlayerIcon[cnt]->SetVtx();
		}
	}
	else
	{//不正
		assert(false);
	}

	if (m_pExitIcon == nullptr)
	{
		m_pExitIcon = CObject2D::Create(4);
		m_pExitIcon->BindTexture(pTexture->Regist("data\\TEXTURE\\exit.png"));
		m_pExitIcon->SetLength(24.0f, 24.0f);
		m_pExitIcon->SetVtx();
	}

	// テクスチャの生成
	Load();
	Reset();

	//マップオブジェ生成
	if (m_pObjScroll == nullptr)
	{
 		m_pObjScroll = CObject2DScroll::Create(m_pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_pObjScroll->SetSize((m_fHeight * 0.8f) * 2.0f, (m_fHeight * 0.8f));
		m_pObjScroll->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\scroll_minimap.png"));
	}
	
	if (m_pObjMap == nullptr)
	{
		m_pObjMap = CObject2DMap::Create(m_pos, D3DXVECTOR3(0.0f,0.0f,0.0f), m_fWidth * 0.5f, m_fHeight * 0.5f);
		m_pObjMap->SetTexture(m_pTextureMap, m_pTextureUnex);
	}

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CMiniMap::Uninit(void)
{
	//アイコンオブジェ破棄
	if (m_ppPlayerIcon != nullptr)
	{
		for (int cnt = 0; cnt < m_nPlayerNum; cnt++)
		{
			m_ppPlayerIcon[cnt]->Uninit();
		}
		//入れ物破棄
		delete[] m_ppPlayerIcon;
	}

	//探索済みドット用フラグ破棄
	if (m_ppExplored != nullptr)
	{
		int x = static_cast<int>(m_fWidth);
		int y = static_cast<int>(m_fHeight);

		for (int cntX = 0; cntX < x; cntX++)
		{//bool*の部分破棄
			delete[] m_ppExplored[cntX];
		}

		//2次元配列みたいなポインタ破棄
		delete[] m_ppExplored;
	}

	//マップオブジェ破棄
	if (m_pObjMap != nullptr)
	{
		m_pObjMap->Uninit();
		m_pObjMap = nullptr;
	}

	// テクスチャ破棄
	UnLoad();

	//タスク破棄
	Release();
}

//===============================================
// テクスチャ描画処理
//===============================================
void CMiniMap::DrawTexture(void)
{
	//デバイス
	CManager* pManager = CManager::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();		//デバイスへのポインタ
	CScene* pScene = pManager->GetScene();

	//バックバッファ用
	LPDIRECT3DSURFACE9 pOrgSurface;
	LPDIRECT3DSURFACE9 pOrgZBuffer;

	LPDIRECT3DSURFACE9 pTexSurface;

	//int型サイズ
	int nWidth = static_cast<int>(m_fWidth);
	int nHeight = static_cast<int>(m_fHeight);

	//専用カメラ
	D3DXVECTOR3 posR = D3DXVECTOR3(0.0f, 1.0f, -1700.0f);
	D3DXVECTOR3 posV = D3DXVECTOR3(-50.0f, 3500.0f, -1700.0f);
	D3DXVECTOR3 vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//プロジェクションマトリの初期化
	D3DXMatrixIdentity(&m_mtxProj);

	D3DXMatrixPerspectiveFovLH(&m_mtxProj,
		D3DXToRadian(45.0f),
		(float)m_fWidth / (float)m_fHeight,
		10.0f,
		40000.0f);

	//プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProj);

	//ビューマトリの初期化
	D3DXMatrixIdentity(&m_mtxView);

	//ビューマトリの作成
	D3DXMatrixLookAtLH(&m_mtxView,
		&posV,
		&posR,
		&vecU);

	//ビューマトリの設定
	pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	//ビューポートマトリ設定
	D3DXMatrixIdentity(&m_mtxViewPort);
	m_mtxViewPort._11 = m_fWidth * 0.5f;
	m_mtxViewPort._22 = -m_fHeight * 0.5f;
	m_mtxViewPort._41 = m_fWidth * 0.5f;
	m_mtxViewPort._42 = m_fHeight * 0.5f;

	//テクスチャ
	//作成テクスチャ用インターフェース取得
	m_pTextureMap->GetSurfaceLevel(0, &pTexSurface);

	//バックバッファインターフェース取得
	pDevice->GetRenderTarget(0, &pOrgSurface);
	pDevice->GetDepthStencilSurface(&pOrgZBuffer);

	//レンダリングターゲットをテクスチャに変更
	pDevice->SetRenderTarget(0, pTexSurface);
	pDevice->SetDepthStencilSurface(m_pZSurface);

	//テクスチャクリア
	pDevice->Clear(
		0,
		NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0),
		1.0f,
		0);

	//テクスチャに描画
	if (SUCCEEDED(pDevice->BeginScene()))
	{// 描画が成功した場合

		// オブジェクトの描画
		CObjectManager::GetInstance()->DrawOneDimension(CObjectManager::TYPE_3D);

		// 描画終了
		pDevice->EndScene();
	}

	//戻す
	pDevice->SetRenderTarget(0, pOrgSurface);
	pDevice->SetDepthStencilSurface(pOrgZBuffer);

	pOrgSurface->Release();
	pOrgZBuffer->Release();
	pTexSurface->Release();

	//出口描画
	SetExit();
}

//===============================================
// 3人プレイ用単体描画
//===============================================
void CMiniMap::DrawManual(void)
{
	CManager::GetInstance()->GetCamera()->SetCamera();

	// オブジェクトの描画
	if (m_pObjScroll != nullptr)
	{
		m_pObjScroll->Draw();
	}

	if (m_pObjMap != nullptr)
	{
		m_pObjMap->Draw();
	}

	for (int cnt = 0; cnt < m_nPlayerNum; cnt++)
	{
		if (m_ppPlayerIcon[cnt] != nullptr)
		{
			m_ppPlayerIcon[cnt]->Draw();
		}
	}

	if (m_pExitIcon != nullptr && m_pExitIcon->GetDraw() == true)
	{
		m_pExitIcon->Draw();
	}
}

//===============================================
// 探索済みの場所のまっくろくろすけを消してプレイヤーを表示
//===============================================
void CMiniMap::ExploredMap(void)
{
	//デバイス
	CManager* pManager = CManager::GetInstance();
	LPDIRECT3DDEVICE9 pDevice = pManager->GetRenderer()->GetDevice();		//デバイスへのポインタ
	CScene* pScene = pManager->GetScene();
	D3DXMATRIX mtx;

	//int型サイズ
	int nWidth = static_cast<int>(m_fWidth);
	int nHeight = static_cast<int>(m_fHeight);

	//探索済みエリアの透明化（テクスチャが）
	//必要なマトリ計算
	mtx = m_mtxView * m_mtxProj * m_mtxViewPort;	//内部でD3DXMatrixMultiplyやってるみたい

	//プレイヤーすべて見る
	int nPlaceIcon = 0;
	CPlayer* pPlayer = CPlayer::GetTop();
	while (pPlayer != nullptr)
	{
		//プレイヤーごとに必要な変数
		D3DXVECTOR3 posPlayer = pPlayer->GetPosition();
		D3DXVECTOR3 posScreen;

		//これでスクリーン座標に変換できた
		D3DXVec3TransformCoord(&posScreen, &posPlayer, &mtx);
		int posX = (int)posScreen.x;
		int posY = (int)posScreen.y;
		int posElaseMinX = ((posX - m_nElaseWidth) > 0) ? posX - m_nElaseWidth : 0;
		int posElaseMaxX = ((posX + m_nElaseWidth) < nWidth) ? posX + m_nElaseWidth : nWidth;
		int posElaseMinY = ((posY - m_nElaseHeight) > 0) ? posY - m_nElaseHeight : 0;
		int posElaseMaxY = ((posY + m_nElaseHeight) < nHeight) ? posY + m_nElaseHeight : nHeight;

		
		if (posScreen.x >= 0.0f && posScreen.x <= m_fWidth &&
			posScreen.y >= 0.0f && posScreen.y <= m_fHeight)
		{//マップの範囲内なら探索済み状態化とアイコン描画
			//一定範囲消す
			for (int y = posElaseMinY; y < posElaseMaxY; y++)
			{
				for (int x = posElaseMinX; x < posElaseMaxX; x++)
				{
					m_ppExplored[x][y] = true;
				}
			}

			//アイコン置く（脱出後の挙動により変更予定）
			if (nPlaceIcon < m_nPlayerNum)
			{
				D3DXVECTOR3 posIcon = m_pos;
				posIcon.x += -m_fWidth * 0.5f + static_cast<float>(posX);
				posIcon.y += -m_fHeight * 0.5f + static_cast<float>(posY);
				m_ppPlayerIcon[nPlaceIcon]->SetPosition(posIcon);
				m_ppPlayerIcon[nPlaceIcon]->SetVtx();
				m_ppPlayerIcon[nPlaceIcon]->SetDraw(true);
			}
		}
		else
		{
			if (nPlaceIcon < m_nPlayerNum)
			{
				m_ppPlayerIcon[nPlaceIcon]->SetDraw(false);
			}
		}

		//次
		nPlaceIcon++;
		pPlayer = pPlayer->GetNext();
	}

	D3DLOCKED_RECT lockrect;
	m_pTextureUnex->LockRect(0, &lockrect, nullptr, 0);
	BYTE* pBitByte = (BYTE*)lockrect.pBits;
	for (int y = 0; y < nHeight; y++)
	{
		DWORD* pBitColor = (DWORD*)(pBitByte + y * lockrect.Pitch);
		for (int x = 0; x < nWidth; x++)
		{
			if (m_ppExplored[x][y] == true)
			{
				pBitColor[x] = 0x00000000;
			}
			else
			{
				pBitColor[x] = 0xff000000;
			}
		}
	}
	m_pTextureUnex->UnlockRect(0);
}

//===============================================
// テクスチャ生成処理
//===============================================
void CMiniMap::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();	//デバイスへのポインタ

	//int型サイズ
	int nWidth = static_cast<int>(m_fWidth);
	int nHeight = static_cast<int>(m_fHeight);

	//各テクスチャ生成
	if (m_pTextureMap == nullptr)
	{
		D3DXCreateTexture(pDevice, nWidth, nHeight,
			1,
			D3DUSAGE_RENDERTARGET,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, //こいつのためデバイスロスト時は破棄が必要
			&m_pTextureMap);
	}
	
	if (m_pTextureUnex == nullptr)
	{
		D3DXCreateTexture(pDevice, nWidth, nHeight,
			1,
			D3DUSAGE_DYNAMIC,
			D3DFMT_A8R8G8B8,
			D3DPOOL_DEFAULT, //こいつのためデバイスロスト時は破棄が必要
			&m_pTextureUnex);

		//あらかじめ黒く塗りつぶす
		D3DLOCKED_RECT lockrect;
		m_pTextureUnex->LockRect(0, &lockrect, nullptr, 0);
		BYTE* pBitByte = (BYTE*)lockrect.pBits;
		for (int y = 0; y < nHeight; y++)
		{
			DWORD* pBitColor = (DWORD*)(pBitByte + y * lockrect.Pitch);
			for (int x = 0; x < nWidth; x++)
			{
				pBitColor[x] = 0xff000000;
			}
		}

		m_pTextureUnex->UnlockRect(0);
	}

	if (m_pZSurface == nullptr)
	{
		//共通Zバッファ生成
		pDevice->CreateDepthStencilSurface(
			nWidth, nHeight,
			D3DFMT_D16,
			D3DMULTISAMPLE_NONE,
			0, false,
			&m_pZSurface,
			nullptr);
	}
}

//===============================================
// テクスチャ開放処理
//===============================================
void CMiniMap::UnLoad(void)
{
	if (m_pTextureMap != nullptr)
	{
		m_pTextureMap->Release();
		m_pTextureMap = nullptr;
	}
	if (m_pTextureUnex != nullptr)
	{
		m_pTextureUnex->Release();
		m_pTextureUnex = nullptr;
	}
	if (m_pZSurface != nullptr)
	{
		m_pZSurface->Release();
		m_pZSurface = nullptr;
	}
}

//===============================================
// テクスチャリセット処理
//===============================================
void CMiniMap::Reset(void)
{
	if (m_pTextureUnex != nullptr)
	{
		//int型サイズ
		int nWidth = static_cast<int>(m_fWidth);
		int nHeight = static_cast<int>(m_fHeight);

		//あらかじめ黒く塗りつぶす
		D3DLOCKED_RECT lockrect;
		m_pTextureUnex->LockRect(0, &lockrect, nullptr, 0);
		BYTE* pBitByte = (BYTE*)lockrect.pBits;
		for (int y = 0; y < nHeight; y++)
		{
			DWORD* pBitColor = (DWORD*)(pBitByte + y * lockrect.Pitch);
			for (int x = 0; x < nWidth; x++)
			{
				pBitColor[x] = 0xffffffff;
				m_ppExplored[x][y] = false;
			}
		}

		m_pTextureUnex->UnlockRect(0);
	}
}

//===============================================
// 生成処理
//===============================================
CMiniMap* CMiniMap::Create(const int playerNum, const int elaseWidth, const int elaseHeight)
{
	CMiniMap* pMiniMap = nullptr;

	if (pMiniMap == nullptr)
	{//ちゃんとぬるぽだから生成
		pMiniMap = new CMiniMap;
		pMiniMap->m_nPlayerNum = playerNum;
		pMiniMap->m_fWidth = PlacePos::NUM_PLACE_DATA[playerNum - 1].fWidth;
		pMiniMap->m_fHeight = PlacePos::NUM_PLACE_DATA[playerNum - 1].fHeight;
		pMiniMap->m_nElaseWidth = elaseWidth;
		pMiniMap->m_nElaseHeight = elaseHeight;
		pMiniMap->m_pos = PlacePos::NUM_PLACE_DATA[playerNum - 1].pos;
		pMiniMap->Init();
	}

	return pMiniMap;
}

//===============================================
// 出口のアイコン表示・非表示
//===============================================
void CMiniMap::SetGoalIcon(const bool bDisp)
{
	if (m_pExitIcon != nullptr)
	{
		m_pExitIcon->SetDraw(bDisp);
	}
}

//===============================================
// 出口配置
//===============================================
void CMiniMap::SetExit(void)
{
	//プレイヤーごとに必要な変数
	CGimmick* pGimmick = CGimmick::GetTop();
	while (pGimmick != nullptr)
	{
		CGimmick* pGimmickNext = pGimmick->GetNext();

		if (pGimmick->GetType() == CGimmick::TYPE_STARTDOOR)
		{
			break;
		}

		pGimmick = pGimmickNext;
	}

	if (pGimmick != nullptr)
	{
		D3DXVECTOR3 posPlayer = pGimmick->GetPosition();
		D3DXVECTOR3 posScreen;
		D3DXMATRIX mtx;

		//int型サイズ
		int nWidth = static_cast<int>(m_fWidth);
		int nHeight = static_cast<int>(m_fHeight);

		//必要なマトリ計算
		mtx = m_mtxView * m_mtxProj * m_mtxViewPort;	//内部でD3DXMatrixMultiplyやってるみたい

		//これでスクリーン座標に変換できた
		D3DXVec3TransformCoord(&posScreen, &posPlayer, &mtx);
		int posX = (int)posScreen.x;
		int posY = (int)posScreen.y;
		int posElaseMinX = ((posX - m_nElaseWidth) > 0) ? posX - m_nElaseWidth : 0;
		int posElaseMaxX = ((posX + m_nElaseWidth) < nWidth) ? posX + m_nElaseWidth : nWidth;
		int posElaseMinY = ((posY - m_nElaseHeight) > 0) ? posY - m_nElaseHeight : 0;
		int posElaseMaxY = ((posY + m_nElaseHeight) < nHeight) ? posY + m_nElaseHeight : nHeight;

		//アイコン置く（脱出後の挙動により変更予定）
		if (m_pExitIcon != nullptr)
		{
			D3DXVECTOR3 posIcon = m_pos;
			posIcon.x += -m_fWidth * 0.5f + static_cast<float>(posX);
			posIcon.y += -m_fHeight * 0.5f + static_cast<float>(posY);
			m_pExitIcon->SetPosition(posIcon);
			m_pExitIcon->SetVtx();
			m_pExitIcon->SetDraw(false);
		}
	}
}
