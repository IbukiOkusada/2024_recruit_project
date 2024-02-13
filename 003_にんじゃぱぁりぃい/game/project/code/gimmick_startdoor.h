//==========================================================
//
// ギミック開始地点ドア [gimmick_startdoor.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_STARTDOOR_H_		// このマクロが定義されていない場合
#define _GIMMICK_STARTDOOR_H_		// 二重インクルード防止用マクロを定義

#include "gimmick.h"			// これでファイルインクルードできます

class CModel;
class CGimmickLever;
class CEffect;

// エフェクトの名前空間
namespace DOOR_EFFECT
{
	const int NUM_EFFECT = (20);
}

//==========================================================
// 開始地点ドアのクラス定義
//==========================================================
class CGimmickStartDoor : public CGimmick
{
private:

public:

	// 状態列挙型
	enum STATE
	{
		STATE_NONE = 0,	// 何もない
		STATE_OPEN,		// 押されている
		STATE_MAX
	};

public:	// 誰でもアクセス可能

	CGimmickStartDoor();		// コンストラクタ(オーバーロード)
	~CGimmickStartDoor();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CGimmickStartDoor *Create(const D3DXVECTOR3 pos);
	bool CollisionCheckCloss(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3* posCollisioned) { return false; }

	// メンバ関数(取得)
	STATE GetState(void) { return m_state; }

	// メンバ関数(設定)
	void SetLever(CGimmickLever *pLever) { m_pLever = pLever; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand = nullptr);

	// メンバ変数
	CModel *m_pObj;			// オブジェクトのポインタ
	STATE m_state;			// 状態
	D3DXVECTOR3 m_PosDest;		// 目標の角度
	CGimmickLever *m_pLever;	// 同期されるレバーのポインタ
	CEffect *m_apEffect[DOOR_EFFECT::NUM_EFFECT];
	static char *m_pFileName;	// モデルファイルネーム
	int m_nSoundDown;
	int m_nSoundUp;
};

#endif
