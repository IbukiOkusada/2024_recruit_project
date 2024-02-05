//==========================================================
//
// ���̏p [wave.cpp]
// Author : Ibuki Okusada
//
//==========================================================
#include "wave.h"
#include "player.h"
#include "enemy.h"
#include "manager.h"
#include "debugproc.h"
#include "meshcylinder.h"
#include "texture.h"
#include "particle.h"
#include "player_manager.h"

// �������O���
namespace {
	const float MAX_LENGTH = (500.0f);	// �ő�͈̔�
	const float RANGE_UPSPEED = (15.0f);	// 1�t���[���Ԃ͈̔͑�����
	const float FLYAWAY_SPEED = (100.0f);	// ������ԑ��x(���������ϐ���)
	const float ITEMAWAY_SPEED = (18.0f);	// �A�C�e���̐�����ԑ��x
	const float ITEMAWAY_JUMP = (15.0f);
	const float AIROBJ_HEIGHT = (40.0f);	// �I�u�W�F�N�g�̍���
	const int OBJ_NUMWIDTH = (10);		// ������
	const float FLYAWAY_JUMP = (20.0f);	// ������ԍ���
	const int PARTICLE_CT = (1);		// �p�[�e�B�N���̃J�E���^�[
	const char* FILENAME[CWave::TYPE_MAX] = {	// �e�N�X�`���t�@�C����
		"data\\TEXTURE\\wind000.png",
		"data\\TEXTURE\\wind001.png"
	};
}

//==========================================================
// �R���X�g���N�^
//==========================================================
CWave::CWave()
{
	// �l�̃N���A
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		m_apObject[nCnt] = nullptr;
	}
	m_Info.fRange = 0.0f;
	m_Info.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Info.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_Info.mtxWorld);
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CWave::~CWave()
{

}

//==========================================================
// ����������
//==========================================================
HRESULT CWave::Init(void)
{
	// �I�u�W�F�N�g�̐���
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		m_apObject[nCnt] = CMeshCylinder::Create(m_Info.pos, m_Info.rot, m_Info.fRange, AIROBJ_HEIGHT, 5, OBJ_NUMWIDTH);
		m_apObject[nCnt]->BindTexture(CManager::GetInstance()->GetTexture()->Regist(FILENAME[nCnt]));
	}

	return S_OK;
}

//==========================================================
// �I������
//==========================================================
void CWave::Uninit(void)
{
	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		if (m_apObject[nCnt] != nullptr) {
			m_apObject[nCnt]->Uninit();
			m_apObject[nCnt] = nullptr;
		}
	}

	Release();
}

//==========================================================
// �X�V����
//==========================================================
void CWave::Update(void)
{
	float fRate = m_Info.fRange / MAX_LENGTH;
	m_Info.fRange += RANGE_UPSPEED;

	if (m_Info.fRange > MAX_LENGTH) {	// �ő�͈͂𒴂���
		Uninit();
	}
	else
	{
		m_nParticleCounter--;

		// �p�[�e�B�N���̐���
		if (m_nParticleCounter <= 0) {
			float fRange = (rand() % 629 - 314) * 0.01f;
			D3DXVECTOR3 pos = D3DXVECTOR3(m_Info.pos.x + sinf(fRange) * m_Info.fRange, m_Info.pos.y, m_Info.pos.z + cosf(fRange) * m_Info.fRange);
			CParticle::Create(pos, CEffect::TYPE_AIR);
			m_nParticleCounter = PARTICLE_CT;
		}

		// �����蔻������
		Collision();
	}

	for (int nCnt = 0; nCnt < TYPE_MAX; nCnt++) {
		if (m_apObject[nCnt] != nullptr) {
			m_apObject[nCnt]->SetLength(m_Info.fRange);
			m_apObject[nCnt]->SetPosition(m_Info.pos);

			// ���݂̊��������߂�
			float fAdd = 0.1f;
			float fMulti = 0.15f;

			if (nCnt == TYPE_ANOTHER) {
				fAdd *= -1.0f;
				fMulti *= -1.0f;
			}

			D3DXVECTOR3 rot = m_apObject[nCnt]->GetRotation();
			rot.y += fAdd + (fRate * fMulti);
			m_apObject[nCnt]->SetRotation(rot);
			m_apObject[nCnt]->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f - (fRate)));
		}
	}
}

//==========================================================
// ����
//==========================================================
CWave *CWave::Create(const D3DXVECTOR3& pos, const int nId)
{
	CWave *pWave = nullptr;

	pWave = new CWave;

	if (pWave != nullptr)
	{
		// ����������
		pWave->Init();

		// ���W�̐ݒ�
		pWave->SetPosition(pos);

		// ID�̐ݒ�
		pWave->BindId(nId);
	}

	return pWave;
}

//==========================================================
// �����蔻��
//==========================================================
void CWave::Collision(void)
{
	// �v���C���[�Ƃ̔���
	{
		CPlayer *pPlayer = CPlayerManager::GetInstance()->GetTop();

		while (pPlayer != nullptr) 
		{
			CPlayer *pPlayerNext = pPlayer->GetNext();	// ����ێ�

			if (pPlayer->GetId() == m_nId) {	// �o�����{�l
				pPlayer = pPlayerNext;
				continue;
			}

			// ���������
			D3DXVECTOR3 ObjPos = pPlayer->GetPosition();
			float fLength = sqrtf((m_Info.pos.x - ObjPos.x) * (m_Info.pos.x - ObjPos.x)
				+ (m_Info.pos.z - ObjPos.z) * (m_Info.pos.z - ObjPos.z));

			if (fLength > m_Info.fRange) {	// ���͈͓̔��ł͂Ȃ��ꍇ
				pPlayer = pPlayerNext;
				continue;
			}

			D3DXVECTOR3 move = pPlayer->GetMove();
			float fRot = atan2f(ObjPos.x - m_Info.pos.x, ObjPos.z - m_Info.pos.z);	//�ڕW�܂ł̈ړ�����

			// �ړ�������ݒ�
			move.x = sinf(fRot) * FLYAWAY_SPEED;
			if (move.y <= 0.0f)
			{
				move.y = FLYAWAY_JUMP;
			}
			move.z = cosf(fRot) * FLYAWAY_SPEED;

			// �ړ��ʂ𔽉f
			pPlayer->SetMove(move);
			pPlayer = pPlayerNext;	// ���Ɉړ�
		}
	}
}