//==========================================================
//
// ゲージ [gage.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GAGE_H_		// このマクロが定義されていない場合
#define _GAGE_H_		// 二重インクルード防止用マクロを定義

#include "task.h"	// これでファイルインクルードできます

// 前方宣言
class CObjectBillboard;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CGage : public CTask
{
public:

	// 種類列挙型
	enum TYPE
	{
		TYPE_FRAME = 0,	// 枠
		TYPE_CONTENT,		// 中身
		TYPE_MAX
	};

public:	// 誰でもアクセス可能

	CGage();		// コンストラクタ
	~CGage();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CGage *Create(D3DXVECTOR3* pPos, const float fUpHeight, const float fPolyWidth, const float fPolyHeight);

	// メンバ関数(取得)
	void SetParent(D3DXVECTOR3 *pPos) { m_pPos = pPos; }
	void SetUpHeight(const float fHeight) { m_fUpHeight = fHeight; }
	void SetRate(const float fRate);

private:	// 自分だけがアクセス可能

	// メンバ関数
	void SetPolySize(const float fWidth, const float fHeight);
	void SetMixPosition(void);

	// メンバ変数
	D3DXVECTOR3 *m_pPos;	// 親の座標
	float m_fUpHeight;	// 上げる高さ
	float m_fPolyWidth;	// オブジェクトの幅
	float m_fPolyHeight;	// オブジェクトの高さ
	float m_fRate;		// 割合
	CObjectBillboard *m_apObject[TYPE_MAX];	// オブジェクトの配列
};

#endif

