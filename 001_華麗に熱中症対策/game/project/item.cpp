//===============================================
//
// アイテムの処理 [item.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "item.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "particle.h"
#include "sound.h"
#include "score.h"
#include "texture.h"

//===============================================
// マクロ定義
//===============================================
#define LIFE	(120)		// 寿命

//===============================================
// 静的メンバ変数宣言(ファイル名)
//===============================================
const char *CItem::m_apTextureFileName[] =
{
	"data\\TEXTURE\\item000.png",
	"data\\TEXTURE\\item001.png",
};

////===============================================
//// コンストラクタ
////===============================================
//CItem::CItem()
//{
//	// 値のクリア
//}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CItem::CItem(int nPriority) : CObject2D(nPriority)
{
	// 値のクリア
}

//===============================================
// デストラクタ
//===============================================
CItem::~CItem()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CItem::Init(void)
{
	// オブジェクトの初期化処理
	CObject2D::Init();

	m_nType = 0;

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CItem::Uninit(void)
{
	// オブジェクトの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CItem::Update(void)
{
	if (Collision(GetPosition()) == true)
	{// 当たった場合
		
		return;
	}
	// 座標更新
	Controller();

	// 頂点情報更新
	//SetVtx();
}

//===============================================
// 描画処理
//===============================================
void CItem::Draw(void)
{
	CObject2D::Draw();
}

//===============================================
// 頂点情報設定
//===============================================
CItem *CItem::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, TYPE type)
{
	CTexture *pTexture = CManager::GetTexture();	// テクスチャのポインタ
	CItem *pItem = NULL;

	// オブジェクト2Dの生成
	pItem = new CItem();

	if (pItem != NULL)
	{// 生成できた場合

		// 初期化処理
		pItem->Init();

		// 種類設定
		pItem->m_nType = type;

		// オブジェクトタイプの設定
		pItem->SetType(CObject::TYPE_ITEM);

		// 座標設定
		pItem->SetPosition(pos);

		// 向き設定
		pItem->SetRotation(rot);

		// サイズ設定
		pItem->SetLength(100.0f, 100.0f);

		// テクスチャの割り当て
		pItem->BindTexture(pTexture->Regist(m_apTextureFileName[type]));
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pItem;
}

//===============================================
// 操作処理
//===============================================
void CItem::Controller(void)
{
	
}

//===============================================
// アイテムとの当たり判定
//===============================================
bool CItem::Collision(D3DXVECTOR3 pos)
{
	//for (int nCntPri = 0; nCntPri < NUM_PRIORITY; nCntPri++)
	//{
	//	for (int nCntObj = 0; nCntObj < MAX_OBJECT; nCntObj++)
	//	{
	//		CObject *pObj;	// オブジェクトクラスのポインタ

	//		// オブジェクトの取得
	//		pObj = GetObject(nCntPri, nCntObj);

	//		if (pObj != NULL)
	//		{// 使用されている場合

	//			CObject::TYPE type;	// 種類

	//			// オブジェクトの種類を取得
	//			type = pObj->GetType();

	//			if (type == TYPE_PLAYER)
	//			{// 種類がプレイヤーの場合

	//				D3DXVECTOR3 ObjPos;	// オブジェクトの座標

	//				// 座標の取得
	//				ObjPos = pObj->GetPosition();

	//				float fObjWidth = 0.0f;
	//				float fObjHeight = 0.0f;

	//				// 座標の取得
	//				ObjPos = pObj->GetPosition();
	//				CObject2D *pObj2D = pObj->GetObject2D();

	//				if (pObj2D != NULL)
	//				{
	//					fObjWidth = pObj2D->GetWidth();
	//					fObjHeight = pObj2D->GetHeight();
	//				}

	//				if (pos.x + GetWidth() >= ObjPos.x - fObjWidth && pos.x - GetWidth() <= ObjPos.x + fObjWidth
	//					&& pos.y + GetHeight() >= ObjPos.y - fObjHeight && pos.y - GetHeight() <= ObjPos.y)
	//				{// 敵と重なった場合

	//					// 爆発の生成
	//					CExplosion::Create(pos);

	//					// スコアの加算
	//					CManager::GetScore()->Add(500);

	//					// 自分自身の終了処理
	//					Uninit();

	//					return TRUE;	// TRUEを返す(当たった)
	//				}

	//			}
	//		}
	//	}
	//}
	return FALSE;	// FALSEを返す(何にも当たっていない)
}