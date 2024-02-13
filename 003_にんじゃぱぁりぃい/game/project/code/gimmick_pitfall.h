//==========================================================
//
// 落とし穴 [gimmick_pitfall.h]
// Author : Soma Ishihara
//
//==========================================================
#ifndef _PITFALL_H_		// このマクロが定義されていない場合
#define _PITFALL_H_		// 二重インクルード防止用マクロを定義

#include "gimmick.h"

//前方宣言
class CModel;
class CGimmickButton;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CGimmickPitFall : public CGimmick
{
private:
	enum FLOOR
	{
		FLOOR_LEFT = 0,
		FLOOR_RIGHT,
		FLOOR_MAX
	};

public:	// 誰でもアクセス可能

	CGimmickPitFall();	// コンストラクタ(オーバーロード)
	~CGimmickPitFall();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CGimmickPitFall *Create(const D3DXVECTOR3 pos);
	virtual void IdSetButton(void) override;
	bool CollisionCheckCloss(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3* posCollisioned) { return false; }

	// メンバ関数(取得)

	// メンバ関数(設定)
	void IsOpen(const bool bOpen) { m_bOpen = bOpen; }
	void BindButton(CGimmickButton *pButton) { m_pButton = pButton; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand = nullptr);

	// メンバ変数
	CModel* m_apModel[FLOOR_MAX];		//穴の床
	CGimmickButton *m_pButton;		// 連動するボタン
	float m_fAngle;				//床の開き具合
	bool m_bOpen;				//穴を開いているか
};

#endif

