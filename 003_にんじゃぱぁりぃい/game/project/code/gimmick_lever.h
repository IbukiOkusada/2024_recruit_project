//==========================================================
//
// ギミックレバー [gimmick_lever.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_LEVER_H_		// このマクロが定義されていない場合
#define _GIMMICK_LEVER_H_		// 二重インクルード防止用マクロを定義

#include "gimmick.h"			// これでファイルインクルードできます

class CModel;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CGimmickLever : public CGimmick
{
private:

	// 使用モデル列挙型
	enum MODEL
	{
		MODEL_FRAME = 0,	// 枠組み
		MODEL_BUTTON,		// ボタン
		MODEL_MAX
	};

public:

	// 状態列挙型
	enum STATE
	{
		STATE_NONE = 0,	// 何もない
		STATE_PRESS,		// 押されている
		STATE_MAX
	};

public:	// 誰でもアクセス可能

	CGimmickLever();		// コンストラクタ(オーバーロード)
	~CGimmickLever();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CGimmickLever *Create(const D3DXVECTOR3 pos);
	void Switch(bool bUse) override;
	bool CollisionCheckCloss(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3* posCollisioned) { return false; }

	// メンバ関数(取得)
	STATE GetState(void) { return m_state; }

	// メンバ関数(設定)

private:	// 自分だけがアクセス可能

	// メンバ関数
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand = nullptr);

	// メンバ変数
	CModel *m_apObj[MODEL_MAX];			// ボタン側
	STATE m_state;						// 状態
	bool m_bUpDown;						// 上げ下げ
	D3DXVECTOR3 m_RotDest;					// 目標の角度
	static char *m_apFileName[MODEL_MAX];	// モデルファイルネーム
	int m_nInterval;						// インターバル
};

#endif
