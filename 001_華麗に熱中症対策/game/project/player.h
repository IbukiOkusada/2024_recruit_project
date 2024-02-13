//==========================================================
//
// プレイヤーの処理 [player.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _PLAYER_H_		// このマクロが定義されていない場合
#define _PLAYER_H_		// 二重インクルード防止用マクロを定義

#include "main.h"
#include "object.h"
#include "meshballoon.h"

// 前方宣言
class CShadow;
class CCharacter;
class CWaist;
class CObject2D;
class CObjectBillboard;
class CModel;
class CLockOn;
class CMeshOrbit;
class CObject3D;
class CCar;

//==========================================================
// プレイヤーのクラス定義(派生クラス)
//==========================================================
class CPlayer : public CObject
{
private:	// 自分だけがアクセス可能な定義

	// 上半身モーション列挙
	typedef enum
	{
		BMOTION_NEUTRAL = 0,	// 待機モーション
		BMOTION_WALK,			// 歩行モーション
		BMOTION_JUMP,			// ジャンプモーション
		BMOTION_LAND,			// 着地モーション
		BMOTION_TWINATK,		// 二丁射撃モーション
		BMOTION_SHWATK,			// シャワー射撃モーション
		BMOTION_SLOWATK,		// スロー中二丁射撃モーション
		BMOTION_SLOWSHW,		// スロー中シャワー射撃モーション
		BMOTION_MAX
	}BMOTION;

	// 下半身モーション列挙
	typedef enum
	{
		LMOTION_NEUTRAL = 0,	// 待機モーション
		LMOTION_WALK,			// 歩行モーション
		LMOTION_JUMP,			// ジャンプモーション
		LMOTION_LAND,			// 着地モーション
		LMOTION_MAX
	}LMOTION;

	// 武器種類
	typedef enum
	{
		WEAPON_GUNL = 0,		// 二丁左
		WEAPON_GUNR,			// 二丁右
		WEAPON_SHOWER,			// シャワー
		WEAPON_MAX
	}WEAPON;

	// 攻撃方法
	typedef enum
	{
		ATK_GUN = 0,	// 二丁
		ATK_SHOWER,		// シャワー
		ATK_MAX
	}ATK;

	// 情報構造体
	typedef struct
	{
		D3DXVECTOR3 pos;		// 位置
		D3DXVECTOR3 rot;		// 向き
		D3DXVECTOR3 move;		// 移動量
		D3DXVECTOR3 posOld;		// 設定位置
		D3DXMATRIX mtxWorld;	// ワールドマトリックス
	}INFO;

public:	// 誰でもアクセス可能

	//CPlayer();	// コンストラクタ
	CPlayer(const D3DXVECTOR3 pos);	// コンストラクタ(オーバーロード)
	CPlayer(int nPriOrity = 1);
	~CPlayer();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	HRESULT Init(const char *pBodyName, const char *pLegName);	// オーバーロード
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CPlayer *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, 
	const char *pBodyName, const char *pLegName, const int nPriority = 4);

	// メンバ関数(設定)
	void SetMove(const D3DXVECTOR3 move) { m_Info.move = move; }
	void SetPosition(const D3DXVECTOR3 pos) { m_Info.pos = pos; }
	void SetRotation(const D3DXVECTOR3 rot) { m_Info.rot = rot; }
	void AddSlowTime(int nAddTime = 10);

	// メンバ関数(取得)
	D3DXVECTOR3 GetMove(void) { return m_Info.move; }
	D3DXVECTOR3 GetPosition(void) { return m_Info.pos; }
	D3DXVECTOR3 GetRotation(void) { return m_Info.rot; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Controller(void);
	void Attack(void);
	void SetBodyRot(void);
	void Move(void);
	void Jump(void);
	void Slow(void);
	void SlowShw(void);
	void SlowGun(void);
	void SetGageColor(float fRate);
	void Particle(void);
	void ShotBalloon(void);

	// メンバ変数
	CWaist *m_pWaist;		// 腰
	CCharacter *m_pBody;	// 上半身
	CCharacter *m_pLeg;		// 下半身
	CModel *m_pWeaponL;		// 武器左
	CModel *m_pWeaponR;		// 武器右
	CObject2D *m_pSlowGage;	// スローゲージ
	CObject2D *m_pSlowManual;	// スローゲージ
	CMeshOrbit *m_pOrbit;	// 軌跡
	CMeshBalloon *m_pBalloon;	// 風船のポインタ
	INFO m_Info;			// 自分自身の情報
	bool m_bJump;			// ジャンプしたかどうか
	bool m_bAttack;			// 攻撃しているかどうか
	float m_fAttackTimer;	// 攻撃タイマー
	int m_nAttackHand;		// 攻撃する手
	ATK m_WepType;			// 装備中武器種類
	ATK m_WepTypeOld;		// 変更前の武器種類
	bool m_bMove;			// 移動したかどうか
	int m_nSlowTime;		// スロー可能時間
	float m_fChargeCnt;		// チャージカウント
	bool m_bActiveSlow;		// スロー入力できるか否か
	bool m_bSlow;			// スロー状態か否か
	bool m_bLock;			// ロックオンしているか否か
	float m_fRotMove;		// 現在の角度
	float m_fRotDiff;		// 目的の角度
	float m_fRotDest;		// 角度計算
	float m_fTexU;
	CShadow *pShadow;		// 影
	CLockOn *m_pLockon;		// ロックオン
	CCar *m_pCar;
	int m_aWepNum[WEAPON_MAX];						// 武器ファイル読み込み番号
	static const D3DXVECTOR3 SetWepPos[WEAPON_MAX];	// 武器設定位置
	static const int m_aWepTimer[ATK_MAX];			// 攻撃タイマー	
	static const char *m_apFileName[WEAPON_MAX];	// 武器ファイル名
	static const CMeshBalloon::SET m_SetBalloon;	// 風船初期設定
	CObject3D *m_pMapIcon;	// マップアイコン
	float m_fMove;
};

#endif
