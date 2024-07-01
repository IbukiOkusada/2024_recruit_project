//==========================================================
//
// メッセージ表示の処理 [message.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _MESSAGE_H_		// このマクロが定義されていない場合
#define _MESSAGE_H_		// 二重インクルード防止用マクロを定義

#include "object2D.h"

//==========================================================
// オブジェクト(2D)のクラス定義(派生クラス)
//==========================================================
class CMessage : public CObject2D
{
public:	// 誰でもアクセス可能

	//CMessage();	// コンストラクタ
	CMessage(int nPriority = 7);	// デフォルト引数コンストラクタ
	virtual ~CMessage();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CMessage *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nPriority = 7);

private:	// 自分だけがアクセス可能

	// メンバ変数
	D3DXCOLOR m_col;	// 色
};

#endif