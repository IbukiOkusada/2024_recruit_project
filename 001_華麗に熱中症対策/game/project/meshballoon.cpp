//==========================================================
//
// メッシュの水風船処理 [meshballoon.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "meshballoon.h"
#include "texture.h"
#include "manager.h"
#include "renderer.h"
#include "slow.h"
#include "game.h"
#include "meshfield.h"
#include "bullet.h"
#include "wet.h"
#include "particle.h"
#include "sound.h"

// マクロ定義
#define TEXTUREFILE_DATA	"data\\TEXTURE\\balloon001.jpg"		//テクスチャデータ
#define GRAVITY	(-0.1f)		// 重力
#define SWELL_CNT	(5)		// 膨らむカウント
#define SWELL_MOVE	(20.0f)	// 膨らむ量
#define SHRINK_CNT	(2)		// 縮むカウント
#define SHRINK_MOVE	(-3.0f)	// 縮む量
#define NUM_EXPLOWATER	(20)	// 水しぶき数
#define SIZEUP_WIDTH	(0.05f)	// 幅
#define MIN_SIZE	(60.0f)	// 最低サイズ

//==========================================================
// コンストラクタ
//==========================================================
CMeshBalloon::CMeshBalloon() : CObjectMesh(6)
{
	m_pMtxParent = NULL;
	m_fHeight = 0.0f;
	m_fHeightRot = 0.0f;
	m_fLength = 0.0f;
	m_fRot = 0.0f;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_fStateCnt = 0.0f;
	m_State = STATE_NONE;
}

//==========================================================
// デストラクタ
//==========================================================
CMeshBalloon::~CMeshBalloon()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CMeshBalloon::Init(void)
{
	m_State = STATE_NONE;
	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CMeshBalloon::Uninit(void)
{
	//頂点バッファの廃棄
	CObjectMesh::Uninit();
}

//==========================================================
// 更新処理
//==========================================================
void CMeshBalloon::Update(void)
{
	if (m_pMtxParent != NULL)
	{// 親が存在している場合
		return;
	}

	// 移動
	D3DXVECTOR3 pos = GetPosition();
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float fHeight = CMeshField::GetHeight(GetPosition());
	m_move.y += GRAVITY * CManager::GetSlow()->Get();
	pos += m_move * CManager::GetSlow()->Get();

	if (pos.y < fHeight + m_fLength)
	{// 地面に触れた
		pos.y = fHeight + m_fLength;
		m_move.y *= -0.6f;
		m_move.x *= 0.5f;
		m_move.z *= 0.5f;
		CManager::GetSound()->Play(CSound::LABEL_SE_BALLOONLAND);

		if (m_State == STATE_NONE)
		{// 破裂前の場合
			m_State = STATE_SHRINK;
			m_fStateCnt = SWELL_CNT;
		}
	}

	SetPosition(pos);

	// 状態ごとの更新
	UpdateState();
}

//==========================================================
// 描画処理
//==========================================================
void CMeshBalloon::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;		//デバイスへのポインタ

	//デバイスの取得
	pDevice = CManager::GetRenderer()->GetDevice();

	//ライティングをオフにする
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if (m_pMtxParent == NULL)
	{// 親が存在しない
		// 描画
		CObjectMesh::Draw();
	}
	else
	{// 親が存在する
		// マトリックス設定
		SetParentMtx();
	}

	//ライティングをオンにする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
}

