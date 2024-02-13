//==========================================================
//
// ギミック引けるもの [gimmick_rotatedoor.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_PULL_H_		// このマクロが定義されていない場合
#define _GIMMICK_PULL_H_		// 二重インクルード防止用マクロを定義

#include "gimmick.h"		// これでファイルインクルードできます

class CModel;

//==========================================================
// 回転ドアのクラス定義
//==========================================================
class CGimmickPull : public CGimmick
{
private:

public:

	// 状態列挙型
	enum STATE
	{
		STATE_NONE = 0,	// 何もない
		STATE_PULL,		// 引き中
		STATE_MAX
	};

public:	// 誰でもアクセス可能

	CGimmickPull();		// コンストラクタ(オーバーロード)
	~CGimmickPull();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CGimmickPull *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot);
	bool CollisionCheckCloss(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3* posCollisioned) { return false; }

	// メンバ関数(取得)
	STATE GetState(void) { return m_state; }
	CModel *GetModel(void) { return m_pObj; }
	virtual CGimmickPull *GetPull(void) override { return this; }

	// メンバ関数(設定)
	virtual void SetMtxParent(D3DXMATRIX *pMtx) override { m_pMtxParent = pMtx; }

private:	// 自分だけがアクセス可能



	// メンバ関数
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand = nullptr);
	void SetParentMatrix(void);

	// メンバ変数
	D3DXMATRIX *m_pMtxParent;	// 親のマトリックス
	D3DXVECTOR3 m_posOld;		// 前回の位置
	D3DXVECTOR3 m_move;		// 移動量
	CModel *m_pObj;			// オブジェクトのポインタ
	STATE m_state;			// 状態
	D3DXVECTOR3 m_RotDest;		// 目標の角度
	static char *m_pFileName;	// モデルファイルネーム
	int m_nParticleTimer;
};

#endif
