//==========================================================
//
// ライトの処理全般 [light.h]
// Author : Ibuki Okusada
//
//==========================================================
#include "light.h"
#include "input.h"
#include "manager.h"
#include "renderer.h"

//==========================================================
// コンストラクタ
//==========================================================
CLight::CLight()
{
	
}

//==========================================================
// デストラクタ
//==========================================================
CLight::~CLight()
{

}

//==========================================================
// 初期化処理
//==========================================================
HRESULT CLight::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();		// デバイスへのポインタを取得
	D3DXVECTOR3 vecDir;		// 設定変更用ベクトル

	for (int nCntLight = 0; nCntLight < NUM_LIGHT; nCntLight++)
	{
		// ライトの情報をクリアする
		ZeroMemory(&m_aLight[nCntLight], sizeof(D3DLIGHT9));

		// ライトの種類を設定
		m_aLight[nCntLight].Type = D3DLIGHT_DIRECTIONAL;

		// ライトの拡散光を設定
		switch (nCntLight)
		{
		case 0:
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		case 1:
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.65f, 0.65f, 0.65f, 1.0f);
			break;
		case 2:
			m_aLight[nCntLight].Diffuse = D3DXCOLOR(0.15f, 0.15f, 0.15f, 1.0f);
			break;
		}

		// ライトの方向を設定
		switch (nCntLight)
		{
		case 0:
			//vecDir = D3DXVECTOR3(0.1f, -0.11f, 0.44f);
			//vecDir = D3DXVECTOR3(0.8f, 0.23f, -0.55f);
			vecDir = D3DXVECTOR3(0.22f, -0.87f, -0.44f);
			break;
		case 1:
			vecDir = D3DXVECTOR3(0.8f, 0.23f, -0.55f);
			break;
		case 2:
			vecDir = D3DXVECTOR3(0.89f, -0.11f, 0.44f);
			break;
		}

		D3DXVec3Normalize(&vecDir, &vecDir);	// ベクトルを正規化する
		m_aLight[nCntLight].Direction = vecDir;

		// ライトを設定する
		pDevice->SetLight(nCntLight, &m_aLight[nCntLight]);

		// ライトを有効化する
		pDevice->LightEnable(nCntLight, TRUE);
	}

	return S_OK;
}

//==========================================================
// 終了処理
//==========================================================
void CLight::Uninit(void)
{

}

//==========================================================
// 更新処理
//==========================================================
void CLight::Update(void)
{

}