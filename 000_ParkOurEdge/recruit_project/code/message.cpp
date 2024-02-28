//==========================================================
//
// ���b�Z�[�W�\���̏��� [message.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "message.h"
#include "billboard.h"
#include "manager.h"
#include "slow.h"
#include "texture.h"

// �������O���
namespace
{
	const float ALPHA_MINUS = (0.015f);
	const float MOVEY = (2.0f);
	const float SETLIFE = (120.0f);
	const float WIDTH = (120.0f);
	const float HEIGHT = (30.0f);
	const char* TEXTUREFILENAME = "data\\TEXTURE\\checkpoint000.png";
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CMessage::CMessage()
{
	// �l�̃N���A
	m_pos = { 0.0f, 0.0f, 0.0f };
	m_pObj = nullptr;
	m_fLife = 0.0f;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CMessage::~CMessage()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CMessage::Init(void)
{
	m_pObj = CObjectBillboard::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5);
	m_pObj->SetSize(WIDTH, HEIGHT);
	m_pObj->SetCol(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	m_pObj->BindTexture(CManager::GetInstance()->GetTexture()->Regist(TEXTUREFILENAME));
	m_pObj->SetAlphaText(true);
	m_pObj->SetFusion(CObjectBillboard::FUSION_NORMAL);
	m_fLife = SETLIFE;
	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CMessage::Uninit(void)
{
	// �`��I�u�W�F�N�g�̏I��
	if (m_pObj != nullptr) {
		m_pObj->Uninit();
		m_pObj = nullptr;
	}

	Release();
}

//==========================================================
// �X�V����
//==========================================================
void CMessage::Update(void)
{
	if (m_pObj == nullptr) { // �g�p����Ă��Ȃ�
		Uninit();
		return;
	}

	// �̗͂����炷
	float fSlow = CManager::GetInstance()->GetSlow()->Get();
	m_fLife -= fSlow;

	if (m_fLife <= 0.0f) {	// �̗͂��Ȃ��Ȃ���
		Uninit();
		return;
	}

	// �F�̕ύX
	{
		D3DXCOLOR col = m_pObj->GetCol();
		col.a -= ALPHA_MINUS * fSlow;
		m_pObj->SetCol(col);
	}

	// �ړ�
	{
		D3DXVECTOR3 pos = m_pObj->GetPosition();
		pos.y += MOVEY * fSlow;
		m_pObj->SetPosition(pos);
	}
}

//==========================================================
// ����
//==========================================================
CMessage *CMessage::Create(const D3DXVECTOR3& pos)
{
	CMessage *pMessage = nullptr;

	pMessage = new CMessage;

	if (pMessage != nullptr)
	{
		// ����������
		pMessage->Init();

		pMessage->SetPosition(pos);
	}

	return pMessage;
}

//==========================================================
// ���W�ݒ�
//==========================================================
void CMessage::SetPosition(const D3DXVECTOR3& pos)
{
	m_pos = pos;

	// �g�p����Ă��Ȃ�
	if(m_pObj == nullptr){ return; }

	m_pObj->SetPosition(pos);
}