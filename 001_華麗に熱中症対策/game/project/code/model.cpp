//==========================================================
//
// 階層構造のモデル処理 [model.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "model.h"
#include "Xfile.h"
#include "manager.h"
#include "texture.h"
#include "renderer.h"
#include "slow.h"

//==========================================================
// コンストラクタ
//==========================================================
CModel::CModel()
{
	// 値のクリア
	m_nIdxModel = -1;
	m_bChangeCol = false;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_ChangeMat.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_ChangeMat.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	m_ChangeMat.Emissive = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//m_pParent = NULL;
}

//==========================================================
// デストラクタ
//==========================================================
CModel::~CModel()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CModel::Init(const char *pFileName)
{
	CXFile *pModelFile = CManager::GetModelFile();	// モデルファイル情報のポインタ

	//Xファイル情報を読み込み
	m_nIdxModel = pModelFile->Regist(pFileName);

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CModel::Uninit(void)
{
	// ポインタを使用していない状態にする
	m_pParentMtx = NULL;
}

//==========================================================
// 更新処理
//==========================================================
void CModel::Update(void)
{

}

//==========================================================
// 描画処理
//==========================================================
void CModel::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスへのポインタを取得
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	CXFile *pModelFile = CManager::GetModelFile();	// Xファイル情報のポインタ
	D3DMATERIAL9 matDef;			//現在のマテリアル保存用
	D3DXMATERIAL *pMat;				//マテリアルデータへのポインタ
	D3DXMATRIX mtxParent;			// 親のマトリックス情報
	CSlow *pSlow = CManager::GetSlow();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_CurRot.y, m_CurRot.x, m_CurRot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_CurPos.x, m_CurPos.y, m_CurPos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	if(m_pParentMtx != NULL)
	{// 覚えている場合
		mtxParent = *m_pParentMtx;

		//パーツのマトリックスと親のマトリックスをかけ合わせる
		D3DXMatrixMultiply(&m_mtxWorld,
			&m_mtxWorld, &mtxParent);
	}

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);

	// モデル情報を取得
	CXFile::FileData *pFileData = pModelFile->SetAddress(m_nIdxModel);

	if (pFileData != NULL)
	{// 使用されている場合
	 //マテリアルデータへのポインタを取得
		pMat = (D3DXMATERIAL*)pFileData->pBuffMat->GetBufferPointer();
		for (int nCntMat = 0; nCntMat < (int)pFileData->dwNumMat; nCntMat++)
		{
			int nIdxTex = pFileData->pIdexTexture[nCntMat];	// テクスチャ番号

			if (!m_bChangeCol)
			{
				//マテリアルの設定
				pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
			}
			else
			{
				//マテリアルの設定
				if (pSlow->Get() != 1.0f)
				{
					m_ChangeMat.Emissive.r -= 0.3f;
					m_ChangeMat.Emissive.g -= 0.3f;
					m_ChangeMat.Emissive.b -= 0.3f;
					m_ChangeMat.Emissive.a -= 0.3f;
				}
				else
				{
					m_ChangeMat.Emissive = pMat[nCntMat].MatD3D.Emissive;
					m_ChangeMat.Emissive.r += 0.1f;
					m_ChangeMat.Emissive.g += 0.1f;
					m_ChangeMat.Emissive.b += 0.1f;
					m_ChangeMat.Emissive.a += 0.1f;
				}
				pDevice->SetMaterial(&m_ChangeMat);
			}

			//テクスチャの設定
			pDevice->SetTexture(0, pTexture->SetAddress(nIdxTex));

			//モデル(パーツ)の描画
			pFileData->pMesh->DrawSubset(nCntMat);
		}
	}

	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

//==========================================================
// 生成
//==========================================================
CModel *CModel::Create(const char *pFileName)
{
	CModel *pModel = NULL;
	CXFile *pModelFile = CManager::GetModelFile();

	// オブジェクトXの生成
	pModel = new CModel;

	if (pModel != NULL)
	{// 生成できた場合

		// 初期化処理
		pModel->Init(pFileName);

	}
	else
	{// 生成に失敗した場合
		return NULL;
	}

	return pModel;
}

//==========================================================
// 親モデルを設定
//==========================================================
void CModel::SetParent(D3DXMATRIX *pMtx)
{
	m_pParentMtx = pMtx;
}

//==========================================================
// 初期位置を設定
//==========================================================
void CModel::SetPosition(const D3DXVECTOR3 pos)
{
	m_pos = pos;

	SetCurrentPosition(m_pos);
}

//==========================================================
// 初期向きを設定
//==========================================================
void CModel::SetRotation(const D3DXVECTOR3 rot)
{
	m_rot = rot;

	SetCurrentRotation(m_rot);
}

//==========================================================
// 現在の位置を設定
//==========================================================
void CModel::SetCurrentPosition(const D3DXVECTOR3 pos)
{
	m_CurPos = pos;
}

//==========================================================
// 現在の向きを設定
//==========================================================
void CModel::SetCurrentRotation(const D3DXVECTOR3 rot)
{
	m_CurRot = rot;
}

//==========================================================
// モデル番号の設定
//==========================================================
void CModel::BindModelFile(int nIdx)
{
	m_nIdxModel = nIdx;
}

//==========================================================
// モデルカラー設定
//==========================================================
void CModel::SetChangeCol(const bool bChange, D3DXCOLOR col)
{
	m_bChangeCol = bChange;
	m_ChangeMat.Ambient = col;
	m_ChangeMat.Diffuse = col;
	m_ChangeMat.Emissive = col;
}