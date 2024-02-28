//==========================================================
//
// メッセージ表示の処理 [message.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _MESSAGE_H_		// このマクロが定義されていない場合
#define _MESSAGE_H_		// 二重インクルード防止用マクロを定義

#include "task.h"

// 前方宣言
class CObjectBillboard;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CMessage : public CTask
{

public:	// 誰でもアクセス可能

	CMessage();	// コンストラクタ(オーバーロード)
	~CMessage();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CMessage *Create(const D3DXVECTOR3& pos);

	// メンバ関数(取得)

	// メンバ関数(設定)

private:	// 自分だけがアクセス可能

	// メンバ関数
	void SetPosition(const D3DXVECTOR3& pos);

	// メンバ変数
	CObjectBillboard* m_pObj;	// 描画オブジェクト
	D3DXVECTOR3 m_pos;			// 座標
	float m_fLife;				// 体力
};

#endif
