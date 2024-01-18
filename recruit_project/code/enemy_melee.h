//==========================================================
//
// 近接攻撃の敵 [enemy_melee.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _ENEMY_MELEE_H_		// このマクロが定義されていない場合
#define _ENEMY_MELEE_H_		// 二重インクルード防止用マクロを定義

#include "enemy.h"	// これでファイルインクルードできます

class CWaist;
class CCharacter;

//==========================================================
// サンプルのクラス定義
//==========================================================
class CEnemyMelee : public CEnemy
{

public:	// 誰でもアクセス可能

	CEnemyMelee();	// コンストラクタ(オーバーロード)
	~CEnemyMelee();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CEnemyMelee *Create(void);

	// メンバ関数
	void Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage) override;

	// メンバ関数(取得)

	// メンバ関数(設定)

private:	// 自分だけがアクセス可能

	// メンバ関数
	void BodySet(void);
	bool BodyCheck(CCharacter* pBody);
	void AttackCheck(void);
	void Damage(const int nDamage);

	// メンバ変数
	CWaist* m_pWaist;		// 腰
	CCharacter* m_pBody;	// 上半身
	CCharacter* m_pLeg;		// 下半身
	int m_nAction;			// アクション番号
	int m_nInterVal;
};

#endif
