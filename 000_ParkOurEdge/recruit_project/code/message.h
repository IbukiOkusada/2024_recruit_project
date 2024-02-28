//==========================================================
//
// ���b�Z�[�W�\���̏��� [message.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _MESSAGE_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _MESSAGE_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

#include "task.h"

// �O���錾
class CObjectBillboard;

//==========================================================
// �T���v���̃N���X��`
//==========================================================
class CMessage : public CTask
{

public:	// �N�ł��A�N�Z�X�\

	CMessage();	// �R���X�g���N�^(�I�[�o�[���[�h)
	~CMessage();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CMessage *Create(const D3DXVECTOR3& pos);

	// �����o�֐�(�擾)

	// �����o�֐�(�ݒ�)

private:	// �����������A�N�Z�X�\

	// �����o�֐�
	void SetPosition(const D3DXVECTOR3& pos);

	// �����o�ϐ�
	CObjectBillboard* m_pObj;	// �`��I�u�W�F�N�g
	D3DXVECTOR3 m_pos;			// ���W
	float m_fLife;				// �̗�
};

#endif
