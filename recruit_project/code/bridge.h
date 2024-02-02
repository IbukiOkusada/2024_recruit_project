//==========================================================
//
// 倒れてくる橋 [bridge.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _BRIDGE_H_		// このマクロが定義されていない場合
#define _BRIDGE_H_		// 二重インクルード防止用マクロを定義

#include "task.h"	// これでファイルインクルードできます

class CObjectX;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CBridge : public CTask
{

public:	// 誰でもアクセス可能

	CBridge();	// コンストラクタ(オーバーロード)
	~CBridge();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void SetClose(bool bValue) { m_bClose = bValue; }
	static CBridge *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

	// メンバ関数(取得)

	// メンバ関数(設定)

private:	// 自分だけがアクセス可能

	// メンバ関数
	void SetData(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);

	// メンバ変数
	CObjectX* m_pObj;
	bool m_bClose;
};

#endif
