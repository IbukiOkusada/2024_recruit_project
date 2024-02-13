//==========================================================
//
// 車の処理 [car.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _CAR_H_	// このマクロが定義されていない場合
#define _CAR_H_	// 二重インクルード防止用マクロを定義

#include "objectX.h"

// 前方宣言
class CShadow;

//==========================================================
// 車クラスの定義(派生クラス)
//==========================================================
class CCar : public CObjectX
{
public:	// 誰でもアクセス可能

	CCar();		// コンストラクタ
	~CCar();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CCar *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *pFileName, int nType = 0, const int nPriority = 3);

	// メンバ関数(設定)
	void SetMove(D3DXVECTOR3 move) { m_move = move; }
	void SetNext(CCar *pNext) { m_pNext = pNext; }
	void SetPrev(CCar *pPrev) { m_pPrev = pPrev; }
	void SetOldPos(D3DXVECTOR3 posOld) { m_posOld = posOld; }

	// メンバ関数(取得)
	D3DXVECTOR3 GetMove(void) { return m_move; }
	D3DXVECTOR3 GetOldPos(void) { return m_posOld; }
	CCar *GetNext(void) { return m_pNext; }
	CCar *GetPrev(void) { return m_pPrev; }
	CShadow *GetShadow(void) { return m_pShadow; }
	int GetType(void) { return m_nType; }

private:	// 自分だけがアクセス可能

	// メンバ変数
	D3DXVECTOR3 m_move;	// 移動量
	D3DXVECTOR3 m_posOld;	// 前回の座標
	CShadow *m_pShadow;	// 影
	CCar *m_pPrev;	// 前
	CCar *m_pNext;	// 次
	int m_nType;
};

#endif