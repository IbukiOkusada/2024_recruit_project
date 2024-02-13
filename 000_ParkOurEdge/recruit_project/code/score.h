//===============================================
//
// 数字全般の処理 [score.h]
// Author : Ryosuke Ohara
//
//===============================================
#ifndef _SCORE_H_		// このマクロが定義されていない場合
#define _SCORE_H_		// 二重インクルード防止用マクロを定義

#include "main.h"

// 前方宣言
class CNumber;

// マクロ定義
#define NUM_SCORE  (8)  //スコアの桁数

//===============================================
// ナンバークラスの定義
//===============================================
class CScore
{
public:	// 誰でもアクセス可能

	CScore();	// コンストラクタ
	~CScore();	// デストラクタ

	// メンバ関数
	HRESULT Init(D3DXVECTOR3 pos, int nDesit, float fGap, float fWidth, float fHeight);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CScore *Create(void);
	static CScore *Create(D3DXVECTOR3 pos, int nDesit, float fGap, float fWidth, float fHeight);
	//CObject2D *GetObject2D(void) { return m_pObject2D; }
	void PolygonDelete(void);

	void BindTexture(LPDIRECT3DTEXTURE9 m_Texture);  //テクスチャをバインド

	void AddScore(int nScore);  // スコア加算処理
	void LowerScore(int nScore);  // スコア減算処理

	// メンバ関数(設定)
	void SetIdx(const int nIdx);
	void SetScore(int nScore);
	void SetClo(D3DXCOLOR col);
	void SetPosition(const D3DXVECTOR3& pos);

	// メンバ関数(取得)
	int GetIdx(void) { return m_nIdx; }
	int GetScore(void) { return m_nNumScore; }
	D3DXVECTOR3 GetPosition(void) const { return m_pos; }

private:	// 自分だけがアクセス可能

	// メンバ変数
	CNumber *m_apNumber[NUM_SCORE];	// ナンバーへのポインタ
	D3DXVECTOR3 m_pos;				// 位置
	float m_fHeight;				// 高さ
	float m_fWidth;				    // 幅
	float m_fGap;
	int m_nDesit;                   // 桁数
	int m_nIdx;					    // 数字の番号
	int m_nNumScore;			    // スコア
};

#endif