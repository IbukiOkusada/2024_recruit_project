//==========================================================
//
// アイテムの処理 [item.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _ITEM_H_		// このマクロが定義されていない場合
#define _ITEM_H_		// 二重インクルード防止用マクロを定義

#include "main.h"
#include "object2D.h"

//==========================================================
// アイテムのクラス定義(派生クラス)
//==========================================================
class CItem : public CObject2D
{
public:	// 誰でもアクセス可能

	// 種類列挙型の定義
	typedef enum
	{
		TYPE_MOON = 0,	// 月
		TYPE_DIAMOND,	// 宝石
		TYPE_MAX
	}TYPE;

	//CItem();	// コンストラクタ
	CItem(int Priority = 4);
	~CItem();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CItem *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 move, TYPE type);

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Controller(void);
	bool Collision(D3DXVECTOR3 pos);

	// メンバ変数
	int m_nIdxTexture;
	static const char *m_apTextureFileName[];	// ファイル名
	int m_nType;			// 種類
};

#endif
