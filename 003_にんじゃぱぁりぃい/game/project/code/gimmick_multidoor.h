//==========================================================
//
// ギミック協力ドア [gimmick_multidoor.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _GIMMICK_MULTIDOOR_H_	// このマクロが定義されていない場合
#define _GIMMICK_MULTIDOOR_H_	// 二重インクルード防止用マクロを定義

#include "gimmick.h"	// これでファイルインクルードできます

class CModel;
class CGimmickButton;

//==========================================================
// 生える槍のクラス定義
//==========================================================
class CGimmickMultiDoor : public CGimmick
{
public:

	// 状態列挙型
	enum STATE {
		STATE_NEUTRAL = 0,	// 待機
		STATE_OPEN,			// 開く
		STATE_CLOSE,			// 閉じる
		STATE_MAX
	};

private:

	// 使用オブジェクト種類列挙
	enum TYPE 
	{
		TYPE_LEFT = 0,	// 左側
		TYPE_RIGHT,		// 右側
		TYPE_MAX
	};

	// オブジェクト情報構造体
	struct SObj {
		CModel *pModel;		// オブジェクトのポインタ
		D3DXVECTOR3 posDest;	// 目標の座標
		D3DXVECTOR3 posOld;	// 前回の座標
	};

public:	// 誰でもアクセス可能

	CGimmickMultiDoor();		// コンストラクタ(オーバーロード)
	~CGimmickMultiDoor();		// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CGimmickMultiDoor *Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& rot);
	void BindButton(CGimmickButton *pButton);
	void SetNumButton(const int nNum);
	void SetActiveButton(const int nNum);
	virtual void IdSetButton(void) override;
	bool CollisionCheckCloss(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3* posCollisioned = nullptr);
	
	// メンバ関数(取得)

private:	// 自分だけがアクセス可能

	// メンバ関数
	bool CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 &SetPos, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, int nAction, CGimmick **ppGimmick, bool* bLand = nullptr);
	void StateSet(void);
	void UpdateState(void);
	void ObjIner(void);
	void SetButtonMaterial(int nPressCnt);

	// メンバ変数
	SObj m_aObject[TYPE_MAX];
	int m_nNumSwitch;					// 規定値のスイッチ数
	int m_nNumLinkSwitch;				// リンクしているスイッチ数
	int m_nActiveSwitch;				// 起動に必要なスイッチ数
	CGimmickButton **m_ppButton;		// アクセス入力装置
	STATE m_state;					// 状態
	int m_nStateCnt;					// 状態管理カウンター
	float m_fInerMulti;				// 補正倍率
	int m_nCount;
};

#endif


