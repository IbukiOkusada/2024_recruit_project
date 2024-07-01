//===============================================
//
// 濡れ跡の処理 [wet.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _WET_H_		// このマクロが定義されていない場合
#define _WET_H_		// 二重インクルード防止用マクロを定義

#include "shadow.h"

//===============================================
// 濡れ跡クラスの定義 (派生クラス)
//===============================================
class CWet : public CShadow
{
public:		// 誰でもアクセス可能

	CWet();		// コンストラクタ
	~CWet();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CWet *Create(D3DXVECTOR3 pos, float fWidth, float fHeight);
	void SetSize(float fWidth, float fHeight);

private:	// 自分だけがアクセス可能

	// メンバ変数
	float m_fLife;		// 寿命
	float m_fDefWidth;	// 幅
	float m_fDefHeight;	// 高さ
	D3DXCOLOR m_col;	// 色
};

#endif
