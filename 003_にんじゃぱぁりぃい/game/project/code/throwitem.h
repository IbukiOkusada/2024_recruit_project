//==========================================================
//
// 捨てるアイテム [throwitem.h]
// Author : Ryosuke Ohara
//
//==========================================================
#ifndef _THROWITEM_H_		// このマクロが定義されていない場合
#define _THROWITEM_H_		// 二重インクルード防止用マクロを定義

#include "task.h"	// これでファイルインクルードできます

// 前方宣言
class CObjectBillboard;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CThrowItem : public CTask
{
public:

	// 種類列挙型
	enum TYPE
	{
		TYPE_ARROW = 0,         // 矢印
		TYPE_ITEM,  // アイテムのアイコン
		TYPE_FRAME,         // 枠
		TYPE_MAX
	};

public:	// 誰でもアクセス可能

	CThrowItem();		// コンストラクタ
	~CThrowItem();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	CObjectBillboard *GetObjectBillBoard(void) { return m_apObject[0]; }
	static CThrowItem *Create(D3DXVECTOR3* pPos, const float fUpHeight, const float fPolyWidth, const float fPolyHeight);

	// メンバ関数(取得)
	void SetParent(D3DXVECTOR3 *pPos) { m_pPos = pPos; }
	void SetUpHeight(const float fHeight) { m_fUpHeight = fHeight; }
	void SetItem(int nThrowItemID);


private:	// 自分だけがアクセス可能

	// メンバ関数
	void SetPolySize(const float fWidth, const float fHeight);
	void SetMixPosition(void);

	// メンバ変数
	D3DXVECTOR3 *m_pPos;	// 親の座標
	int m_nBeforeID;        // 現在選択している番号の前の番号
	int m_nNowID;           // 現在選択している番号
	int m_nNextID;          // 現在選択している番号の次の番号
	float m_fUpHeight;	    // 上げる高さ
	float m_fPolyWidth;	    // オブジェクトの幅
	float m_fPolyHeight;	// オブジェクトの高さ
	float m_fRate;		    // 割合
	CObjectBillboard *m_apObject[TYPE_MAX];	// オブジェクトの配列
};

#endif