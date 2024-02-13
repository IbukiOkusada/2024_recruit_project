//===============================================
//
// オブジェクトの処理 [object.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _OBJECT_H_		// このマクロが定義されていない場合
#define _OBJECT_H_		// 二重インクルード防止用マクロを定義

#include "main.h"

// マクロ定義
#define MAX_OBJECT		(1024)	// 最大数
#define NUM_PRIORITY	(8)		// 優先順位管理数

// 前方宣言
class CObject2D;
class CMultiBg;

//===============================================
// オブジェクトクラスの定義(抽象クラス)
//===============================================
class CObject
{
public:	// 誰でもアクセス可能

		// 種類列挙型の定義
	typedef enum
	{
		TYPE_NONE = 0,	// 何もない
		TYPE_BG,		// 背景
		TYPE_PLAYER,	// プレイヤー
		TYPE_ENEMY,		// 敵
		TYPE_BULLET,	// 弾
		TYPE_ITEM,		// アイテム
		TYPE_EXPLOSION,	// 爆発
		TYPE_EFFECT,	// エフェクト
		TYPE_BLOCK,		// ブロック
		TYPE_MODEL,		// モデル
		TYPE_MAX
	}TYPE;

	//CObject();			// コンストラクタ
	CObject(int nPriority = 3);	// コンストラクタ(オーバーロード)デフォルト引数
	virtual ~CObject();	// デストラクタ

	// 純粋仮想関数関数
	virtual HRESULT Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	// メンバ関数(設定)
	virtual void SetPosition(const D3DXVECTOR3 pos) = 0;
	virtual void SetRotation(const D3DXVECTOR3 rot) = 0;

	// メンバ関数(取得)
	virtual D3DXVECTOR3 GetPosition(void) = 0;
	virtual D3DXVECTOR3 GetRotation(void) = 0;
	virtual CObject2D *GetObject2D(void) { return NULL; }

	// メンバ関数
	static void ReleaseAll(void);
	static void UpdateAll(void);
	static void DrawAll(void);
	static CObject *GetObject(const int nPriority,const int nIdx) { return m_apObject[nPriority][nIdx]; }
	static int GetNumAll(void) { return m_nNumAll; }
	static int GetItemNumAll(void) { return m_nItemNumAll; }
	static int GetPriNumAll(int nPriority) { return m_aPriNumAll[nPriority]; }

	// メンバ関数(取得
	TYPE GetType(void) { return m_type; }
	int GetId(void) { return m_nID; }

	// メンバ関数(設定)
	void SetType(const TYPE type);

protected:	// 派生クラスからもアクセス可能

	// メンバ関数
	void Release(void);

private:	// 自分だけがアクセス可能

	static CObject *m_apObject[NUM_PRIORITY][MAX_OBJECT];	// オブジェクトクラスのポインタ
	static int m_aPriNumAll[NUM_PRIORITY];	// 各優先順位ごとの総数
	static int m_nNumAll;	// オブジェクト総数
	static int m_nItemNumAll;	// アイテムの総数
	int m_nID;			// 自分自身のID
	int m_nPriority;	// 優先順位の位置
	TYPE m_type;		// 種類
};

#endif
