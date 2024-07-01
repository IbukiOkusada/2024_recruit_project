//===============================================
//
// �ԑS�ʂ̊Ǘ�����(car_manager.h)
// Author : Ibuki Okusada
//
//===============================================
#ifndef _CARMANAGER_H_	// ���̃}�N������`����Ă��Ȃ��ꍇ
#define _CARMANAGER_H_	// ��d�C���N���[�h�h�~�p�}�N�����`

// �萔��`
namespace
{
	const int MAX_CAR = 6;	// �Ԃ̍ő���
}

// �O���錾
class CCar;

//===============================================
// �}�l�[�W���[�N���X�̒�`
//===============================================
class CCarManager
{
private:	// ���������A�N�Z�X�\�Ȓ�`

	// �Ԑ���`
	enum LOAD
	{
		LOAD_RIGHT = 0,	// �E�Ԑ�
		LOAD_LEFT,		// ���Ԑ�
		LOAD_MAX
	};

	// �����Ǘ��p�\����
	struct SPAWN
	{
		int nNum;		// �o����
		float fCount;	// �J�E���^�[
		float fDef;		// �K��l
	};

	// ����p�\����
	struct COLLISION
	{
		D3DXVECTOR3 pos;	// ���W
		D3DXVECTOR3 posOld;	// �O��̍��W
		D3DXVECTOR3 vtxMax;	// �ő�
		D3DXVECTOR3 vtxMin;	// �ŏ�
		D3DXVECTOR3 move;	// �ړ���

		COLLISION() : 
			pos(MYDEFAULT::VECTOR3), posOld(MYDEFAULT::VECTOR3), 
			vtxMax(MYDEFAULT::VECTOR3), vtxMin(MYDEFAULT::VECTOR3), 
			move(MYDEFAULT::VECTOR3) {}

		COLLISION(const D3DXVECTOR3& _pos, const D3DXVECTOR3& _posOld, 
			const D3DXVECTOR3& _vtxMax, const D3DXVECTOR3& _vtxMin, 
			const D3DXVECTOR3& _move) : pos(_pos), posOld(_posOld),
			vtxMax(_vtxMax), vtxMin(_vtxMin),
			move(_move) {}
	};

public:	// �N�ł��A�N�Z�X�\

	CCarManager();	// �R���X�g���N�^
	~CCarManager();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	CCar *Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, bool *pJump);
	bool CollisionCheck(COLLISION* car, COLLISION* target, const int nType);
	void SetRanking(void);

private:	// �����������A�N�Z�X�\

	// �����o�֐�
	void Spawn(void);
	void Delete(void);
	bool BindList(CCar *pCar, LOAD load);
	void DetachList(CCar *pCar, LOAD load);

	// �ÓI�����o�ϐ���
	static const char *m_apFileName[MAX_CAR];	// �Ԃ̎��
	static const D3DXVECTOR2 m_aShadowSize[MAX_CAR];	// �Ԃ̎�ނ��Ƃ̉e�̃T�C�Y

	// �����o�ϐ�
	SPAWN m_aSpawn[LOAD_MAX];	// �������

	// ���X�g�\���Ǘ��ϐ�
	CCar *m_apTop[LOAD_MAX];	// �擪
	CCar *m_apCur[LOAD_MAX];	// �Ō��
};

#endif