//==========================================================
// 頂点情報設定
//==========================================================
void CMeshBalloon::SetVtxInfo(void)
{
	int nVertex = GetVertex();			// 頂点数を取得
	int nNumWidth = GetNumWidth();		// 幅枚数を取得
	int nNumHeight = GetNumHeight();	// 高さ枚数を取得
	D3DXVECTOR3 pos = GetPosition();	// 座標
	D3DXVECTOR3 vecDir;	//設定変更用ベクトル

	//頂点座標の設定(左奥から右手前に向かって頂点情報を設定する
	for (int nCntVtx = 0; nCntVtx < nVertex; nCntVtx++)
	{
		float fRot = m_fRot * (nCntVtx % (nNumWidth + 1));

		if (fRot > D3DX_PI || fRot < -D3DX_PI)
		{//-3.14～3.14の範囲外の場合
			if (fRot > D3DX_PI)
			{
				fRot += (-D3DX_PI * 2);
			}
			else if (fRot < -D3DX_PI)
			{
				fRot += (D3DX_PI * 2);
			}
		}

		float fHeightRot = m_fHeightRot * (nCntVtx / (nNumWidth + 1));

		if (fHeightRot > D3DX_PI || fHeightRot < -D3DX_PI)
		{//-3.14～3.14の範囲外の場合
			if (fHeightRot > D3DX_PI)
			{
				fHeightRot += (-D3DX_PI * 2);
			}
			else if (fHeightRot < -D3DX_PI)
			{
				fHeightRot += (D3DX_PI * 2);
			}
		}

		//頂点座標
		m_pVtx[nCntVtx].pos.x = (cosf(fHeightRot) * cosf(fRot)) * m_fLength;
		m_pVtx[nCntVtx].pos.y = sinf(fHeightRot) * m_fLength;
		m_pVtx[nCntVtx].pos.z = (cosf(fHeightRot) * sinf(fRot)) * m_fLength;

		// ベクトルを求める
		vecDir = pos - m_pVtx[nCntVtx].pos;

		//ベクトルを正規化する
		D3DXVec3Normalize(&vecDir, &vecDir);

		//法線
		m_pVtx[nCntVtx].nor = vecDir;

		//色
		m_pVtx[nCntVtx].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		m_pVtx[nCntVtx].tex = D3DXVECTOR2((1.0f / nNumWidth) * (nCntVtx % (nNumWidth + 1)), 1.0f - ((1.0f / nNumHeight) * (nCntVtx / (nNumWidth + 1))));
	}

	// 頂点設定
	SetVtx();
}

//==========================================================
// 生成
//==========================================================
CMeshBalloon *CMeshBalloon::Create(const D3DXVECTOR3 pos, const D3DXVECTOR3 rot,
	float fLength, float fHeight, const int nPriority, const int nWidth, const int nHeight)
{
	CMeshBalloon *pMeshWall = NULL;	// メッシュフィールドのポインタ
	CTexture *pTexture = CManager::GetTexture();	// テクスチャへのポインタ

	// メモリの確保
	pMeshWall = new CMeshBalloon;

	if (pMeshWall != NULL)
	{// 確保できた場合

		// 初期化
		pMeshWall->Init();

		// 座標設定
		pMeshWall->SetPosition(pos);

		// 向き設定
		pMeshWall->SetRotation(rot);

		// 頂点生成
		pMeshWall->CObjectMesh::Create(nWidth, nHeight);

		// 一頂点辺りの角度を求める
		pMeshWall->m_fRot = (-D3DX_PI * 2) / nWidth;

		// 縦方向の角度を求める
		pMeshWall->m_fHeightRot = (-D3DX_PI * 2) / (nHeight);

		// サイズ設定
		pMeshWall->SetSize(fLength, fHeight);

		// テクスチャ設定
		pMeshWall->BindTexture(pTexture->Regist(TEXTUREFILE_DATA));
	}

	return pMeshWall;
}

//==========================================================
// 幅設定
//==========================================================
void CMeshBalloon::SetSize(float fLength, float fHeight)
{
	// サイズ設定
	m_fLength = fLength;
	m_fHeight = fHeight;

	// 頂点情報設定
	SetVtxInfo();
}

//==========================================================
// 半径加算
//==========================================================
void CMeshBalloon::AddLength(float fAddLength)
{
	// サイズ設定
	m_fLength += fAddLength;

	if (m_fLength < 0.0f)
	{
		m_fLength = 0.0f;
	}

	// 頂点情報設定
	SetVtxInfo();
}

