//==========================================================
//
// メッセージ表示の処理 [message.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "message.h"
#include "manager.h"
#include "game.h"

//===============================================
// マクロ定義
//===============================================
#define VTX	(4)	// 頂点数
#define WIDTH	(100.0f)	// 幅
#define HEIGHT	(100.0f)	// 高さ
#define POS_X	(300.0f)	// X座標
#define POS_Y	(300.0f)	// Y座標
#define TEX_FILENAME "data\\TEXTURE\\meteor001.png"	// テクスチャ名

//====================================================================================
// オブジェクト2Dクラス
//====================================================================================
//===============================================
// コンストラクタ
//===============================================
//CMessage::CMessage()
//{
//	// 値をクリアする
//	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
//	m_fLength = 0.0f;
//	m_fAngle = 0.0f;
//	m_fWidth = 0.0f;
//	m_fHeight = 0.0f;
//}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CMessage::CMessage(int nPriority) : CObject2D(nPriority)
{
	// 値をクリアする
}

//===============================================
// デストラクタ
//===============================================
CMessage::~CMessage()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CMessage::Init(void)
{
	CObject2D::Init();

	m_col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CMessage::Uninit(void)
{
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CMessage::Update(void)
{
	m_col.a -= 0.02f;

	if (m_col.a < 0.0f)
	{
		Uninit();
	}
	else
	{
		SetCol(m_col);
	}
}

//===============================================
// 描画処理
//===============================================
void CMessage::Draw(void)
{
	CObject2D::Draw();
}

//===============================================
// 頂点情報設定
//===============================================
CMessage *CMessage::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nPriority)
{
	CMessage *pObject2D = NULL;

	// オブジェクト2Dの生成
	pObject2D = new CMessage(nPriority);

	if (pObject2D != NULL)
	{// 生成できた場合
		// 初期化処理
		pObject2D->Init();

		pObject2D->SetPosition(pos);

		pObject2D->SetRotation(rot);

		pObject2D->SetLength(300.0f, 100.0f);

		// 種類設定
		pObject2D->SetType(TYPE_NONE);
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pObject2D;
}
