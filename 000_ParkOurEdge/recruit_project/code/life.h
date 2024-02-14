//==========================================================
//
// 体力UI [life.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _LIFE_H_		// このマクロが定義されていない場合
#define _LIFE_H_		// 二重インクルード防止用マクロを定義

#include "task.h"	// これでファイルインクルードできます

// 前方宣言
class CObject2D;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CLife : public CTask
{

public:	// 誰でもアクセス可能

	CLife();	// コンストラクタ(オーバーロード)
	~CLife();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CLife *Create(const D3DXVECTOR3& pos);

	// メンバ関数(取得)

	// メンバ関数(設定)

private:	// 自分だけがアクセス可能

	CObject2D* m_pObj;	// オブジェクトのポインタ
};

#endif
