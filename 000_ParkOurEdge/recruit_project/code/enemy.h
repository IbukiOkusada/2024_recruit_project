//==========================================================
//
// 敵の処理 [enemy.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _ENEMY_H_		// このマクロが定義されていない場合
#define _ENEMY_H_		// 二重インクルード防止用マクロを定義

#include "task.h"	// これでファイルインクルードできます

//==========================================================
// サンプルのクラス定義
//==========================================================
class CEnemy : public CTask
{
protected:

	// 種類列挙型
	enum TYPE
	{
		TYPE_MELEE = 0,		// 近接型(近距離)
		TYPE_SHIELD,		// 盾持ち
		TYPE_GUN,			// 銃(遠距離)
		TYPE_BOSS,			// ボス
		TYPE_MAX
	};

protected:

	// 情報構造体
	struct SInfo
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		D3DXVECTOR3 move;		// 移動量
		D3DXVECTOR3 posOld;		// 前回の座標
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
	};

public:	// 誰でもアクセス可能

	CEnemy();	// コンストラクタ(オーバーロード)
	~CEnemy();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// メンバ関数(取得)
	CEnemy* GetNext(void) { return m_pNext; }
	CEnemy* GetPrev(void) { return m_pPrev; }
	D3DXVECTOR3 GetPosition(void) const { return m_Info.pos; }
	D3DXVECTOR3 GetRotation(void) const { return m_Info.rot; }
	D3DXVECTOR3 GetMove(void) const { return m_Info.move; }
	D3DXMATRIX* GetMtx(void) { return &m_Info.mtxWorld; }
	float GetRotDiff(void) { return m_fRotDiff; }
	float GetRotMove(void) { return m_fRotMove; }
	float GetBumpSize(void) { return m_fBumpSize; }
	bool GetLockon(void) { return m_bLockOn; }

	// メンバ関数(設定)
	void SetNext(CEnemy* pNext) { m_pNext = pNext; }
	void SetPrev(CEnemy* pPrev) { m_pPrev = pPrev; }
	void SetPosition(const D3DXVECTOR3& pos) { m_Info.pos = pos; }
	void SetRotation(const D3DXVECTOR3& rot) { m_Info.rot = rot; }
	void SetMove(const D3DXVECTOR3& move) { m_Info.move = move; }
	void SetRotDiff(const float fRotDiff) { m_fRotDiff = fRotDiff; }

	// メンバ関数
	virtual bool Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage);

protected:

	// メンバ変数
	TYPE GetType(void) { return m_type; }
	void SetType(TYPE type) { m_type = type; }
	SInfo* GetInfo(void) { return &m_Info; }
	void SetInfo(const SInfo& Info) { m_Info = Info; }
	void SetMatrix(void);
	int GetLife(void) { return m_nLife; }
	void SetLife(const int nLife) { m_nLife = nLife; }
	void InfoReset(void);
	void AddMove(void);
	void SetIner(const float fIner) { m_fMoveIner = fIner; }
	void SetRotMulti(const float fMulti) { m_fRotMulti = fMulti; }
	void SetLockOn(const bool bUse) { m_bLockOn = bUse; }
	void SetBumpSize(const float fSize) { m_fBumpSize = fSize; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Adjust(void);

	// メンバ変数
	CEnemy* m_pPrev;	// 前のオブジェクトへのポインタ
	CEnemy* m_pNext;	// 次のオブジェクトへのポインタ
	int m_nLife;		// 体力
	TYPE m_type;		// 種類
	SInfo m_Info;		// 情報
	float m_fRotMove;	// 現在の角度
	float m_fRotDiff;	// 目的の角度
	float m_fMoveIner;	// 移動量の慣性設定
	float m_fRotMulti;	// 向きの補正速度
	float m_fBumpSize;	// 衝突判定サイズ
	bool m_bLockOn;		// ロックオンできるかどうか
};

#endif
