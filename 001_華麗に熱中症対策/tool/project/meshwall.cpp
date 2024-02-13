//==========================================================
//
// メッシュフィールドの処理 [meshfield.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "meshwall.h"
#include "texture.h"
#include "manager.h"

// マクロ定義
#define TEXTUREFILE_DATA	"data\\TEXTURE\\welltile.png"		//テクスチャデータ

//==========================================================
// コンストラクタ
//==========================================================
CMeshWall::CMeshWall()
{

}

//==========================================================
// デストラクタ
//==========================================================
CMeshWall::~CMeshWall()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CMeshWall::Init(void)
{
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CMeshWall::Uninit(void)
{
	//頂点バッファの廃棄
	CObjectMesh::Uninit();
}

//==========================================================
// 更新処理
//==========================================================
void CMeshWall::Update(void)
{

}

//==========================================================
// 描画処理
//==========================================================
void CMeshWall::Draw(void)
{
	// 描画
	CObjectMesh::Draw();
}

//==========================================================
// 頂点情報設定
//==========================================================
void CMeshWall::SetVtxInfo(void)
{
	int nVertex = GetVertex();			// 頂点数を取得
	int nNumWidth = GetNumWidth();		// 幅枚数を取得
	int nNumHeight = GetNumHeight();	// 高さ枚数を取得

	//頂点座標の設定(左奥から右手前に向かって頂点情報を設定する
	for (int nCntpVtx = 0; nCntpVtx < nVertex; nCntpVtx++)
	{
		//頂点座標
		m_pVtx[nCntpVtx].pos.x = -(m_fWidth * nNumWidth) + (nCntpVtx % (nNumWidth + 1) * (m_fWidth * 2));
		m_pVtx[nCntpVtx].pos.y = ((m_fHeight * 2) * nNumHeight) + ((nCntpVtx / (nNumWidth + 1) * (-m_fHeight * 2)));
		m_pVtx[nCntpVtx].pos.z = 0.0f;

		//法線
		m_pVtx[nCntpVtx].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		//色
		m_pVtx[nCntpVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		m_pVtx[nCntpVtx].tex = D3DXVECTOR2(1.0f * (nCntpVtx % (nNumWidth + 1)), 1.0f * (nCntpVtx / (nNumWidth + 1)));
	}

	// 頂点設定
	SetVtx();
}

//==========================================================
// 生成
//==========================================================
CMeshWall *CMeshWall::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot, float fWidth, float fHeight,
	const char *pFileName, const int nWidth, const int nHeight, const int nPriority )
{
	CMeshWall *pMeshWall = NULL;	// メッシュフィールドのポインタ
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// メモリの確保
	pMeshWall = new CMeshWall;

	if (pMeshWall != NULL)
	{// 確保できた場合
		if (pMeshWall->GetId() < 0)
		{// オブジェクト側で管理できていない場合
			delete pMeshWall;
			pMeshWall = NULL;

			return NULL;
		}

		// 初期化
		pMeshWall->Init();

		// 座標設定
		pMeshWall->SetPosition(pos);

		// 向き設定
		pMeshWall->SetRotation(rot);

		// 頂点生成
		pMeshWall->CObjectMesh::Create(nWidth, nHeight);

		// サイズ設定
		pMeshWall->SetSize(fWidth, fHeight);

		// テクスチャ設定
		pMeshWall->BindTexture(pTexture->Regist(pFileName));
	}

	return pMeshWall;
}


//==========================================================
// 幅設定
//==========================================================
void CMeshWall::SetSize(float fWidth, float fHeight)
{
	// サイズ設定
	m_fWidth = fWidth;
	m_fHeight = fHeight;

	// 頂点情報設定
	SetVtxInfo();
}
