//==========================================================
//
// フィルターの処理 [filter.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _FILTER_H_		// このマクロが定義されていない場合
#define _FILTER_H_		// 二重インクルード防止用マクロを定義

#include "object2D.h"

//==========================================================
// フィルターのクラス定義(派生クラス)
//==========================================================
class CFilter : public CObject2D
{
public:

	// 使用種類
	enum TYPE
	{
		TYPE_SLOWGUN = 0,	// 二丁拳銃スロー
		TYPE_SLOWSHW,		// シャワースロー
		TYPE_MAX
	};

private:	// 自分だけがアクセス可能
	// 状態
	enum STATE
	{
		STATE_NONE = 0,	// 何もない
		STATE_NORMAL,	// 通常
		STATE_START,	// 開始
		STATE_END,		// 終了
	};

public:	// 誰でもアクセス可能

	//CFilter();	// コンストラクタ
	CFilter(int nPriority = 7);	// デフォルト引数コンストラクタ
	virtual ~CFilter();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CFilter *Create(TYPE type);
	D3DXCOLOR GetCol(void);

private:	// 自分だけがアクセス可能

	// メンバ変数
	D3DXCOLOR m_col;	// 色
	float m_fSpeed;		// サイズ変更
	float m_fCola;		// 透明度変更量
	STATE m_state;		// 状態
};

//==========================================================
// 陽炎フィルタークラス
//==========================================================
class CHeatFilter : public CObject2D
{
public:	// 誰でもアクセス可能

	//CFilter();	// コンストラクタ
	CHeatFilter(int nPriority = 7);	// デフォルト引数コンストラクタ
	~CHeatFilter();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CHeatFilter *Create();
	void SetFil(float fDiff);

private:	// 自分だけがアクセス可能

	// メンバ変数
	D3DXCOLOR m_col;	// 色
	D3DXVECTOR3 m_move;	// 移動量
	float m_fTex;		// テクスチャ座標
};

#endif