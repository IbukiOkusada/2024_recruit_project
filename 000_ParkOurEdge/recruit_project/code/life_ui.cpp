//==========================================================
//
// �̗�UI�Ǘ� [life_ui.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "life_ui.h"
#include "object2D.h"
#include "life.h"
#include "texture.h"
#include "manager.h"
#include "object_manager.h"

// ���O���
namespace {
	const char* TEXTURENAME = "data\\TEXTURE\\lifeframe000.png";	// �e�N�X�`���t�@�C����
	const float FRAME_WIDTH = (200.0f);
	const float FRAME_HEIGHT = (40.0f);
	const char* TEXTUREFONTNAME = "data\\TEXTURE\\lifefont.png";	
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CLifeUI::CLifeUI()
{
	// �l�̃N���A
	m_ppObj = nullptr;
	m_pFrame = nullptr;
	m_pFont = nullptr;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CLifeUI::~CLifeUI()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CLifeUI::Init(void)
{
	m_pFrame = CObject2D::Create(NUM_PRIORITY);
	m_pFrame->BindTexture(CManager::GetInstance()->GetTexture()->Regist(TEXTURENAME));
	m_pFont = CObject2D::Create(NUM_PRIORITY);
	m_pFont->BindTexture(CManager::GetInstance()->GetTexture()->Regist(TEXTUREFONTNAME));

	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CLifeUI::Uninit(void)
{
	if (m_pFrame != nullptr) {
		m_pFrame->Uninit();
		m_pFrame = nullptr;
	}

	if (m_pFont != nullptr) {
		m_pFont->Uninit();
		m_pFont = nullptr;
	}

	if (m_ppObj != nullptr) {
		for (int i = 0; i < m_nMaxLife; i++) {
			if (m_ppObj[i] == nullptr) {
				continue;
			}
			m_ppObj[i]->Uninit();
			m_ppObj[i] = nullptr;
		}

		delete[] m_ppObj;
		m_ppObj = nullptr;
	}

	Release();
}

//==========================================================
// �X�V����
//==========================================================
void CLifeUI::Update(void)
{

}

//==========================================================
// ����
//==========================================================
CLifeUI *CLifeUI::Create(const D3DXVECTOR3& pos, const int nMax)
{
	CLifeUI *pLifeUI = nullptr;

	pLifeUI = new CLifeUI;

	if (pLifeUI != nullptr) {
		// ����������
		pLifeUI->Init();

		// ���W�ݒ�
		pLifeUI->SetPosition(pos);
		pLifeUI->SetMaxLife(nMax);
	}

	return pLifeUI;
}

//==========================================================
// ���W�ݒ�
//==========================================================
void CLifeUI::SetPosition(const D3DXVECTOR3& pos)
{
	m_pos = pos;
	if (m_pFrame != nullptr) {
		m_pFrame->SetPosition(pos);
		m_pFrame->SetSize(FRAME_WIDTH, FRAME_HEIGHT);
	}

	if (m_pFont != nullptr) {
		D3DXVECTOR3 FontPos = pos;
		float fWidth = FRAME_WIDTH * 0.35f;
		float fHeight = FRAME_HEIGHT * 0.5f;
		FontPos.y -= FRAME_HEIGHT + fHeight;
		FontPos.x -= FRAME_WIDTH - fWidth;
		m_pFont->SetPosition(FontPos);
		m_pFont->SetSize(fWidth, fHeight);
	}
}

//==========================================================
// �ő�̗͐ݒ�
//==========================================================
void CLifeUI::SetMaxLife(const int nMax)
{
	if (m_ppObj != nullptr) {
		for (int i = 0; i < m_nMaxLife; i++) {
			if (m_ppObj[i] == nullptr) {
				continue;
			}
			m_ppObj[i]->Uninit();
			m_ppObj[i] = nullptr;
		}

		delete[] m_ppObj;
		m_ppObj = nullptr;
	}

	m_nMaxLife = nMax;
	m_ppObj = new CLife*[m_nMaxLife];

	float fWidth = ((FRAME_WIDTH * 0.95f) / static_cast<float>(m_nMaxLife));
	float fHeight = FRAME_HEIGHT * 0.9f;

	for (int i = 0; i < m_nMaxLife; i++) {
		D3DXVECTOR3 pos = m_pos;
		pos.x += (i * fWidth * 2.0f) - (FRAME_WIDTH * 0.85f);
		m_ppObj[i] = CLife::Create(pos, fWidth, fHeight);
	}
}

//==========================================================
// �̗͐ݒ�
//==========================================================
void CLifeUI::SetLife(const int nNum)
{
	if (m_ppObj == nullptr) {	// �g�p����Ă��Ȃ��ꍇ
		return;
	}

	float fWidth = ((FRAME_WIDTH * 0.95f) / static_cast<float>(m_nMaxLife));
	float fHeight = FRAME_HEIGHT * 0.9f;

	// ���݂̗̑͂Ɣ�r���Đ����폜��ݒ肷��
	for (int i = 0; i < m_nMaxLife; i++) {
		if (i >= nNum) {	// �ݒ肷��l�Ɠ���������ȏ�
			if(m_ppObj[i] != nullptr){	// �g�p����Ă���
				m_ppObj[i]->Lost();
				m_ppObj[i] = nullptr;
			}
		}
		else {	// ����ȉ�
			if (m_ppObj[i] == nullptr) {	// �g�p����Ă��Ȃ�
				D3DXVECTOR3 pos = m_pos;
				pos.x += (i * fWidth * 2.0f) - (FRAME_WIDTH * 0.85f);
				m_ppObj[i] = CLife::Create(pos, fWidth, fHeight);
			}
		}
	}
}