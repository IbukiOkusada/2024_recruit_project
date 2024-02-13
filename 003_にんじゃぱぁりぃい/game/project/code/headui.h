//===============================================
//
// 頭の上に出るUI処理 [overheadui.h]
// Author : Ryosuke Ohara
//
//===============================================
#ifndef _HEADUI_H_		// このマクロが定義されていない場合
#define _HEADUI_H_		// 二重インクルード防止用マクロを定義

#include "task.h"

// マクロ定義
#define NUM_DIGIT (2)  // 桁数

// 前方宣言
class CObjectBillboard;

//===============================================
// ナンバークラスの定義
//===============================================
class CHeadUI : public CTask
{
public:	// 誰でもアクセス可能

	CHeadUI();	// コンストラクタ
	~CHeadUI();	// デストラクタ

				// メンバ関数
				//HRESULT Init(D3DXVECTOR3 pos, float fWidth, float fHeight);
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CHeadUI *Create(void);
	static CHeadUI *Create(D3DXVECTOR3 * pPos, D3DXMATRIX *Matrix, const float fUpHeight, const float fPolyWidth, const float fPolyHeight);
	CObjectBillboard *GetObjectBill(void) { return m_apObject[0]; }

	// メンバ関数(設定)
	void SetIdx(const int nIdx);
	void SetParent(D3DXVECTOR3 *pPos) { m_pPos = pPos; }
	void SetUpHeight(const float fHeight) { m_fUpHeight = fHeight; }
	void SetMixPosition(void);

private:	// 自分だけがアクセス可能

			// メンバ関数
	void SetPolySize(const float fWidth, const float fHeight);
	
	void SetMtxParent(D3DXMATRIX *mtxParent);

	// メンバ変数
	D3DXVECTOR3 *m_pPos;	        // 親の座標
	D3DXMATRIX m_mtxWorld[NUM_DIGIT];          // ワールドマトリックス
	D3DXMATRIX *m_mtxParent;         // 親のマトリックス
	float m_fUpHeight;	            // 上げる高さ
	float m_fPolyWidth;	            // オブジェクトの幅
	float m_fPolyHeight;	        // オブジェクトの高さ
	CObjectBillboard *m_apObject[NUM_DIGIT];	// ビルボードのオブジェクトのポインタ
	int m_nIdx;                     // インデックス番号
};

#endif