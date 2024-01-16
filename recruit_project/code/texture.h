//===============================================
//
// テクスチャの管理全般 [texture.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _TEXTURE_H_		// このマクロが定義されていない場合
#define _TEXTURE_H_		// 二重インクルード防止用マクロを定義

#include "main.h"

// マクロ定義
#define MAX_TEXTURE		(256)	// テクスチャの最大数
#define MAX_FILENAME	(256)	// ファイル名最大文字数

//**********************************************************
// テクスチャクラスの定義
//**********************************************************
class CTexture
{
public:	// 誰でもアクセス可能な定義
	
	// 初期読み込み種類
	enum TYPE
	{
		TYPE_EFFECT = 0,		// エフェクト
		TYPE_MONEY,				// スコアの単位
		TYPE_FRAME0,			// 枠0
		TYPE_FRAME1,			// 枠1
		TYPE_FRAME2,			// 枠2
		TYPE_FRAME3,			// 枠3
		TYPE_PLAYER0,			// プレイヤーの顔0
		TYPE_PLAYER1,			// プレイヤーの顔1
		TYPE_PLAYER2,			// プレイヤーの顔2
		TYPE_PLAYER3,			// プレイヤーの顔3
		TYPE_TIMER,			//  タイム
		TYPE_LIFE0,			// 体力0
		TYPE_LIFE1,			// 体力1
		TYPE_SMOOK,          // 煙
		TYPE_RANK,			// 順位
		TYPE_RANK_ONE,		// 個人スコア
		TYPE_RANK_TEAM,		// チームスコア
		TYPE_RANK_NEW,		// スコア更新
		TYPE_RESULTRANK,		// リザルト
		TYPE_RESULTCLEAR,		// ノルマ達成
		TYPE_RESULTFAILED,	// ノルマ失敗
		TYPE_ITEMGET_EF,		// アイテム獲得エフェクト
		TYPE_WOODPOW,			// 木っ端
		TYPE_WALK,			// 歩き
		TYPE_AIRWOOD,			// 風で舞った木っ端
		TYPE_WARNING,			// 警告
		TYPE_MAX				   
	};

private:	// 自分だけがアクセス可能な定義
	
	// ファイル情報
	typedef struct
	{
		LPDIRECT3DTEXTURE9 pTexture;	// テクスチャのポインタ
		char aName[MAX_FILENAME];		// ファイル名
	}File;

public:	// 誰でもアクセス可能

	CTexture();		// コンストラクタ
	~CTexture();	// デストラクタ

	// メンバ関数
	HRESULT Load(void);
	void Unload(void);
	int Regist(const char* pFileName);
	LPDIRECT3DTEXTURE9 SetAddress(int nIdx);

	// メンバ関数(取得)
	int GetNumAll(void) { return m_nNumAll; }
	const static char *GetFileName(int nIdx);

private:	// 自分だけがアクセス可能

	// メンバ関数
	File m_aFile[MAX_TEXTURE];	// テクスチャへのポインタ
	static int m_nNumAll;		// 読み込み総数
	const static char *m_apDefFileName[TYPE_MAX];	// 初期読み込みファイル名
};

#endif
