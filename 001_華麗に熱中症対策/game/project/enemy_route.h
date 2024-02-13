//==========================================================
//
// 敵の順路管理処理 [enemy_route.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _ENEMY_ROUTE_H_
#define _ENEMY_ROUTE_H_

#define MAX_ROUTE	(64)	// 行動パターン最大数
#define ROUTE_NAME	(256)

//==========================================================
// 敵の出現管理クラスの定義(基底クラス)
//==========================================================
class CEnemyRoute
{
public:

	// 順路情報構造体
	struct Route
	{
		D3DXVECTOR3 StartPos;	// 開始地点
		D3DXVECTOR3 StartRot;	// 開始向き
		D3DXVECTOR3 *pPoint;	// チェックポイント
		float fMove;			// 移動速度
		int nNumPoint;	// ポイント総数
	};

private:

	// 保存情報用構造体
	struct RouteData
	{
		Route route;	// 順路情報
		char aName[ROUTE_NAME];	// 名前入力
	};

public:		// 誰でもアクセス可能

	CEnemyRoute();	// コンストラクタ
	~CEnemyRoute();	// デストラクタ

	HRESULT Init(void);
	void Uninit(void);
	Route *SetAddress(const char *pFileName);
	Route *SetAddress(const int nIdx);
	int Regist(const char *pFileName);
	bool Load(Route *pRoute, const char *pFileName);

private:	// 自分だけがアクセス可能

	RouteData *m_apRoute[MAX_ROUTE];	// ルートの最大数
	int m_nNumAll;	// 総数
};

#endif