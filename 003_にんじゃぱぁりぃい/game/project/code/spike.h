//==========================================================
//
// まきびし [spike.h]
// Author : Soma Ishihara
//
//==========================================================
#ifndef _SPIKE_H_		// このマクロが定義されていない場合
#define _SPIKE_H_		// 二重インクルード防止用マクロを定義

#include "task.h"

//前方宣言
class CObjectX;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CSpike : public CTask
{
private:
	// 情報構造体
	struct SInfo
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		D3DXVECTOR3 move;		// 移動量
		D3DXVECTOR3 posOld;		// 設定位置
		int nLife;
	};

public:	// 誰でもアクセス可能

	CSpike();	// コンストラクタ(オーバーロード)
	~CSpike();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CSpike *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float moveHorizontal, const float moveVertical);

	// メンバ関数(取得)
	D3DXVECTOR3 GetPosition(void) { return m_info.pos; }
	D3DXVECTOR3 GetRotation(void) { return m_info.rot; }
	D3DXVECTOR3 GetMove(void) { return m_info.move; }
	D3DXVECTOR3 GetPositionOld(void) { return m_info.posOld; }
	int GetLife(void) { return m_info.nLife; }
	static CSpike* GetTop(void) { return m_pTop; }
	CSpike* GetNext(void) { return m_pNext; }

	// メンバ関数(設定)
	void SetPosition(const D3DXVECTOR3 pos) { m_info.pos = pos; }
	void SetRotation(const D3DXVECTOR3 rot) { m_info.rot = rot; }
	void SetMove(const D3DXVECTOR3 move) { m_info.move = move; }
	void SetPositionOld(const D3DXVECTOR3 pos) { m_info.posOld = pos; }
	void SetLife(const int nLife) { m_info.nLife = nLife; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void CollisionCloss(void);
	void Collision(void);

	// メンバ変数
	static CSpike *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CSpike *m_pCur;	// 最後尾のオブジェクトへのポインタ
	CSpike *m_pPrev;	// 前のオブジェクトへのポインタ
	CSpike *m_pNext;	// 次のオブジェクトへのポインタ
	CObjectX* m_pObj;	//オブジェクト
	SInfo m_info;		//情報
	static int m_nNumCount;
};

#endif

