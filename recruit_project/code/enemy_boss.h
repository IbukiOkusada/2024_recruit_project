//==========================================================
//
// ボス敵 [enemy_boss.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _ENEMY_BOSS_H_		// このマクロが定義されていない場合
#define _ENEMY_BOSS_H_		// 二重インクルード防止用マクロを定義

#include "enemy.h"	// これでファイルインクルードできます

class CWaist;
class CCharacter;
class CPlayer;
class CBridge;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CEnemyBoss : public CEnemy
{
public:

	// 追加パーツ列挙型
	enum PARTS {
		PARTS_LEFTARM,	// 左腕
		PARTS_RIGHTARM,	// 右腕
		PARTS_MAX
	};

private:

	// 状態列挙型
	enum STATE
	{
		STATE_NONE = 0,		// 何もない状態
		STATE_APPEAR,		// 出現状態
		STATE_NORMAL,		// 通常状態
		STATE_LOCKON,		// ロックオン状態
		STATE_DAMAGE,		// ダメージ状態
		STATE_DEATH,		// 死亡状態
		STATE_MAX
	};

	// アクション列挙型
	enum ACTION {
		ACTION_NEUTRAL = 0,		// 待機
		ACTION_WALK,			// 歩行
		ACTION_ATK,				// 攻撃
		ACTION_DAMAGE,			// ダメージ
		ACTION_DEATH,			// 死亡
		ACTION_MAX
	};

	// 腕アクション列挙型
	enum ARM {
		ARM_NEUTRAL = 0,	// 待機
		ARM_ATTACK,			// 攻撃
		ARM_MAX
	};

	// 追跡対象情報構造体
	struct SChase
	{
		CPlayer* pTarget;	// 標的
		float fLength;		// 標的との距離
	};

	// 状態管理用構造体
	struct SStateInfo
	{
		STATE state;	// 状態
		float fCounter;	// 管理カウンター
	};

public:	// 誰でもアクセス可能

	CEnemyBoss();	// コンストラクタ(オーバーロード)
	~CEnemyBoss();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CEnemyBoss *Create(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot);

	// メンバ関数
	bool Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage) override;

	// メンバ関数(取得)

	// メンバ関数(設定)

private:	// 自分だけがアクセス可能

	// メンバ関数
	void BodySet(void);
	bool BodyCheck(CCharacter* pBody);
	void Damage(const int nDamage);
	CPlayer *Search(float &fChaseLength);
	void LockOn(void);
	void MethodLine(void);
	void SetState(void);
	void SetMotion(void);
	void Gravity(void);
	void AttackCheck(void);
	void Attack(const int nRandRange = 3);
	void FootCheck(void);
	void ArmDamage(void);

	// メンバ変数
	CWaist* m_pWaist;		// 腰
	CCharacter* m_apArm[PARTS_MAX];	// 腕
	int m_nArmAction;
	int m_NowArm;			// 現在動作する腕
	CCharacter* m_pBody;	// 上半身
	CCharacter* m_pLeg;		// 下半身
	SChase m_Chase;			// 追跡管理
	SStateInfo m_StateInfo;	// 状態管理
	int m_nAction;			// アクション番号
	float m_fInterVal;		// ダメージインターバル
	float m_fAtkCnter;		// 攻撃カウンター
	CBridge* m_pBridge;		// 橋
};

#endif
