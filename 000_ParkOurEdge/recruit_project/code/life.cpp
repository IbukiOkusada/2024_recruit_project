//==========================================================
//
// �̗�UI [life.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "life.h"
#include "object2D.h"
#include "object_manager.h"
#include "texture.h"
#include "manager.h"
#include "slow.h"

// ���O���
namespace {
	const char* TEXTURENAME = "data\\TEXTURE\\life.png";	// �e�N�X�`���t�@�C����
	const float MOVE = (-8.0f);
	const float GRAVITY = (0.55f);
	const float TIMER = (120.0f);
	const D3DXCOLOR DAMAGECOL = { 1.0f, 0.0f, 0.0f, 1.0f };
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CLife::CLife()
{
	// �l�̃N���A
	m_pObj = nullptr;
	m_bEnd = false;
	m_fMoveY = 0.0f;
	m_fTimer = 0.0f;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CLife::~CLife()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CLife::Init(void)
{
	m_pObj = CObject2D::Create(NUM_PRIORITY);
	m_pObj->BindTexture(CManager::GetInstance()->GetTexture()->Regist(TEXTURENAME));
	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CLife::Uninit(void)
{
	if (m_pObj != nullptr) {
		m_pObj->Uninit();
		m_pObj = nullptr;
	}

	Release();
}

//==========================================================
// �X�V����
//==========================================================
void CLife::Update(void)
{
	if (!m_bEnd) {
		return;
	}

	// ���ł܂ł̃J�E���g��i�߂�
	float fSlow = CManager::GetInstance()->GetSlow()->Get();
	m_fTimer -= fSlow;

	// 2D�|���S�����ړ�
	m_fMoveY += GRAVITY * fSlow;
	if (m_pObj != nullptr) {	// �g�p����Ă���
		// ���W�̈ړ�
		D3DXVECTOR3 pos = m_pObj->GetPosition();
		pos.y += m_fMoveY * fSlow;
		m_pObj->SetPosition(pos);
		m_pObj->SetSize(m_pObj->GetWidth(), m_pObj->GetHeight());

		// �F�̒���
		D3DXCOLOR col = m_pObj->GetCol();
		col.a = m_fTimer / TIMER;
		m_pObj->SetCol(col);
	}

	if (m_fTimer <= 0.0f) {	// �^�C�}�[���I��
		Uninit();
	}
}

//==========================================================
// ����
//==========================================================
CLife *CLife::Create(const D3DXVECTOR3& pos, const float fWidth, const float fHeight)
{
	CLife *pLife = nullptr;

	pLife = new CLife;

	if (pLife != nullptr)
	{
		// ����������
		pLife->Init();

		pLife->m_pObj->SetPosition(pos);
		pLife->m_pObj->SetSize(fWidth, fHeight);
	}

	return pLife;
}

//==========================================================
// ����
//==========================================================
void CLife::Lost(void)
{
	// �I�����Ă��Ȃ��Ƃ��̂�
	if (m_bEnd) {
		return;
	}

	m_bEnd = true;	// �t���O���Ă�
	m_fMoveY = MOVE;
	m_fTimer = TIMER;

	if (m_pObj != nullptr) {	// �g�p����Ă���
		m_pObj->SetCol(DAMAGECOL);
	}
}