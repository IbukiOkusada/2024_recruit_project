//==========================================================
//
// 体力UI管理 [life_ui.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _LIFEUI_H_		// このマクロが定義されていない場合
#define _LIFEUI_H_		// 二重インクルード防止用マクロを定義

#include "task.h"	// これでファイルインクルードできます

// 前方宣言
class CLife;
class CObject2D;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CLifeUI : public CTask
{

public:	// 誰でもアクセス可能

	CLifeUI();	// コンストラクタ(オーバーロード)
	~CLifeUI();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CLifeUI *Create(const D3DXVECTOR3& pos, const int nMax);
	void SetLife(const int nNum);

	// メンバ関数(取得)

	// メンバ関数(設定)

private:	// 自分だけがアクセス可能

	// メンバ関数
	void SetPosition(const D3DXVECTOR3& pos);
	void SetMaxLife(const int nMax);

	// メンバ変数
	int m_nMaxLife;	// 最大体力
	D3DXVECTOR3 m_pos;	// 座標
	CObject2D* m_pFrame;	// 枠組み
	CLife** m_ppObj;		// 体力
};

#endif
