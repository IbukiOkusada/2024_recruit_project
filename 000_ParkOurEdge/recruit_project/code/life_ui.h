//==========================================================
//
// �̗�UI�Ǘ� [life_ui.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _LIFEUI_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _LIFEUI_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

#include "task.h"	// ����Ńt�@�C���C���N���[�h�ł��܂�

// �O���錾
class CLife;
class CObject2D;

//==========================================================
// �T���v���̃N���X��`
//==========================================================
class CLifeUI : public CTask
{

public:	// �N�ł��A�N�Z�X�\

	CLifeUI();	// �R���X�g���N�^(�I�[�o�[���[�h)
	~CLifeUI();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CLifeUI *Create(const D3DXVECTOR3& pos, const int nMax);
	void SetLife(const int nNum);

	// �����o�֐�(�擾)

	// �����o�֐�(�ݒ�)

private:	// �����������A�N�Z�X�\

	// �����o�֐�
	void SetPosition(const D3DXVECTOR3& pos);
	void SetMaxLife(const int nMax);

	// �����o�ϐ�
	int m_nMaxLife;	// �ő�̗�
	D3DXVECTOR3 m_pos;	// ���W
	CObject2D* m_pFrame;	// �g�g��
	CLife** m_ppObj;		// �̗�
};

#endif