//==========================================================
// 親子関係の存在するマトリックス設定
//==========================================================
void CMeshBalloon::SetParentMtx(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();	//デバイスへのポインタを取得
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス
	D3DXMATRIX mtxParent;			// 親のマトリックス情報
	CSlow *pSlow = CManager::GetSlow();
	D3DXVECTOR3 pos = GetPosition(), rot = GetRotation();
	D3DXMATRIX *pMtx = GetMtx();

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(pMtx);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(pMtx, pMtx, &mtxTrans);

	if (m_pMtxParent != NULL)
	{// 覚えている場合
		mtxParent = *m_pMtxParent;

		//パーツのマトリックスと親のマトリックスをかけ合わせる
		D3DXMatrixMultiply(pMtx,
			pMtx, &mtxParent);
	}

	// マトリックス設定
	SetMtx(*pMtx);

	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, pMtx);

	// 描画
	NotMtxDraw();
}

//==========================================================
// 状態ごとの更新処理
//==========================================================
void CMeshBalloon::UpdateState(void)
{
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	switch (m_State)
	{
	case STATE_NONE:

		break;

	case STATE_SHRINK:

		AddLength(SHRINK_MOVE * CManager::GetSlow()->Get());
		m_fStateCnt -= CManager::GetSlow()->Get();

		if (m_fStateCnt <= 0.0f)
		{
			m_State = STATE_SWELL;	// 爆発状態に変更
			m_fStateCnt = SWELL_CNT;
		}

		break;

	case STATE_SWELL:

		AddLength(SWELL_MOVE * CManager::GetSlow()->Get());
		m_fStateCnt -= CManager::GetSlow()->Get();

		if (m_fStateCnt <= 0.0f)
		{
			// 弾の発射
			CBullet *pBullet = NULL;
			pBullet = CBullet::Create(GetPosition(),
				D3DXVECTOR3(0.0f, 2.0f, 0.0f), CBullet::TYPE_GRAVITY);

			pBullet->SetSize(m_fLength * 5, 150.0f);
			pBullet->SetLife(300.0f);

			float fMultiRot = 1.0f / NUM_EXPLOWATER;
			int nAddPar = (int)((m_fLength - MIN_SIZE) * SIZEUP_WIDTH);
			int nAddWater = nAddPar * NUM_EXPLOWATER;

			//水しぶきの生成
			for (int nCnt = 0; nCnt < nAddWater; nCnt++)
			{
				// 座標の設定
				D3DXVECTOR3 pos = GetPosition();
				D3DXVECTOR3 move;
				float fRand = (float)(rand() % 628 - 314) * 0.01f;

				//移動量の設定
				move.x = (-cosf(-D3DX_PI + D3DX_PI * 2 * (fMultiRot * nCnt))) * (rand() % 15);
				move.y = rand() % 100 * 0.11f;
				move.z = (-sinf(-D3DX_PI + D3DX_PI * 2 * (fMultiRot * nCnt))) * (rand() % 15);

				pBullet = CBullet::Create(pos,
					move, CBullet::TYPE_EXPLOSION);
				pBullet->SetLife(300.0f);
			}

			float fHeight = CMeshField::GetHeight(GetPosition());

			// 濡れ跡の生成
			CWet::Create(D3DXVECTOR3(GetPosition().x, fHeight + 0.05f, GetPosition().z), m_fLength, m_fLength);
			CWet::Create(D3DXVECTOR3(GetPosition().x, fHeight + 0.05f, GetPosition().z), m_fLength, m_fLength);

			// パーティクルの生成
			for (int nCnt = 0; nCnt < nAddPar + 1; nCnt++)
			{
				CParticle::Create(D3DXVECTOR3(GetPosition().x, fHeight, GetPosition().z), CEffect::TYPE_BALEXPLOSION);
			}

			CParticle::Create(D3DXVECTOR3(GetPosition().x, fHeight + 0.05f, GetPosition().z), CEffect::TYPE_SMAKE);
			CManager::GetSound()->Play(CSound::LABEL_SE_BALLOONSPLASH);

			// 終了処理
			Uninit();
		}

		break;
	}
}