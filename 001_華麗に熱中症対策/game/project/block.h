//==========================================================
//
// ブロックの処理 [block.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _BLOCK_H_		// このマクロが定義されていない場合
#define _BLOCK_H_		// 二重インクルード防止用マクロを定義

#include "main.h"
#include "object2D.h"

#define BLOCK_PRIORITY	(2)

//==========================================================
// ブロックのクラス定義(派生クラス)
//==========================================================
class CBlock : public CObject2D
{
public:	// 誰でもアクセス可能

	//CBlock();	// コンストラクタ
	CBlock(const D3DXVECTOR3 pos, const int nPriority = BLOCK_PRIORITY);	// コンストラクタ(オーバーロード)
	CBlock(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const int nPriority = BLOCK_PRIORITY);	// コンストラクタ(オーバーロード)
	CBlock(const int nPriority = BLOCK_PRIORITY);
	~CBlock();	// デストラクタ

	// メンバ関数
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static CBlock *Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, int nWidthTex = 1, int nHeightTex = 1);
	static HRESULT Load(void);
	static void Unload(void);

	// メンバ関数(設定)
	void SetMove(const D3DXVECTOR3 move) { m_move = move; }

	// メンバ関数(取得)
	D3DXVECTOR3 GetMove(void) { return m_move; }

private:	// 自分だけがアクセス可能

	// メンバ関数
	void Controller(void);

	// メンバ変数
	static const char *apTextureFileName[];	// ファイル名
	D3DXVECTOR3 m_move;		// 移動量
	int m_nIdxTexture;	// テクスチャ番号
	int m_nMoveCnt;
	int m_nWidthTex;	// 横幅枚数
	int m_nHeightTex;	// 縦幅枚数
};

#endif
