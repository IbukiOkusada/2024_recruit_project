//===============================================
//
// �G�t�F�N�g�̏��� [effect.h]
// Author : Ibuki Okusada
//
//===============================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

#include "task.h"
#include "texture.h"

// �O���錾
class CObjectBillboard;

//===============================================
//�N���X�̒�`
//===============================================
class CEffect : public CTask
{
public:	// �N�ł��A�N�Z�X�\�Ȓ�`

	// ��ނ̗񋓌^��`
	typedef enum
	{
		TYPE_NONE = 0,			// �ʏ�
		TYPE_SMAKE,				// ��
		TYPE_ITEMGET,			// �A�C�e���l��
		TYPE_LANDCLEAR,			// ���n����
		TYPE_LANDFAILED,		// ���s
		TYPE_HIT,				// �U���q�b�g
		TYPE_BLACKSMAKE,		// ������
		TYPE_WALK,				// ����
		TYPE_KUNAI,				// �N�i�C
		TYPE_AIR,				// ���_�̏p
		TYPE_JUMP,				// �W�����v
		TYPE_LAND,				// ���n
		TYPE_PARTY,				// �^�C�g��
		TYPE_TUTORIAL,			// �`���[�g���A��
		TYPE_PULLSMAKE,			// �������̎�
		TYPE_DUST,				// ��
		TYPE_GUNCHARGE,			// �e�`���[�W
		TYPE_BOSSGUN,			// �{�X�e
		TYPE_BOSSKNUCKLECHARGE,		// �{�X�̌�����
		TYPE_BOSSKNIFECHARGE,		// �{�X�̃i�C�t����
		TYPE_MAX
	}TYPE;

private:	// ���������A�N�Z�X�\�Ȓ�`

	// ���\���̂̒�`
	typedef struct
	{
		float fLife;	// ����
		D3DXVECTOR3 pos;	// �ʒu
		TYPE Type;	// ���
		D3DXCOLOR col;	//�F
		D3DXVECTOR3 move;	// �ړ���
		float fRadius;	//���a
	}INFO;

public:	// �N�ł��A�N�Z�X�\

	CEffect();	// �R���X�g���N�^
	~CEffect();	// �f�X�g���N�^

	// �����o�֐�
	HRESULT Init(void);
	void Uninit(void);
	void Update(void);
	static CEffect *Create(D3DXVECTOR3 pos, TYPE type);
	static CEffect *Create(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fLife, TYPE type);
	void SetMove(D3DXVECTOR3 move);
	D3DXVECTOR3 GetPosition(void) { return m_Info.pos; }
	void SetPosition(const D3DXVECTOR3& pos) { m_Info.pos = pos; }
	D3DXVECTOR3 GetMove(void) { return m_Info.move; }
	void SetType(TYPE type) { m_Info.Type = type; }
	CObjectBillboard *GetObj(void) { return m_pObjectBilBoard; }
	float GetRange(void) const;
	D3DXCOLOR GetCol(void) const;
	float GetLife(void) const { return m_Info.fLife; }

private:	// �����������A�N�Z�X�\

	// �����o�֐�
	void RadiusSet(void);
	void ColorSet(void);
	void InfoSet(void);
	void DrawSet(void);
	CTexture::TYPE SetTex(TYPE type);

	// �����o�ϐ�
	INFO m_Info;	// ���̍\����
	CObjectBillboard *m_pObjectBilBoard;
};

#endif 