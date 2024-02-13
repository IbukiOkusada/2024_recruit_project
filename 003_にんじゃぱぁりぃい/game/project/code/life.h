//==========================================================
//
// ライフ [life.h]
// Author : Yuuka Sakuma
//
//==========================================================
#ifndef _LIFE_H_		// このマクロが定義されていない場合
#define _LIFE_H_		// 二重インクルード防止用マクロを定義

#include "task.h"	// これでファイルインクルードできます


class CObject2D;
//==========================================================
// サンプルのクラス定義
//==========================================================
class CLife : public CTask
{

public:	// 誰でもアクセス可能

	CLife();	// コンストラクタ(オーバーロード)
	~CLife();	// デストラクタ

					// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CLife *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	// メンバ関数(取得)
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }
	int GetLife(void) { return m_life; }

	// メンバ関数(設定)
	void SetPosition(D3DXVECTOR3 pos) { m_pos = pos; }
	void SetRotation(D3DXVECTOR3 rot) { m_rot = rot; }
	void SetLife(int nlife);

private:	// 自分だけがアクセス可能

#define NUM_LIFE (6)	//オブジェクトの数
			// メンバ関数

			// メンバ変数
	CObject2D *m_pObject[NUM_LIFE];
	D3DXVECTOR3 m_pos;
	D3DXVECTOR3 m_rot;
	int m_life;

};

#endif


