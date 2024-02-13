//==========================================================
//
// マップオブジェクト [object2DMap.h]
// Author : Soma Ishihara
//
//==========================================================
#ifndef _OBJECT_MAP_H_		// このマクロが定義されていない場合
#define _OBJECT_MAP_H_		// 二重インクルード防止用マクロを定義

#include "object2D.h"

//==========================================================
// マップのクラス定義
//==========================================================
class CObject2DMap : public CObject2D
{

public:	// 誰でもアクセス可能

	CObject2DMap(int nPriOrity = 3);	// コンストラクタ(オーバーロード)
	~CObject2DMap();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Draw(void);
	static CObject2DMap *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const float fWidth, const float fHeight);

	// メンバ関数(取得)
	

	// メンバ関数(設定)
	void SetTexture(LPDIRECT3DTEXTURE9 pTexMap, LPDIRECT3DTEXTURE9 pTexUnex);

private:	// 自分だけがアクセス可能

	// メンバ関数

	// メンバ変数
	LPDIRECT3DTEXTURE9 m_pTextureMap;	//マップテクスチャ
	LPDIRECT3DTEXTURE9 m_pTextureUnex;	//未探索テクスチャ
};

#endif

