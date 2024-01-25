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
class CPlayer;

//==========================================================
// �T���v���̃N���X��`
//==========================================================
class CEnemyMelee : public CEnemy
{
private:

	// ��ԗ񋓌^
	enum STATE
	{
		STATE_NONE = 0,		// �����Ȃ����
		STATE_APPEAR,		// �o�����
		STATE_NORMAL,		// �ʏ���
		STATE_CHASE,		// �ǐՏ��
		STATE_ATTACK,		// �U�����
		STATE_DAMAGE,		// �_���[�W���
		STATE_DEATH,		// ���S���
		STATE_MAX
	};

	// �A�N�V�����񋓌^
	enum ACTION
	{
		ACTION_NEUTRAL = 0,		// �ҋ@
		ACTION_WALK,			// ���s
		ACTION_DUSH,			// ���s
		ACTION_ATK,				// �U��
		ACTION_2NDATK,			// ��i�U��
		ACTION_DAMAGE,			// �_���[�W
		ACTION_DEATH,			// ���S
		ACTION_MAX
	};

	// �ǐՑΏۏ��\����
	struct SChase
	{
		CPlayer* pTarget;	// �W�I
		float fLength;		// �W�I�Ƃ̋���
	};

	// ��ԊǗ��p�\����
	struct SStateInfo
	{
		STATE state;	// ���
		float fCounter;	// �Ǘ��J�E���^�[
	};

public:	// �N�ł��A�N�Z�X�\

	CEnemyMelee();	// �R���X�g���N�^(�I�[�o�[���[�h)
	~CEnemyMelee();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CEnemyMelee *Create(D3DXVECTOR3 &pos, D3DXVECTOR3 &rot);

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
	CPlayer *Search(float &fChaseLength);
	void Chase(void);
	void MethodLine(void);
	void SetState(void);
	void SetMotion(void);

	// �����o�ϐ�
	CWaist* m_pWaist;		// ��
	CCharacter* m_pBody;	// �㔼�g
	CCharacter* m_pLeg;		// �����g
	SChase m_Chase;			// �ǐՊǗ�
	SStateInfo m_StateInfo;	// ��ԊǗ�
	int m_nAction;			// �A�N�V�����ԍ�
	int m_nInterVal;
};

#endif
