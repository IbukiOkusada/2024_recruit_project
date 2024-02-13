//==========================================================
//
// 敵マネージャ [enemymanager.h]
// Author : Soma Ishihara
//
//==========================================================
#ifndef _ENEMY_MANAGER_H_		// このマクロが定義されていない場合
#define _ENEMY_MANAGER_H_		// 二重インクルード防止用マクロを定義

#include "task.h"

//==========================================================
// サンプルのクラス定義
//==========================================================
class CEnemyManager : public CTask
{

public:	// 誰でもアクセス可能

	CEnemyManager();	// コンストラクタ(オーバーロード)
	~CEnemyManager();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CEnemyManager *Create(void);

	// メンバ関数(取得)

	// メンバ関数(設定)

private:	// 自分だけがアクセス可能

	// メンバ関数

	// メンバ変数
	int m_nCounterSpawn;	//スポーンカウンター
};

#endif

