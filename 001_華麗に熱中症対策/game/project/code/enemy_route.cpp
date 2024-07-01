//==========================================================
//
// 敵の順路管理処理 [enemy_route.h]
// Author : Ibuki Okusada
//
//==========================================================
#include "enemy_route.h"
#include <stdio.h>

// マクロ定義
#define NEXTWAVECNT	(20)	// 各wave開始カウント

//==========================================================
// コンストラクタ
//==========================================================
CEnemyRoute::CEnemyRoute()
{
	for (int nCnt = 0; nCnt < MAX_ROUTE; nCnt++)
	{
		m_apRoute[nCnt] = NULL;
	}

	m_nNumAll = 0;
}

//==========================================================
// デストラクタ
//==========================================================
CEnemyRoute::~CEnemyRoute()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CEnemyRoute::Init(void)
{

	return true;
}

//==========================================================
// 終了処理
//==========================================================
void CEnemyRoute::Uninit(void)
{
	for (int nCnt = 0; nCnt < MAX_ROUTE; nCnt++)
	{
		if (m_apRoute[nCnt] == NULL)
		{
			continue;
		}

		if (m_apRoute[nCnt]->route.pPoint != NULL)
		{// 登録されている
			delete m_apRoute[nCnt]->route.pPoint;
			m_apRoute[nCnt]->route.pPoint = NULL;
		}

		delete m_apRoute[nCnt];
		m_apRoute[nCnt] = NULL;
		m_nNumAll--;	// 総数をカウントダウン
	}
}

//===============================================
// 指定ファイルの情報を取得
//===============================================
CEnemyRoute::Route *CEnemyRoute::SetAddress(const char *pFileName)
{
	for (int nCnt = 0; nCnt < MAX_ROUTE; nCnt++)
	{
		if (m_apRoute[nCnt] == NULL)
		{// 使用されていない場合

			m_apRoute[nCnt] = new RouteData;

			// ファイル名を取得
			strcpy(&m_apRoute[nCnt]->aName[0], pFileName);

			if (Load(&m_apRoute[nCnt]->route, pFileName) == true)
			{
				return &m_apRoute[nCnt]->route;
			}
			else
			{
				delete m_apRoute[nCnt];
				m_apRoute[nCnt] = NULL;
			}
		}
		else
		{// 使用されている場合
			if (strstr(&m_apRoute[nCnt]->aName[0], pFileName) != NULL)
			{// ファイル名が一致している場合
				return &m_apRoute[nCnt]->route;
				break;
			}
		}
	}

	return NULL;
}

//===============================================
// 指定アドレスの情報を取得
//===============================================
CEnemyRoute::Route *CEnemyRoute::SetAddress(const int nIdx)
{
	if (nIdx > m_nNumAll || nIdx < 0)
	{// 読み込み範囲外の場合
		return NULL;
	}
	else if (m_apRoute[nIdx] == NULL)
	{
		return NULL;
	}

	return &m_apRoute[nIdx]->route;
}

//===============================================
// 指定ファイルのアドレスを取得
//===============================================
int CEnemyRoute::Regist(const char *pFileName)
{
	int nIdx = -1;	// 番号

	for (int nCnt = 0; nCnt < MAX_ROUTE; nCnt++)
	{
		if (m_apRoute[nCnt] == NULL)
		{// 使用されていない場合

			m_apRoute[nCnt] = new RouteData;
			// ファイル名を取得
			strcpy(&m_apRoute[nCnt]->aName[0], pFileName);

			if (Load(&m_apRoute[nCnt]->route, pFileName) == true)
			{
				nIdx = nCnt;	// テクスチャ番号を設定
				break;
			}
			else
			{
				delete m_apRoute[nCnt];
				m_apRoute[nCnt] = NULL;
			}
		}
		else
		{// 使用されている場合
			if (strstr(&m_apRoute[nCnt]->aName[0], pFileName) != NULL)
			{// ファイル名が一致している場合
				nIdx = nCnt;	// テクスチャ番号を設定
				break;
			}
		}
	}

	return nIdx;	// テクスチャ番号を返す
}

//===============================================
// ファイル読み込み
//===============================================
bool CEnemyRoute::Load(Route *pRoute, const char *pFileName)
{
	pRoute->nNumPoint = 0;
	pRoute->pPoint = NULL;
	pRoute->StartPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pRoute->StartRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	FILE *pFile = fopen(pFileName, "r");	// ファイルを開く

	if (pFile == NULL)
	{// 開けなかった
		return false;
	}

	char aStr[128] = {};

	// 最後まで繰り返し読み込み
	while (fscanf(pFile, "%s", &aStr[0]) != EOF)
	{
		if (strcmp(&aStr[0], "SCRIPT") == 0)
		{//スクリプト開始の文字が確認できた場合
			int nPoint = 0;	// 確認するポイント№
			// 終了文字まで
			while (strcmp(&aStr[0], "END_SCRIPT") != 0)
			{
				int nResult = fscanf(pFile, "%s", &aStr[0]);

				if (nResult == EOF)
				{// 終了
					break;
				}
				else if (pRoute->nNumPoint != 0 && nPoint >= pRoute->nNumPoint)
				{// 全て読み込んだ
					break;
				}

				// 確認
				if (strcmp(&aStr[0], "NUM_POINT") == 0)
				{// 総数
					fscanf(pFile, "%s", &aStr[0]);			
					fscanf(pFile, "%d", &pRoute->nNumPoint);
					pRoute->pPoint = new D3DXVECTOR3[pRoute->nNumPoint];

					for (int nCnt = 0; nCnt < pRoute->nNumPoint; nCnt++)
					{
						pRoute->pPoint[nCnt] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				else if (strcmp(&aStr[0], "POS") == 0)
				{// 開始地点
					fscanf(pFile, "%s", &aStr[0]);
					fscanf(pFile, "%f", &pRoute->StartPos.x);
					fscanf(pFile, "%f", &pRoute->StartPos.y);
					fscanf(pFile, "%f", &pRoute->StartPos.z);
				}
				else if (strcmp(&aStr[0], "ROT") == 0)
				{// 開始地点
					fscanf(pFile, "%s", &aStr[0]);
					fscanf(pFile, "%f", &pRoute->StartRot.x);
					fscanf(pFile, "%f", &pRoute->StartRot.y);
					fscanf(pFile, "%f", &pRoute->StartRot.z);
				}
				else if (strcmp(&aStr[0], "MOVE") == 0)
				{// 移動量
					fscanf(pFile, "%s", &aStr[0]);
					fscanf(pFile, "%f", &pRoute->fMove);
				}
				else if (strcmp(&aStr[0], "POINT") == 0)
				{// 開始地点
					fscanf(pFile, "%s", &aStr[0]);
					fscanf(pFile, "%f", &pRoute->pPoint[nPoint].x);
					fscanf(pFile, "%f", &pRoute->pPoint[nPoint].y);
					fscanf(pFile, "%f", &pRoute->pPoint[nPoint].z);
					nPoint++;
				}

			}
			//各データの読み込み開始
			break;
		}
	}

	// ファイルを閉じる
	fclose(pFile);

	if (pRoute->nNumPoint == 0)
	{// 読み込み失敗
		if (pRoute->pPoint != NULL)
		{
			delete[] pRoute->pPoint;
			pRoute->pPoint = NULL;
		}
		return false;
	}

	m_nNumAll++;	// 総数をカウントアップ

	return true;
}