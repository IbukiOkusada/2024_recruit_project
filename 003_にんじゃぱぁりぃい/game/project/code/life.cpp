//==========================================================
//
// ライフ [life.cpp]
// Author : Yuuka Sakuma
//
//==========================================================
#include "life.h"
#include "object2D.h"
#include "manager.h"
#include "texture.h"

// マクロ定義

//==========================================================
// コンストラクタ
//==========================================================
CLife::CLife()
{
	// 値のクリア
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_life = 0;
}

//==========================================================
// デストラクタ
//==========================================================
CLife::~CLife()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CLife::Init(void)
{

	for (int nCnt = 0; nCnt < m_life; nCnt++)
	{
		if (m_pObject[nCnt] == NULL)
		{
			m_pObject[nCnt] = CObject2D::Create(6);

			if (nCnt % 2 == 0)
			{
				m_pObject[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\heartL_0.png"));
				m_pObject[nCnt]->SetPosition(D3DXVECTOR3(m_pos.x + (20.0f * nCnt), m_pos.y, m_pos.z));
				m_pObject[nCnt]->SetLength(25.0f, 50.0f);
			}
			else if (nCnt % 2 != 0)
			{
				m_pObject[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\heartR_0.png"));
				m_pObject[nCnt]->SetPosition(D3DXVECTOR3(m_pos.x + (20.0f * nCnt), m_pos.y, m_pos.z));
				m_pObject[nCnt]->SetLength(25.0f, 50.0f);
			}
		}
	}

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CLife::Uninit(void)
{
	for (int nCnt = 0; nCnt < NUM_LIFE; nCnt++)
	{
		if (nullptr != m_pObject[nCnt]) {
			m_pObject[nCnt]->Uninit();
			m_pObject[nCnt] = NULL;
		}
	}

	Release();
}

//==========================================================
// 更新処理
//==========================================================
void CLife::Update(void)
{
}

//==========================================================
// 生成
//==========================================================
CLife *CLife::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CLife *pLife = nullptr;

	pLife = new CLife;

	if (pLife != nullptr)
	{
		pLife->SetPosition(pos);

		pLife->SetRotation(rot);

		// 初期化処理
		pLife->Init();
	}

	return pLife;
}

//==========================================================
// 設定処理
//==========================================================
void CLife::SetLife(int nLife)
{
	m_life = nLife;

	for (int nCnt = 0; nCnt < NUM_LIFE; nCnt++)
	{
		if (m_life >= NUM_LIFE)
		{
			if (m_pObject[nCnt] == NULL)
			{
				m_pObject[nCnt] = CObject2D::Create(7);

				if (nCnt % 2 == 0)
				{
					m_pObject[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\heartL_0.png"));
					m_pObject[nCnt]->SetPosition(D3DXVECTOR3(m_pos.x + (13.0f * nCnt), m_pos.y, m_pos.z));
					m_pObject[nCnt]->SetLength(15.0f, 40.0f);
				}
				else if (nCnt % 2 != 0)
				{
					m_pObject[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->Regist("data\\TEXTURE\\heartR_0.png"));
					m_pObject[nCnt]->SetPosition(D3DXVECTOR3(m_pos.x + (13.0f * nCnt), m_pos.y, m_pos.z));
					m_pObject[nCnt]->SetLength(15.0f, 40.0f);
				}
			}
		}
		else if (m_life < NUM_LIFE)
		{
			if (nullptr != m_pObject[NUM_LIFE - (NUM_LIFE - m_life)]) 
			{
				m_pObject[NUM_LIFE - (NUM_LIFE - m_life)]->Uninit();
				m_pObject[NUM_LIFE - (NUM_LIFE - m_life)] = NULL;
			}
		}
	}
}