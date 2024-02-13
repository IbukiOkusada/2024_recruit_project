//===============================================
//
// ブロックの処理 [block.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "block.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"

//===============================================
// 静的メンバ変数宣言(ファイル名)
//===============================================
const char *CBlock::apTextureFileName[] =
{
	"data\\TEXTURE\\block000.jpg",
};

//===============================================
// マクロ定義
//===============================================
#define VTX	(4)	// 頂点数
#define WIDTH	(30.0f)	// 幅
#define HEIGHT	(30.0f)	// 高さ

//===============================================
// コンストラクタ
//===============================================
//CBlock::CBlock()
//{
//	// 値をクリアする
//}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CBlock::CBlock(const D3DXVECTOR3 pos, const int nPriority) : CObject2D(pos)
{
	// 値をクリアする
	
}

//===============================================
// コンストラクタ(オーバーロード)
//===============================================
CBlock::CBlock(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, const int nPriority) : CObject2D(pos, rot, nPriority)
{
	// 規定値を入れる
	m_move = move;
}

CBlock::CBlock(const int nPriority) : CObject2D(nPriority)
{
	m_nMoveCnt = 0;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
}

//===============================================
// デストラクタ
//===============================================
CBlock::~CBlock()
{

}

//===============================================
// 初期化処理
//===============================================
HRESULT CBlock::Init(void)
{
	// オブジェクトの初期化処理
	CObject2D::Init();

	return S_OK;
}

//===============================================
// 終了処理
//===============================================
void CBlock::Uninit(void)
{
	// オブジェクトの終了処理
	CObject2D::Uninit();
}

//===============================================
// 更新処理
//===============================================
void CBlock::Update(void)
{
	// プレイヤー操作
	Controller();

	m_nMoveCnt++;

	if (m_nMoveCnt > 180)
	{
		m_nMoveCnt = 0;
		m_move *= -1.0f;
	}

	D3DXVECTOR3 pos = GetPosition();

	pos += m_move;

	SetPosition(pos);

	// 頂点情報更新
	SetSize(WIDTH, HEIGHT);
}

//===============================================
// 描画処理
//===============================================
void CBlock::Draw(void)
{
	CObject2D::Draw();
}

//===============================================
// 頂点情報設定
//===============================================
CBlock *CBlock::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, const D3DXVECTOR3 move, int nWidthTex, int nHeightTex)
{
	CBlock *pBlock = NULL;
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// オブジェクト2Dの生成
	pBlock = new CBlock();

	if (pBlock != NULL)
	{// 生成できた場合

		// 初期化処理
		pBlock->Init();

		// 座標設定
		pBlock->SetPosition(pos);

		// 向き設定
		pBlock->SetRotation(rot);

		// 移動量設定
		pBlock->SetMove(move);

		// 種類の設定
		pBlock->SetType(CObject::TYPE_BLOCK);

		pBlock->SetLength(WIDTH, HEIGHT);

		// テクスチャの割り当て
		pBlock->BindTexture(pTexture->Regist(apTextureFileName[0]));
	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pBlock;
}

//===============================================
// 操作処理
//===============================================
void CBlock::Controller(void)
{
	
}
