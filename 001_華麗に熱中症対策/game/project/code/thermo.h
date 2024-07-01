//===============================================
//
// 温度表示の処理 [thermo.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _THERMO_H_		// このマクロが定義されていない場合
#define _THERMO_H_		// 二重インクルード防止用マクロを定義

#include "object3D.h"

//==========================================================
// フィルターのクラス定義(派生クラス)
//==========================================================
class CThermo : public CObject3D
{
public:	// 誰でもアクセス可能

	CThermo(int nPriority = 3);	// デフォルト引数コンストラクタ
	virtual ~CThermo();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CThermo *Create(void);

	// メンバ関数(取得)
	D3DXCOLOR GetCol(void) { return m_col; }	// 色を取得

	// メンバ関数(設定)
	void SetColor(D3DXCOLOR col) { m_colDiff = col; }	// 色を設定

private:	// 自分だけがアクセス可能

	D3DXCOLOR m_col;	// 色
	D3DXCOLOR m_colDiff;// 目的の色
	float m_fRadius;	// 半径
};

#endif