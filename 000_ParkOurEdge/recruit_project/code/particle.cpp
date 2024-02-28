//===============================================
//
// �p�[�e�B�N���̏��� [particle.cpp]
// Author : Ibuki Okusada
//
//===============================================
#include "particle.h"
#include "manager.h"
#include "renderer.h"
#include "input.h"
#include "debugproc.h"
#include "texture.h"
#include "billboard.h"

//===============================================
// �}�N����`
//===============================================
#define LIFE	(30)		// ����

//===============================================
// �R���X�g���N�^
//===============================================
CParticle::CParticle()
{

}

//===============================================
// �f�X�g���N�^
//===============================================
CParticle::~CParticle()
{

}

//===============================================
// ����
//===============================================
void CParticle::Create(const D3DXVECTOR3& pos, CEffect::TYPE type)
{
	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	Set(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), type);
}

//===============================================
// ���_���ݒ�
//===============================================
void CParticle::Create(const D3DXVECTOR3& pos, const D3DXVECTOR3& move, CEffect::TYPE type)
{
	// �I�u�W�F�N�g�̎�ނ̐ݒ�
	Set(pos, move, type);
}

//===============================================
// �ݒ�
//===============================================
void CParticle::Set(const D3DXVECTOR3& Defpos, const D3DXVECTOR3& Defmove, CEffect::TYPE type)
{
	D3DXVECTOR3 pos = {};
	D3DXVECTOR3 move = {};	// �ړ���
	D3DXCOLOR col = {};	// �F
	CEffect::TYPE ParType = type;
	float fRadius = 0.0f;
	float fLife = 0;
	D3DXVECTOR3 nor;
	float fCol = (float)(rand() % 3) / 10.0f + 0.3f;

	// �ړ��x�N�g�������߂�
	D3DXVec3Normalize(&nor, &Defmove);	// �x�N�g���𐳋K������

	switch (type)
	{
	case CEffect::TYPE_NONE:	// �����Ȃ�

		break;

	case CEffect::TYPE_SMAKE:	// ��

		for (int nCnt = 0; nCnt < 20; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.09f;
			move.y = ((float)(rand() % 50)) * 0.15f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.09f;

			//�F�̐ݒ�
			col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

			//���a�̐ݒ�
			fRadius = 100.0f;

			//�����̐ݒ�
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}

		break;

	case CEffect::TYPE_ITEMGET:	// ��
	{
		for (int nCnt = 0; nCnt < 20; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.05f;
			move.y = ((float)(rand() % 50)) * 0.2f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.05f;

			//�F�̐ݒ�
			col = D3DXCOLOR(1.0f, rand() % 10 * 0.1f + 0.0f, 0.0f, 1.0f);

			//���a�̐ݒ�
			fRadius = 10.0f;

			//�����̐ݒ�
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}

	case CEffect::TYPE_LANDCLEAR:	// ��
	{
		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.05f;
			move.y = ((float)(rand() % 50)) * 0.2f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.05f;

			//�F�̐ݒ�
			col = D3DXCOLOR(0.3f, 0.3f, 0.1f, 1.0f);

			//���a�̐ݒ�
			fRadius = 10.0f;

			//�����̐ݒ�
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}

	case CEffect::TYPE_LANDFAILED:	// ��
	{
		for (int nCnt = 0; nCnt < 60; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.1f;
			move.y = 0.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.1f;

			//�F�̐ݒ�
			col = D3DXCOLOR(0.025f, 0.02f, 0.015f, 1.0f);

			//���a�̐ݒ�
			fRadius = 30.0f;

			//�����̐ݒ�
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_HIT:	// �q�b�g
	{
		for (int nCnt = 0; nCnt < 15; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.15f;
			move.y = sinf((float)(rand() % 629 - 314) * 0.01f) * 4.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.15f;

			//�F�̐ݒ�
			col = D3DXCOLOR(rand() % 5 * 0.1f + 0.6f, 0.3f, 0.3f, 1.0f);

			//���a�̐ݒ�
			fRadius = 7.0f;

			//�����̐ݒ�
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_BLACKSMAKE:	// ��
	{
		for (int nCnt = 0; nCnt < 25; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.09f;
			move.y = ((float)(rand() % 50)) * 0.15f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.09f;

			//�F�̐ݒ�
			col = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

			//���a�̐ݒ�
			fRadius = 100.0f;

			//�����̐ݒ�
			fLife = 30.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
		break;

	case CEffect::TYPE_WALK:	// ��
	{
		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.015f;
			move.y = rand() % 100 * 0.01f * 5.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.015f;

			//�F�̐ݒ�
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

			//���a�̐ݒ�
			fRadius = 17.0f;

			//�����̐ݒ�
			fLife = 250.0f + rand() % 50;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
		break;

	case CEffect::TYPE_KUNAI:	// ��
	{
		for (int nCnt = 0; nCnt < 10; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;
			move.y = ((float)(rand() % 50)) * 0.01f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;

			//�F�̐ݒ�
			float frand = rand() % 8 * 0.1f;
			col = D3DXCOLOR(0.1f, frand, frand + 0.1f, 0.32f);

			//���a�̐ݒ�
			fRadius = 30.0f;

			//�����̐ݒ�
			fLife = 30.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_AIR:	// ��
	{
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.1f;
			move.y = ((float)(rand() % 3 + 1)) * 2.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.1f;

			float frand = rand() % 2 * 0.1f;

			//�F�̐ݒ�
			col = D3DXCOLOR(frand + 0.9f, frand + 0.9f, frand + 0.9f, 1.0f);

			//���a�̐ݒ�
			fRadius = 15.0f;

			//�����̐ݒ�
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_JUMP:	// ��
	{
		for (int nCnt = 0; nCnt < 12; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.055f;
			move.y = rand() % 100 * 0.01f * 11.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.055f;

			//�F�̐ݒ�
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.7f);

			//���a�̐ݒ�
			fRadius = 8.0f + rand() % 10;

			//�����̐ݒ�
			fLife = 250.0f + rand() % 50;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_LAND:	// ��
	{
		for (int nCnt = 0; nCnt < 12; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.045f;
			move.y = rand() % 20 * 0.01f * -1.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.045f;

			//�F�̐ݒ�
			col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

			//���a�̐ݒ�
			fRadius = 30.0f + rand() % 10;

			//�����̐ݒ�
			fLife = 100.0f + rand() % 50;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_PARTY:	// ��
	{
		for (int nCnt = 0; nCnt < 15; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.045f;
			move.y = rand() % 20 * 0.01f * -1.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.045f;

			//�F�̐ݒ�
			col = D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f);

			//���a�̐ݒ�
			fRadius = 30.0f + rand() % 10;

			//�����̐ݒ�
			fLife = 100.0f + rand() % 50;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_TUTORIAL:	// ��
	{
		for (int nCnt = 0; nCnt < 15; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.045f;
			move.y = 0.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.045f;

			//�F�̐ݒ�
			col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

			//���a�̐ݒ�
			fRadius = 10.0f + rand() % 10;

			//�����̐ݒ�
			fLife = 100.0f + rand() % 50;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_PULLSMAKE:	// ��
	{
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			//�ړ��ʂ̐ݒ�
			move.x = sinf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;
			move.y = ((float)(rand() % 3 + 1)) * 3.0f;
			move.z = cosf((float)(rand() % 629 - 314) * 0.01f) * ((float)(rand() % 100)) * 0.01f;

			float frand = rand() % 8 * 0.1f;

			//�F�̐ݒ�
			col = D3DXCOLOR(frand + 0.1f, frand, 1.0f, 1.0f);

			//���a�̐ݒ�
			fRadius = 40.0f;

			//�����̐ݒ�
			fLife = 500.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_GUNCHARGE:	// ��
	{
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// ���W�̐ݒ�
			float fRot = static_cast<float>(rand() % 629 - 314) * 0.01f;
			float fRotZ = static_cast<float>(rand() % 315) * 0.01f;

			// �o���ʒu��ݒ�
			pos = D3DXVECTOR3(
				Defpos.x + (sinf(fRotZ) * cosf(fRot)) * 100.0f,
				Defpos.y + cosf(fRotZ) * 100.0f,
				Defpos.z + (sinf(fRotZ) * sinf(fRot)) * 100.0f);
			
			// �ڕW�܂ł̈ړ��x�N�g�����擾
			D3DXVECTOR3 nor = Defpos - pos;
			D3DXVec3Normalize(&nor, &nor);
			move = nor * 10.0f;

			//�F�̐ݒ�
			float frand = rand() % 8 * 0.1f;
			col = D3DXCOLOR(0.1f, frand, frand + 0.1f, 0.32f);

			//���a�̐ݒ�
			fRadius = 40.0f;

			//�����̐ݒ�
			fLife = 30.0f;

			CEffect::Create(pos, move, col, fRadius, fLife, type);
		}
	}
	break;

	case CEffect::TYPE_BOSSKNUCKLECHARGE:	// ��
	{
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// ���W�̐ݒ�
			float fRot = static_cast<float>(rand() % 629 - 314) * 0.01f;
			float fRotZ = static_cast<float>(rand() % 315) * 0.01f;

			// �o���ʒu��ݒ�
			pos = D3DXVECTOR3(
				Defpos.x + (sinf(fRotZ) * cosf(fRot)) * 100.0f,
				Defpos.y + cosf(fRotZ) * 100.0f,
				Defpos.z + (sinf(fRotZ) * sinf(fRot)) * 100.0f);

			// �ڕW�܂ł̈ړ��x�N�g�����擾
			D3DXVECTOR3 nor = Defpos - pos;
			D3DXVec3Normalize(&nor, &nor);
			move = nor * 10.0f;

			//�F�̐ݒ�
			float frand = rand() % 8 * 0.1f;
			col = D3DXCOLOR(1.0f, frand, 1.0f, 0.32f);

			//���a�̐ݒ�
			fRadius = 20.0f;

			//�����̐ݒ�
			fLife = 30.0f;

			CEffect::Create(pos, move, col, fRadius, fLife, type);
		}
	}
	break;


	case CEffect::TYPE_BOSSKNIFECHARGE:	// ��
	{
		for (int nCnt = 0; nCnt < 1; nCnt++)
		{
			// ���W�̐ݒ�
			float fRot = static_cast<float>(rand() % 629 - 314) * 0.01f;
			float fRotZ = static_cast<float>(rand() % 315) * 0.01f;

			// �o���ʒu��ݒ�
			pos = D3DXVECTOR3(
				Defpos.x + (sinf(fRotZ) * cosf(fRot)) * 100.0f,
				Defpos.y + cosf(fRotZ) * 100.0f,
				Defpos.z + (sinf(fRotZ) * sinf(fRot)) * 100.0f);

			// �ڕW�܂ł̈ړ��x�N�g�����擾
			D3DXVECTOR3 nor = Defpos - pos;
			D3DXVec3Normalize(&nor, &nor);
			move = nor * 10.0f;

			//�F�̐ݒ�
			float frand = rand() % 8 * 0.1f;
			col = D3DXCOLOR(1.0f, 1.0f, frand, 0.32f);

			//���a�̐ݒ�
			fRadius = 20.0f;

			//�����̐ݒ�
			fLife = 30.0f;

			CEffect::Create(pos, move, col, fRadius, fLife, CEffect::TYPE_BOSSKNUCKLECHARGE);
		}
	}
	break;

	case CEffect::TYPE_ENEMYHIT:	// ��
	{
		for (int nCnt = 0; nCnt < 15; nCnt++)
		{
			// ���W�̐ݒ�
			pos = Defpos;

			{
				float frand = static_cast<float>(rand() % 629 - 314);
				//�ړ��ʂ̐ݒ�
				move.x = sinf(frand * 0.01f) * ((float)(rand() % 100)) * 0.1f;
				move.y = sinf(frand * 0.01f) * ((float)(rand() % 100)) * 0.1f;
				move.z = cosf(frand * 0.01f) * ((float)(rand() % 100)) * 0.1f;
			}

			//�F�̐ݒ�
			col = D3DXCOLOR(rand() % 5 * 0.1f + 0.6f, rand() % 5 * 0.1f + 0.6f, rand() % 5 * 0.1f + 0.6f, 1.0f);

			//���a�̐ݒ�
			fRadius = 16.0f;

			//�����̐ݒ�
			fLife = 50.0f;

			CEffect::Create(Defpos + move, move, col, fRadius, fLife, type);
		}
	}
	break;
	}
}