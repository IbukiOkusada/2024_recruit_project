//===============================================
//
// オブジェクト(マップの巻物)の処理 [object2D.h]
// Author : Soma Ishihara
//
//===============================================
#ifndef _OBJECT2D_SCROLL_H_		// このマクロが定義されていない場合
#define _OBJECT2D_SCROLL_H_		// 二重インクルード防止用マクロを定義

#include "main.h"
#include "object2D.h"

//==========================================================
// オブジェクト(マップの巻物)のクラス定義(派生クラス)
//==========================================================
class CObject2DScroll : public CObject2D
{
public:	// 誰でもアクセス可能

	CObject2DScroll();	//デフォルトコンストラクタ
	CObject2DScroll(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nPriority = 3);	// コンストラクタ
	virtual ~CObject2DScroll();	// デストラクタ
	
	// メンバ関数
	virtual HRESULT Init(void);
	virtual void Uninit(void);
	virtual void Update(void);
	virtual void Draw(void);
	static CObject2DScroll *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const int nPriority = 3);
};

#endif