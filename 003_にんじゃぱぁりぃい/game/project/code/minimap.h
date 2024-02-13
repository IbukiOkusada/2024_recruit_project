//===============================================
//
// ミニマップの処理 [minimap.h]
// Author : Soma Ishihara
//
//===============================================
#ifndef _MINIMAP_H_
#define _MINIMAP_H_

#include "task.h"
#include "object2DMap.h"
#include "input.h"

//前方宣言
class CMultiCamera;
class CObject2D;
class CObject2DMap;
class CObject2DScroll;

//人数別ミニマップ配置
namespace PlacePos
{
	struct SPlaceData
	{
		D3DXVECTOR3 pos;
		float fWidth, fHeight;
	};
	const SPlaceData NUM_PLACE_DATA[4] = 
	{
		{ D3DXVECTOR3(1120.0f, 80.0f, 0.0f), 246.0f, 100.0f },	//ONE_PLAYER
		{ D3DXVECTOR3(640.0f, 640.0f, 0.0f), 246.0f, 100.0f },	//TWO_PLAYER
		{ D3DXVECTOR3(960.0f, 540.0f, 0.0f), 492.0f, 200.0f },	//THREE_PLAYER
		{ D3DXVECTOR3(640.0f, 360.0f, 0.0f), 246.0f, 100.0f }	//FOUR_PLAYER
	};
}

//ミニマップクラス
class CMiniMap : public CTask
{
public:
	//頂点変更種類列挙（複数使用時AND演算して入れて）
	enum CHANGE
	{
		CHANGE_POS = 0b0001,
		CHANGE_RHW = 0b0010,
		CHANGE_COL = 0b0100,
		CHANGE_TEX = 0b1000,
		CHANGE_ALL = 0b1111
	};

	//コンストラクタ・デストラクタ
	CMiniMap();
	~CMiniMap();

	//基本処理
	HRESULT Init(void);
	void Uninit(void);
	void Update(void){}
	void DrawTexture(void);
	void DrawManual(void);	//3人プレイ用単体描画
	void ExploredMap(void);
	void Load(void);		//デバイスロスト対策：ロスト解消したら呼び出してね。初期化にも使える。
	void UnLoad(void);	//デバイスロスト対策：ロストしたら呼び出してね。終了にも使える。
	void Reset(void);	//まっくろくろすけにして探索してないことにする。初期化にどうぞ

	static CMiniMap* Create(const int playerNum, const int elaseWidth, const int elaseHeight);

	//設定・取得処理
	void SetPosition(const D3DXVECTOR3 pos) { m_pos = pos; }
	void SetGoalIcon(const bool bDisp);

	D3DXVECTOR3 GetPosition(void) { return m_pos; }
	D3DXVECTOR3 GetRotation(void) { return m_pObjMap->GetRotation(); }
	float GetWidth(void) { return m_fWidth; }
	float GetHeight(void) { return m_fHeight; }
	int GetPlayerNum(void) { return m_nPlayerNum; }

private:
	//メンバ関数
	void SetExit(void);

	// メンバ変数
	LPDIRECT3DTEXTURE9 m_pTextureMap;	// テクスチャ
	LPDIRECT3DTEXTURE9 m_pTextureUnex;	// 未探索場所を黒くしたテクスチャ
	LPDIRECT3DSURFACE9 m_pZSurface;		// テクスチャの深度バッファ
	CObject2DScroll* m_pObjScroll;		// 巻物背景オブジェクト
	CObject2DMap* m_pObjMap;			//マップオブジェ
	CObject2D** m_ppPlayerIcon;
	CObject2D* m_pExitIcon;
	bool** m_ppExplored;//探索済みドット
	D3DXVECTOR3 m_pos;	// 位置
	D3DXMATRIX m_mtxProj, m_mtxView, m_mtxViewPort;	//初期生成・更新共通で使うマトリ
	float m_fWidth;		// 幅
	float m_fHeight;	// 高さ
	int m_nElaseWidth;	//探索済みにする幅
	int m_nElaseHeight;	//探索済みにする高さ
	int m_nPlayerNum;	//プレイヤー人数分
};

#endif // !_MINIMAP_H_
