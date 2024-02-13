//===============================================
//
// 車全般の管理処理(car_manager.h)
// Author : Ibuki Okusada
//
//===============================================
#ifndef _CARMANAGER_H_	// このマクロが定義されていない場合
#define _CARMANAGER_H_	// 二重インクルード防止用マクロを定義

// マクロ定義
#define MAX_CAR	(6)

// 前方宣言
class CCar;

//===============================================
// マネージャークラスの定義
//===============================================
class CCarManager
{
private:	// 自分だけアクセス可能な定義

	// 車線定義
	enum LOAD
	{
		LOAD_RIGHT = 0,	// 右車線
		LOAD_LEFT,		// 左車線
		LOAD_MAX
	};

	// 生成管理用構造体
	struct SPAWN
	{
		int nNum;		// 出現数
		float fCount;	// カウンター
		float fDef;		// 規定値
	};

public:	// 誰でもアクセス可能

	CCarManager();	// コンストラクタ
	~CCarManager();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	CCar *Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, bool *pJump);
	void SetRanking(void);

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Spawn(void);
	void Delete(void);
	bool BindList(CCar *pCar, LOAD load);
	void DetachList(CCar *pCar, LOAD load);

	// 静的メンバ変数委
	static const char *m_apFileName[MAX_CAR];	// 車の種類
	static const D3DXVECTOR2 m_aShadowSize[MAX_CAR];	// 車の種類ごとの影のサイズ

	// メンバ変数
	SPAWN m_aSpawn[LOAD_MAX];	// 生成情報

	// リスト構造管理変数
	CCar *m_apTop[LOAD_MAX];	// 先頭
	CCar *m_apCur[LOAD_MAX];	// 最後尾
};

#endif
