//==========================================================
//
// �ߐڍU���̓G [enemy_melee.h]
// Author : Ibuki Okusada
//
//==========================================================
#ifndef _ENEMY_MELEE_H_		// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _ENEMY_MELEE_H_		// ��d�C���N���[�h�h�~�p�}�N�����`

#include "enemy.h"	// ����Ńt�@�C���C���N���[�h�ł��܂�

class CWaist;
class CCharacter;

//==========================================================
// �T���v���̃N���X��`
//==========================================================
class CEnemyMelee : public CEnemy
{

public:	// �N�ł��A�N�Z�X�\

	CEnemyMelee();	// �R���X�g���N�^(�I�[�o�[���[�h)
	~CEnemyMelee();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CEnemyMelee *Create(void);

	// �����o�֐�
	void Hit(D3DXVECTOR3& pos, const float fRange, const int nDamage) override;

	// �����o�֐�(�擾)

	// �����o�֐�(�ݒ�)

private:	// �����������A�N�Z�X�\

	// �����o�֐�
	void BodySet(void);
	bool BodyCheck(CCharacter* pBody);
	void AttackCheck(void);
	void Damage(const int nDamage);

	// �����o�ϐ�
	CWaist* m_pWaist;		// ��
	CCharacter* m_pBody;	// �㔼�g
	CCharacter* m_pLeg;		// �����g
	int m_nAction;			// �A�N�V�����ԍ�
	int m_nInterVal;
};

#endif
