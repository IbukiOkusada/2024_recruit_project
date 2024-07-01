//===============================================
//
// 車全般の管理処理(car_manager.h)
// Author : Ibuki Okusada
//
//===============================================
#ifndef _CARMANAGER_H_	// このマクロが定義されていない場合
#define _CARMANAGER_H_	// 二重インクルード防止用マクロを定義

// 定数定義
namespace
{
	const int MAX_CAR = 6;	// 車の最大種類
}

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

	// 判定用構造体
	struct COLLISION
	{
		D3DXVECTOR3 pos;	// 座標
		D3DXVECTOR3 posOld;	// 前回の座標
		D3DXVECTOR3 vtxMax;	// 最大
		D3DXVECTOR3 vtxMin;	// 最小
		D3DXVECTOR3 move;	// 移動量

		COLLISION() : 
			pos(MYDEFAULT::VECTOR3), posOld(MYDEFAULT::VECTOR3), 
			vtxMax(MYDEFAULT::VECTOR3), vtxMin(MYDEFAULT::VECTOR3), 
			move(MYDEFAULT::VECTOR3) {}

		COLLISION(const D3DXVECTOR3& _pos, const D3DXVECTOR3& _posOld, 
			const D3DXVECTOR3& _vtxMax, const D3DXVECTOR3& _vtxMin, 
			const D3DXVECTOR3& _move) : pos(_pos), posOld(_posOld),
			vtxMax(_vtxMax), vtxMin(_vtxMin),
			move(_move) {}
	};

public:	// 誰でもアクセス可能

	CCarManager();	// コンストラクタ
	~CCarManager();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	CCar *Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, bool *pJump);
	bool CollisionCheck(COLLISION* car, COLLISION* target, const int nType);
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
