//==========================================================
//
// �G�̏��� [enemy.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _ENEMY_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _ENEMY_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

#include "task.h"	// ����Ńt�@�C���C���N���[�h�ł��܂�

//==========================================================
// �T���v���̃N���X��`
//==========================================================
class CEnemy : public CTask
{
protected:

	// ��ޗ񋓌^
	enum TYPE
	{
		TYPE_MELEE = 0,		// �ߐڌ^(�ߋ���)
		TYPE_SHIELD,		// ������
		TYPE_GUN,			// �e(������)
		TYPE_MAX
	};

protected:

	// ���\����
	struct SInfo
	{
		D3DXVECTOR3 pos;		// �ʒu
		D3DXVECTOR3 rot;		// ����
		D3DXVECTOR3 move;		// �ړ���
		D3DXVECTOR3 posOld;		// �O��̍��W
		D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
	};

public:	// �N�ł��A�N�Z�X�\

	CEnemy();	// �R���X�g���N�^(�I�[�o�[���[�h)
	~CEnemy();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);

	// �����o�֐�(�擾)
	CEnemy* GetNext(void) { return m_pNext; }
	CEnemy* GetPrev(void) { return m_pPrev; }

	// �����o�֐�(�ݒ�)
	void SetNext(CEnemy* pNext) { m_pNext = pNext; }
	void SetPrev(CEnemy* pPrev) { m_pPrev = pPrev; }

	// �����o�֐�
	virtual void Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage);

protected:

	// �����o�ϐ�
	TYPE GetType(void) { return m_type; }
	void SetType(TYPE type) { m_type = type; }
	SInfo* GetInfo(void) { return &m_Info; }
	void SetInfo(const SInfo& Info) { m_Info = Info; }
	void SetMatrix(void);
	int GetLife(void) { return m_nLife; }
	void SetLife(const int nLife) { m_nLife = nLife; }

private:	// �����������A�N�Z�X�\

	// �����o�ϐ�
	CEnemy* m_pPrev;	// �O�̃I�u�W�F�N�g�ւ̃|�C���^
	CEnemy* m_pNext;	// ���̃I�u�W�F�N�g�ւ̃|�C���^
	int m_nLife;		// �̗�
	TYPE m_type;		// ���
	SInfo m_Info;		// ���
};

#endif
