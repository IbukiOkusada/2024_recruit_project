//===============================================
//
// オブジェクトの処理 [object.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

// マクロ定義
#define DEFAULT_PRIORITY	(3)	// 基本の優先順位

//===============================================
// 静的メンバ変数宣言
//===============================================
CObject *CObject::m_apObject[NUM_PRIORITY][MAX_OBJECT] = {};	// オブジェクトのポインタ
int CObject::m_nNumAll = 0;
int CObject::m_nItemNumAll = 0;
int CObject::m_aPriNumAll[NUM_PRIORITY] = {};

//===============================================
// コンストラクタ
//===============================================
//CObject::CObject()
//{
//	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
//	{
//		if (m_apObject[DEFAULT_PRIORITY][nCntObject] == NULL)
//		{// 使用されていない場合
//			m_apObject[DEFAULT_PRIORITY][nCntObject] = this;	// 自分自身を代入
//			m_nID = nCntObject;	// 自分自身のIDを保
//			m_nNumAll++;			// 総数をカウントアップ
//			m_nPriority = DEFAULT_PRIORITY;
//			m_type = TYPE_NONE;	// 種類をリセット
//			break;
//		}
//	}
//}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CObject::CObject(int nPriority)
{
	if (nPriority < 0)
	{
		nPriority = 0;
	}
	else if (nPriority >= NUM_PRIORITY)
	{
		nPriority = NUM_PRIORITY - 1;
	}

	m_nID = -1;

	for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
	{
		if (m_apObject[nPriority][nCntObject] == NULL)
		{// 使用されていない場合
			m_apObject[nPriority][nCntObject] = this;	// 自分自身を代入
			m_nID = nCntObject;	// 自分自身のIDを保存
			m_nNumAll++;			// 総数をカウントアップ
			m_aPriNumAll[nPriority]++;	// 優先順位の総数カウントアップ
			m_nPriority = nPriority;
			m_type = TYPE_NONE;	// 種類をリセット
			break;
		}
	}
}

//===============================================
// デストラクタ
//===============================================
CObject::~CObject()
{
	
}

#if 0	// 純粋仮想関数なので通らない
//===============================================
// 初期化処理
//===============================================
HRESULT CObject::Init(void)
{

}
#endif

#if 0	// 純粋仮想関数なので通らない
//===============================================
// 終了処理
//===============================================
void CObject::Uninit(void)
{
	
}
#endif

#if 0	// 純粋仮想関数なので通らない
//===============================================
// 更新処理
//===============================================
void CObject::Update(void)
{

}
#endif

#if 0	// 純粋仮想関数なので通らない
//===============================================
// 描画処理
//===============================================
void CObject::Draw(void)
{

}
#endif

//===============================================
// 全てのオブジェクトの廃棄
//===============================================
void CObject::ReleaseAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
		{
			if (m_apObject[nCntPri][nCntObj] != NULL)
			{// 使用している場合
				// 終了処理
				m_apObject[nCntPri][nCntObj]->Uninit();
			}
		}
	}
}

//===============================================
// 全てのオブジェクトの更新
//===============================================
void CObject::UpdateAll(void)
{
	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{
			if (m_apObject[nCntPri][nCntObject] != NULL)
			{// 使用している場合
				// 更新処理
				m_apObject[nCntPri][nCntObject]->Update();
			}
		}
	}
}

//===============================================
// 全てのオブジェクトの描画
//===============================================
void CObject::DrawAll(void)
{
	CCamera *pCamera = CManager::GetCamera();

	if (pCamera != NULL)
	{// 使用されている場合
		pCamera->SetCamera();
	}

	for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	{
		for (int nCntObject = 0; nCntObject < MAX_OBJECT; nCntObject++)
		{
			if (m_apObject[nCntPri][nCntObject] != NULL)
			{// 使用している場合
				// 描画処理
				m_apObject[nCntPri][nCntObject]->Draw();
			}
		}
	}
}

//===============================================
// オブジェクトの廃棄
//===============================================
void CObject::Release(void)
{
	int nID = m_nID;	// 番号保持
	int nPri = m_nPriority;	// 優先順位番号保持
	TYPE type = m_type;

	if (m_apObject[nPri][nID] != NULL)
	{// 使用している場合
		delete m_apObject[nPri][nID];	// メモリの開放
		m_apObject[nPri][nID] = NULL;	// 使用していない状態にする
		m_nNumAll--;			//総数をカウントダウン
		m_aPriNumAll[nPri]--;	// 優先順位ごとの総数カウントダウン

		if (type == TYPE_MODEL)
		{
			m_nItemNumAll--;
		}
	}

}

//===============================================
// タイプの設定
//===============================================
void CObject::SetType(const TYPE type)
{
	if (m_type != type)
	{
		if (type == TYPE_MODEL)
		{
			m_nItemNumAll++;
		}
	}

	m_type = type;	// 種類を設定
}
