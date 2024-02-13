//==========================================================
//
// ゴール [goal.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GOAL_H_		// このマクロが定義されていない場合
#define _GOAL_H_		// 二重インクルード防止用マクロを定義

#include "task.h"	// これでファイルインクルードできます

// 前方宣言
class CMeshField;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CGoal : public CTask
{
public:	// 誰でもアクセス可能

	CGoal();	// コンストラクタ(オーバーロード)
	~CGoal();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CGoal *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot, float fWidth);
	static bool Collision(const D3DXVECTOR3& pos, const D3DXVECTOR3& posOld);
	void ListOut(void);

	// メンバ関数(取得)
	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_rot; }

	// メンバ関数(設定)
	void SetPosition(const D3DXVECTOR3& pos) { m_pos = pos; }
	void SetRotation(const D3DXVECTOR3& rot) { m_rot = rot; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	bool CollisionCheck(const D3DXVECTOR3& pos, const D3DXVECTOR3& posOld);

	// メンバ変数
	static CGoal *m_pTop;	// 先頭のオブジェクトへのポインタ
	static CGoal *m_pCur;	// 最後尾のオブジェクトへのポインタ
	CGoal *m_pPrev;		// 前のオブジェクトへのポインタ
	CGoal *m_pNext;		// 次のオブジェクトへのポインタ
	CMeshField *m_pObject;	// メッシュフィールドを使用
	D3DXVECTOR3 m_pos;	// 位置
	D3DXVECTOR3 m_rot;	// 向き
};

#endif
