//==========================================================
//
// 3D扇の処理 [object3DFan.cpp]
// Author : Soma Ishihara
//
//==========================================================
#include "object3DFan.h"
#include "manager.h"
#include "renderer.h"

//==========================================================
//コンストラクタ
//==========================================================
CObject3DFan::CObject3DFan(int nPriority) : CObject(nPriority)
{
	//各種変数の初期化
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pVtxBuff = nullptr;
	m_fLength = 0.0f;
	m_nDivision = 3;
}

//==========================================================
//デストラクタ
//==========================================================
CObject3DFan::~CObject3DFan()
{

}

//==========================================================
//ポリゴンの初期化処理
//==========================================================
HRESULT CObject3DFan::Init(void)
{
	//頂点情報設定
	ResetpVtx();

	return S_OK;
}

//==========================================================
//ポリゴンの終了処理
//==========================================================
void CObject3DFan::Uninit(void)
{
	//頂点バッファの廃棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// 廃棄
	Release();
}

//==========================================================
//ポリゴンの描画処理
//==========================================================
void CObject3DFan::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスへのポインタを取得

	//ワールドマトリックスの設定
	SetMtx();
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(
		0,
		m_pVtxBuff,
		0,
		sizeof(VERTEX_3D));

	//頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	//テクスチャの設定
	pDevice->SetTexture(0, nullptr);

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLEFAN,	//プリミティブの種類
		0,
		m_nDivision	//頂点情報構造体のサイズ
		);
}

//==========================================================
//生成処理
//==========================================================
CObject3DFan *CObject3DFan::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fLength, float fRadius, int nDivision)
{
	CObject3DFan *pObject3DFan = nullptr;

	// オブジェクト2Dの生成
	pObject3DFan = new CObject3DFan;

	if (pObject3DFan != nullptr)
	{// 生成できた場合
		// 初期化処理
		pObject3DFan->Init();
		pObject3DFan->SetPosition(pos);
		pObject3DFan->SetRotation(rot);
		pObject3DFan->SetLength(fLength);
		pObject3DFan->SetRadius(fRadius);
		pObject3DFan->SetDivision(nDivision);

		// 種類設定
		pObject3DFan->SetType(TYPE_NONE);
	}
	else
	{// 生成に失敗した場合
		return nullptr;
	}

	return pObject3DFan;
}

//==========================================================
// 頂点情報設定
//==========================================================
void CObject3DFan::SetpVtx(const int nChangeVtx)
{
	VERTEX_3D *pVtx;
	VERTEX_3D *pVtxStart;

	//頂点バッファをロックし頂点情報へのポインタを取得
	m_pVtxBuff->Lock(
		0,
		0,
		(void**)&pVtxStart,
		0);

	//頂点座標の設定
	if ((nChangeVtx & CHANGE_POS) == CHANGE_POS)
	{
		//先頭入れる
		pVtx = pVtxStart;
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx++;

		//その他入れる
		for (int cnt = 0; cnt < m_nDivision + 1; cnt++)
		{
			//計算
			float fRadiusOne = ((float)cnt / m_nDivision) * m_fRadius;
			D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			pos.x = sinf(fRadiusOne - (m_fRadius * 0.5f)) * m_fLength;
			pos.z = cosf(fRadiusOne - (m_fRadius * 0.5f)) * m_fLength;

			//設定
			pVtx[cnt].pos = pos;
		}
	}

	//法線ベクトルの設定
	if ((nChangeVtx & CHANGE_NOR) == CHANGE_NOR)
	{
		//先頭入れる
		pVtx = pVtxStart;
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		pVtx++;

		//その他入れる
		for (int cnt = 0; cnt < m_nDivision + 1; cnt++)
		{
			pVtx[cnt].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		}
	}

	//頂点カラーの設定
	if ((nChangeVtx & CHANGE_COL) == CHANGE_COL)
	{
		//先頭入れる
		pVtx = pVtxStart;
		pVtx[0].col = m_col;
		pVtx++;

		//その他入れる
		for (int cnt = 0; cnt < m_nDivision + 1; cnt++)
		{
			pVtx[cnt].col = m_col;
		}
	}

	//テクスチャ座標の設定
	if ((nChangeVtx & CHANGE_TEX) == CHANGE_TEX)
	{
		//先頭入れる
		pVtx = pVtxStart;
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx++;

		//その他入れる
		for (int cnt = 0; cnt < m_nDivision + 1; cnt++)
		{
			pVtx[cnt].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//==========================================================
// 長さ設定
//==========================================================
void CObject3DFan::SetLength(const float fLength)
{
	m_fLength = fLength;

	//頂点情報設定
	SetpVtx(CHANGE_POS);
}

//==========================================================
// 角度設定
//==========================================================
void CObject3DFan::SetRadius(const float fRadius)
{
	m_fRadius = fRadius;

	//頂点情報設定
	SetpVtx(CHANGE_POS);
}

//==========================================================
// 分割数設定（最低3～）
//==========================================================
void CObject3DFan::SetDivision(const int nDiv)
{
	m_nDivision = (nDiv > 3) ? nDiv : 3;	//最低3頂点以上にすること

	//頂点情報再生成
	ResetpVtx();
}

//==========================================================
// 色設定
//==========================================================
void CObject3DFan::SetColor(const D3DXCOLOR col)
{
	m_col = col;

	//頂点情報設定
	SetpVtx(CHANGE_COL);
}

//==========================================================
// マトリックス設定
//==========================================================
void CObject3DFan::SetMtx(void)
{
	D3DXMATRIX mtxRot, mtxTrans;	//計算用マトリックス

	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);

	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
}

//==========================================================
// 頂点情報バッファ込みで再設定
//==========================================================
HRESULT CObject3DFan::ResetpVtx(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//デバイスへのポインタを取得

	//頂点バッファの廃棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	//頂点バッファの生成
	if (FAILED(pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * (m_nDivision + 2),
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		nullptr)))
	{
		return E_FAIL;
	}

	//頂点バッファをロックし頂点情報へのポインタを取得
	SetpVtx(CHANGE_ALL);

	return S_OK;
}
