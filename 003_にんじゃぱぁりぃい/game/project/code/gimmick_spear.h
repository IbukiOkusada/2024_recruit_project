//==========================================================
//
// ギミック生える槍 [gimmick_spear.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_SPEAR_H_		// このマクロが定義されていない場合
#define _GIMMICK_SPEAR_H_		// 二重インクルード防止用マクロを定義

#include "gimmick.h"		// これでファイルインクルードできます

class CModel;
class CGimmickButton;
class CMeshCylinder;

//==========================================================
// 生える槍のクラス定義
//==========================================================
class CGimmickSpear : public CGimmick
{
private:

public:

	// 種類
	enum TYPE
	{
		TYPE_AUTO = 0,	// 自動作動
		TYPE_PRESS,		// 作動式
		TYPE_NOTPRESS,	// 押していないとき作動
		TYPE_PRESSAUTO,	// 作動した後自動式
		TYPE_SENSOR,		// センサー式
		TYPE_MAX
	};

	// 状態列挙型
	enum STATE
	{
		STATE_NONE = 0,	// 何もない
		STATE_APPEAR,		// 出現
		STATE_ATKNOW,		// 攻撃中
		STATE_BACK,		// 戻る
		STATE_MAX
	};

public:	// 誰でもアクセス可能

	CGimmickSpear();		// コンストラクタ(オーバーロード)
	~CGimmickSpear();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CGimmickSpear *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, TYPE type);
	virtual void IdSetButton(void) override;
	bool CollisionCheckCloss(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3* posCollisioned) { return false; }

	// メンバ関数(取得)
	STATE GetState(void) { return m_state; }
	CModel *GetModel(void) { return m_pObj; }
	TYPE GetType(void) { return m_type; }

	// メンバ関数(設定)
	void BindType(TYPE type) { m_type = type; }
	void BindButton(CGimmickButton *pGimmick) { m_pInObj = pGimmick; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand = nullptr);
	void StateSet(void);
	void UpdateType(void);

	// メンバ変数
	CModel *m_pObj;			// オブジェクトのポインタ
	STATE m_state;			// 状態
	TYPE m_type;				// 種類
	D3DXVECTOR3 m_PosDest;		// 目標の座標
	CGimmickButton *m_pInObj;	// 入力オブジェクト
	bool m_bActive;			// 動作可能かどうか
	bool m_bParticle;			// パーティクル使用したかどうか
	int m_nStateCounter;		// 状態管理カウンター
	float m_fDiffMulti;		// 差分補正の倍率
	static char *m_pFileName;	// モデルファイルネーム
	static int m_aStateSetCount[STATE_MAX];	// 遷移カウント
	static float m_aStateDiffMulti[STATE_MAX];	// 遷移カウント
};

#endif
