//==========================================================
//
// スコアの処理 [score.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _SCORE_H_	// このマクロが定義されていない場合
#define _SCORE_H_	// 二重インクルード防止用マクロを定義

#include "main.h"

// 前方宣言
class CNumber;

// マクロ定義
#define NUM_SCORE	(8)	// スコアの桁数

//==========================================================
// テクスチャスライドオブジェクト(2D)のクラス定義(派生クラス)
//==========================================================
class CScore
{
public:	// ここから関数(誰でもアクセス可能)

	CScore();	// コンストラクタ
	~CScore();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CScore *Create(D3DXVECTOR3 pos, const int nDegit = NUM_SCORE);

	// メンバ関数
	void Add(int nValue);
	void Set(int nValue);
	void SetColor(D3DXCOLOR col);

	// メンバ関数(取得)
	int GetNum(void) { return m_nNum; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void SetValue(void);

	// メンバ変数
	CNumber *m_apNumber[NUM_SCORE];	// 数字のポインタ
	D3DXVECTOR3 m_pos;	// 座標
	int m_nNum;	// 現在の値
	int m_nIdxTexture;	// テクスチャ番号
	int m_nDesit;	// 桁数
};

#endif
