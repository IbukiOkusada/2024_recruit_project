//==========================================================
//
// 爆発の処理 [explosion.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _EXPLOSION_H_		// このマクロが定義されていない場合
#define _EXPLOSION_H_		// 二重インクルード防止用マクロを定義

#include "main.h"
#include "billboard.h"

//==========================================================
// アニメーションオブジェクト(2D)のクラス定義(派生クラス)
//==========================================================
class CExplosion : public CObjectBillboard
{
public:	// 誰でもアクセス可能

	CExplosion();	// コンストラクタ
	CExplosion(const D3DXVECTOR3 pos);	// コンストラクタ(オーバーロード)
	~CExplosion();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CExplosion *Create(const D3DXVECTOR3 pos);

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Controller(void);

	// メンバ変数
	int m_nCounterAnim;	//アニメーションカウンター
	int m_nPatternAnim;	//アニメーションパターンNo.
};

#endif
