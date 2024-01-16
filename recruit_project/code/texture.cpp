//===============================================
//
// テクスチャの管理全般 [texture.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include <string.h>

// 静的メンバ変数宣言
int CTexture::m_nNumAll = 0;	// 読み込み総数

// ファイル名
const char *CTexture::m_apDefFileName[TYPE_MAX] =
{
	"data\\TEXTURE\\effect000.jpg",
	"data\\TEXTURE\\money0.png",
	"data\\TEXTURE\\frame0.png",
	"data\\TEXTURE\\frame1.png",
	"data\\TEXTURE\\frame2.png",
	"data\\TEXTURE\\frame3.png",
	"data\\TEXTURE\\player_icon0.png",
	"data\\TEXTURE\\player_icon1.png",
	"data\\TEXTURE\\player_icon2.png",
	"data\\TEXTURE\\player_icon3.png",
	"data\\TEXTURE\\number004.png",
	"data\\TEXTURE\\heartL_0.png",
	"data\\TEXTURE\\heartR_0.png",
	"data\\TEXTURE\\smook.png",
	"data\\TEXTURE\\rank00.png",
	"data\\TEXTURE\\ranking_one00.png",
	"data\\TEXTURE\\ranking_team00.png",
	"data\\TEXTURE\\new_record00.png",
	"data\\TEXTURE\\rank01.png",
	"data\\TEXTURE\\clear000.png",
	"data\\TEXTURE\\clear001.png",
	"data\\TEXTURE\\effect000.png",
	"data\\TEXTURE\\effect001.png",
	"data\\TEXTURE\\effect002.png",
	"data\\TEXTURE\\effect003.png",
	"data\\TEXTURE\\warning000.png",
};

//===============================================
// コンストラクタ
//===============================================
CTexture::CTexture()
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		m_aFile[nCntTex].pTexture = NULL;	// 使用していない状態にする
		memset(&m_aFile[nCntTex].aName[0], '\0', sizeof(File::aName));	// 名前のクリア
	}
}

//===============================================
// デストラクタ
//===============================================
CTexture::~CTexture()
{
	
}

//===============================================
// 読み込み
//===============================================
HRESULT CTexture::Load(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();

	// 総数分読み込み
	for (int nCntTex = 0; nCntTex < TYPE_MAX; nCntTex++)
	{
		if (m_aFile[nCntTex].pTexture == NULL)
		{// 使用されていない場合
			if (m_aFile[nCntTex].aName[0] == '\0')
			{// ファイル名が存在していない場合
				D3DXCreateTextureFromFile(pDevice,
					m_apDefFileName[nCntTex],
					&m_aFile[nCntTex].pTexture);

				strcpy(&m_aFile[nCntTex].aName[0], m_apDefFileName[nCntTex]);

				m_nNumAll++;
			}
		}
	}

	return S_OK;
}

//===============================================
// 廃棄
//===============================================
void CTexture::Unload(void)
{
	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		// テクスチャの廃棄
		if (m_aFile[nCntTex].pTexture != NULL)
		{// 使用されている場合
			m_aFile[nCntTex].pTexture->Release();
			m_aFile[nCntTex].pTexture = NULL;
			m_nNumAll--;
		}
	}
}

//===============================================
// 指定ファイル読み込み
//===============================================
int CTexture::Regist(const char* pFileName)
{
	CRenderer *pRenderer = CManager::GetInstance()->GetRenderer();
	LPDIRECT3DDEVICE9 pDevice = pRenderer->GetDevice();		//デバイスへのポインタを取得
	int nIdx = -1;	// テクスチャ番号

	for (int nCntTex = 0; nCntTex < MAX_TEXTURE; nCntTex++)
	{
		if (m_aFile[nCntTex].pTexture == NULL)
		{// 使用されていない場合
			// ファイル名を取得
			strcpy(&m_aFile[nCntTex].aName[0], pFileName);

			// テクスチャの読み込み
			if (SUCCEEDED(D3DXCreateTextureFromFile(pDevice,
				&m_aFile[nCntTex].aName[0],
				&m_aFile[nCntTex].pTexture)))
			{
				m_nNumAll++;	// 総数をカウントアップ
				nIdx = nCntTex;	// テクスチャ番号を設定
			}

			break;
		}
		else
		{// 使用されている場合
			if (strstr(&m_aFile[nCntTex].aName[0], pFileName) != NULL)
			{// ファイル名が一致している場合
				nIdx = nCntTex;	// テクスチャ番号を設定
				break;
			}
		}
	}

	return nIdx;	// テクスチャ番号を返す
}

//===============================================
// 指定アドレスのテクスチャを取得
//===============================================
LPDIRECT3DTEXTURE9 CTexture::SetAddress(int nIdx)
{
	if (nIdx > m_nNumAll || nIdx < 0)
	{// 読み込み範囲外の場合
		return NULL;
	}

	return m_aFile[nIdx].pTexture;
}

//===============================================
// テクスチャ名を取得
//===============================================
const char *CTexture::GetFileName(int nIdx)
{
	if (nIdx >= 0 && nIdx < TYPE_MAX)
	{// 範囲内
		return m_apDefFileName[nIdx];
	}

	return NULL;
}