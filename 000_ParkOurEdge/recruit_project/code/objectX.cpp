//==========================================================
//
// X�t�@�C�����f���̏����S�� [objectX.cpp]
// Author : Ibuki Okusada (�����蔻�蒲�� : Soma Ishihara)
//
//==========================================================
#include "main.h"
#include "objectX.h"
#include "manager.h"
#include "renderer.h"
#include "texture.h"
#include "Xfile.h"

// �ÓI�����o�ϐ��錾
CObjectX *CObjectX::m_pTop = NULL;	// �擪�̃I�u�W�F�N�g�ւ̃|�C���^
CObjectX *CObjectX::m_pCur = NULL;	// �Ō���̃I�u�W�F�N�g�ւ̃|�C���^

//==========================================================
// �R���X�g���N�^
//==========================================================
CObjectX::CObjectX(int nPriority) : CObject(nPriority)
{
	m_nIdxModel = -1;

	m_pNext = NULL;
	m_pPrev = NULL;

	// �������g�����X�g�ɒǉ�
	if (m_pTop != NULL)
	{// �擪�����݂��Ă���ꍇ
		m_pCur->m_pNext = this;	// ���ݍŌ���̃I�u�W�F�N�g�̃|�C���^�ɂȂ���
		m_pPrev = m_pCur;
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}
	else
	{// ���݂��Ȃ��ꍇ
		m_pTop = this;	// �������g���擪�ɂȂ�
		m_pCur = this;	// �������g���Ō���ɂȂ�
	}

	m_bEnableCollision = true;
}

//==========================================================
// �f�X�g���N�^
//==========================================================
CObjectX::~CObjectX()
{

}

//==========================================================
//���f���̏���������
//==========================================================
HRESULT CObjectX::Init(void)
{

	//�e��ϐ��̏�����
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	return S_OK;
}

//==========================================================
//���f���̏I������
//==========================================================
void CObjectX::Uninit(void)
{
	// ���X�g����O��
	ListOut();

	// �p��
	Release();
}

//==========================================================
//���f���̍X�V����
//==========================================================
void CObjectX::Update(void)
{

}

//==========================================================
//���f���̕`�揈��
//==========================================================
void CObjectX::Draw(void)
{
	//Update();

	LPDIRECT3DDEVICE9 pDevice = CManager::GetInstance()->GetRenderer()->GetDevice();		//�f�o�C�X�ւ̃|�C���^���擾
	CTexture *pTexture = CManager::GetInstance()->GetTexture();	// �e�N�X�`���ւ̃|�C���^
	CXFile *pModelFile = CManager::GetInstance()->GetModelFile();	// X�t�@�C�����̃|�C���^
	CXFile::FileData *pFileData = pModelFile->SetAddress(m_nIdxModel);
	D3DXMATRIX mtxRot, mtxTrans;			//�v�Z�p�}�g���b�N�X
	D3DMATERIAL9 matDef;					//���݂̃}�e���A���ۑ��p
	D3DXMATERIAL *pMat;						//�}�e���A���f�[�^�ւ̃|�C���^

	//�A���t�@�e�X�g��L���ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 10);

	if (pFileData != NULL)
	{// ���f�����g�p����Ă���ꍇ
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);

		//�����𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		//�ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		//���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);

		//���݂̃}�e���A�����擾
		pDevice->GetMaterial(&matDef);

		//�}�e���A���f�[�^�ւ̃|�C���^���擾
		pMat = (D3DXMATERIAL*)pFileData->pBuffMat->GetBufferPointer();
		for (int nCntMat = 0; nCntMat < (int)pFileData->dwNumMat; nCntMat++)
		{
			D3DMATERIAL9 mat = pMat[nCntMat].MatD3D;

			//�}�e���A���̐ݒ�
			pDevice->SetMaterial(&mat);

			//�e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, pTexture->SetAddress(pFileData->pIdexTexture[nCntMat]));

			//���f��(�p�[�c)�̕`��
			pFileData->pMesh->DrawSubset(nCntMat);
		}

		//�ۑ����Ă����}�e���A����߂�
		pDevice->SetMaterial(&matDef);
	}

	//�A���t�@�e�X�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 255);
}

//==========================================================
// ����
//==========================================================
CObjectX *CObjectX::Create(D3DXVECTOR3 pos,D3DXVECTOR3 rot, const char *pFileName, const int nPriority)
{
	CObjectX *pObjectX = NULL;
	CXFile *pModelFile = CManager::GetInstance()->GetModelFile();

	// �I�u�W�F�N�gX�̐���
	pObjectX = new CObjectX(nPriority);

	if (pObjectX != NULL)
	{// �����ł����ꍇ
		// ����������
		pObjectX->Init();

		// ���W
		pObjectX->SetPosition(pos);

		// ����
		pObjectX->SetRotation(rot);

		// ��ސݒ�
		pObjectX->SetType(TYPE_MODEL);

		// �ǂݍ��݊m�F
		pObjectX->BindFile(pModelFile->Regist(pFileName));
	}
	else
	{// �����Ɏ��s�����ꍇ
		return NULL;
	}

	return pObjectX;
}

//==========================================================
// ���f���̃t�@�C���ݒ�
//==========================================================
void CObjectX::BindFile(int nIdx)
{
	m_nIdxModel = nIdx;	//�g�p���郂�f���̐ݒ�
}

//==========================================================
// ���W�̐ݒ�
//==========================================================
void CObjectX::SetPosition(const D3DXVECTOR3 pos)
{
	m_pos = pos; 
}

//==========================================================
// �����̐ݒ�
//==========================================================
void CObjectX::SetRotation(const D3DXVECTOR3 rot)
{ 
	m_rot = rot;

	if (m_rot.z < -D3DX_PI)
	{// z���W�p�x���E
		m_rot.z += D3DX_PI * 2;
	}
	else if (m_rot.z > D3DX_PI)
	{// z���W�p�x���E
		m_rot.z += -D3DX_PI * 2;
	}

	if (m_rot.x < -D3DX_PI)
	{// x���W�p�x���E
		m_rot.x += D3DX_PI * 2;
	}
	else if (m_rot.x > D3DX_PI)
	{// x���W�p�x���E
		m_rot.x += -D3DX_PI * 2;
	}

	if (m_rot.y < -D3DX_PI)
	{// x���W�p�x���E
		m_rot.y += D3DX_PI * 2;
	}
	else if (m_rot.y > D3DX_PI)
	{// x���W�p�x���E
		m_rot.y += -D3DX_PI * 2;
	}
}

//==========================================================
// �����蔻��
//==========================================================
bool CObjectX::Collision(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, COLLISION_AXIS& Axis)
{
	CObjectX *pObj = m_pTop;	// �擪�擾
	bool bLand = false;	// ���n�������ۂ�
	D3DXVECTOR3 moveOld = move;

	//���u��
	D3DXVECTOR3 posTemp = posOld;

	//Y
	posTemp.y = pos.y;
	pObj = m_pTop;
	while (pObj != NULL)
	{
		CObjectX* pObjNext = pObj->m_pNext;
		if (pObj->m_bEnableCollision == true && pObj->CollisionCheck(posTemp, posOld, move, vtxMin, vtxMax, TYPE_Y))
		{
			bLand = true;
			Axis = TYPE_Y;
		}

		pObj = pObjNext;
	}

	//Z
	posTemp.z = pos.z;
	pObj = m_pTop;
	while (pObj != NULL)
	{
		CObjectX* pObjNext = pObj->m_pNext;
		if (pObj->m_bEnableCollision == true)
		{
			if (pObj->CollisionCheck(posTemp, posOld, move, vtxMin, vtxMax, TYPE_Z)) {
				Axis = TYPE_Z;
			}
		}

		pObj = pObjNext;
	}

	//X
	posTemp.x = pos.x;
	pObj = m_pTop;
	while (pObj != NULL)
	{
		CObjectX *pObjNext = pObj->m_pNext;
		if (pObj->m_bEnableCollision == true)
		{
			if (pObj->CollisionCheck(posTemp, posOld, move, vtxMin, vtxMax, TYPE_X)) {
				Axis = TYPE_X;
			}
		}

		pObj = pObjNext;
	}

	pos = posTemp;

	return bLand;
}

//==========================================================
// �����蔻��(�G�ꂽ���ǂ���)
//==========================================================
bool CObjectX::Touch(D3DXVECTOR3& pos, D3DXVECTOR3& posOld, D3DXVECTOR3& move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax)
{
	CObjectX *pObj = m_pTop;	// �擪�擾
	bool bLand = false;	// ���n�������ۂ�

	while (pObj != NULL)
	{
		CObjectX *pObjNext = pObj->m_pNext;
		if (pObj->m_bEnableCollision == true)
		{
			pObj->CollisionCheck(pos, posOld, move, vtxMin, vtxMax, TYPE_X);
			pObj->CollisionCheck(pos, posOld, move, vtxMin, vtxMax, TYPE_Z);
		}

		pObj = pObjNext;
	}

	return bLand;
}

//==========================================================
// �����蔻��(�O�ώg�p)
//==========================================================
bool CObjectX::CollisionCloss(D3DXVECTOR3& pos, D3DXVECTOR3& posOld, D3DXVECTOR3* posCollisioned)
{
	CObjectX* pObj = m_pTop;	// �擪�擾
	D3DXVECTOR3 posNear = D3DXVECTOR3(FLT_MAX, 0.0f, 0.0f);
	bool bCollision = false;

	while (pObj != NULL)
	{
		CObjectX* pObjNext = pObj->m_pNext;
		D3DXVECTOR3 posObjColl = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		if (pObj->CollisionCheckCloss(pos, posOld, &posObjColl))
		{
			float fLength = D3DXVec3Length(&(posOld - posObjColl));

			if (D3DXVec3Length(&(posOld - posNear)) > fLength)
			{
				posNear = posObjColl;
			}
			bCollision = true;
		}

		pObj = pObjNext;
	}

	if (posCollisioned != nullptr)
	{
		*posCollisioned = posNear;
	}

	return bCollision;
}

//==========================================================
// �����蔻��
//==========================================================
void CObjectX::SetRotSize(D3DXVECTOR3 &SetMax, D3DXVECTOR3 &SetMin, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMin, float fRot)
{
	//���ׂē����l���ɓ����
	SetMax.y = vtxMax.y;	//Max��y���W
	SetMin.y = vtxMin.y;	//Min��y���W

	//�����ɂ���ĕύX����
	if (fRot == D3DX_PI * 0.5f)
	{//������
		//�ő�l���f
		SetMax.x = vtxMax.z;	//x���W
		SetMax.z = -vtxMin.x;	//z���W

		//�ŏ��l���f
		SetMin.x = vtxMin.z;	//x���W
		SetMin.z = -vtxMax.x;	//z���W
	}
	else if (fRot == -D3DX_PI * 0.5f)
	{//�E����
		//�ő�l���f
		SetMax.x = -vtxMin.z;	//x���W
		SetMax.z = vtxMax.x;	//z���W

		//�ŏ��l���f
		SetMin.x = -vtxMax.z;	//x���W
		SetMin.z = vtxMin.x;	//z���W
	}
	else if(fRot == D3DX_PI || fRot == -D3DX_PI)
	{//���]���Ă���ꍇ
		//�����ȊO�̍ő�l���ŏ��l�ɂ���
		SetMax.x = -vtxMin.x;	//x���W
		SetMax.z = -vtxMin.z;	//z���W

		//�����ȊO�̍ŏ��l���ő�l�ɂ���
		SetMin.x = -vtxMax.x;	//x���W
		SetMin.z = -vtxMax.z;	//z���W
	}
	else
	{//����ȊO�̏ꍇ
		//�ő�l���f
		SetMax.x = vtxMax.x;	//x���W
		SetMax.z = vtxMax.z;	//z���W

		//�ŏ��l���f
		SetMin.x = vtxMin.x;	//x���W
		SetMin.z = vtxMin.z;	//z���W
	}
}

//==========================================================
// �ʔ���`�F�b�N
//==========================================================
bool CObjectX::CollisionCheck(D3DXVECTOR3 &pos, D3DXVECTOR3 &posOld, D3DXVECTOR3 &move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, COLLISION_AXIS axis, const float fRefMulti)
{
	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	bool bLand = false;	// ���n�������ۂ�
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �����𔽉f
	int nIdx = GetIdx();
	SetRotSize(vtxObjMax,
		vtxObjMin,
		pFile->GetMax(nIdx),
		pFile->GetMin(nIdx),
		m_rot.y);

	
	switch (axis)
	{
	case CObjectX::TYPE_X:	//X
		if (pos.y + vtxMax.y > m_pos.y + vtxObjMin.y
			&& pos.y + vtxMin.y < m_pos.y + vtxObjMax.y
			&& pos.z + vtxMax.z > m_pos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < m_pos.z + vtxObjMax.z)
		{//�͈͓��ɂ���
			if (posOld.x + vtxMin.x >= m_pos.x + vtxObjMax.x
				&& pos.x + vtxMin.x < m_pos.x + vtxObjMax.x)
			{//�E���獶�ɂ߂荞��
				move.x *= -0.0f;
				move.x *= fRefMulti;
				pos.x = m_pos.x + vtxObjMax.x - vtxMin.x + 0.1f + move.x;
			}
			else if (posOld.x + vtxMax.x <= m_pos.x + vtxObjMin.x
				&& pos.x + vtxMax.x > m_pos.x + vtxObjMin.x)
			{//������E�ɂ߂荞��
			 //�ʒu��߂�
				move.x *= -0.0f;
				move.x *= fRefMulti;
				pos.x = m_pos.x + vtxObjMin.x - vtxMax.x - 0.1f + move.x;
			}
		}
		break;
	case CObjectX::TYPE_Y:	//Y
		if (pos.x + vtxMax.x > m_pos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < m_pos.x + vtxObjMax.x
			&& pos.z + vtxMax.z > m_pos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < m_pos.z + vtxObjMax.z)
		{//�͈͓��ɂ���
			//�ォ��̔���
			if (posOld.y + vtxMin.y >= m_pos.y + vtxObjMax.y
				&& pos.y + vtxMin.y < m_pos.y + vtxObjMax.y)
			{//�ォ��߂荞��
				//��ɂ̂���
				pos.y = m_pos.y + vtxObjMax.y - vtxMin.y;
				move.y = 0.0f;
				bLand = true;
			}

			//������̔���
			if (posOld.y + vtxMax.y <= m_pos.y + vtxObjMin.y
				&& pos.y + vtxMax.y > m_pos.y + vtxObjMin.y)
			{//������߂荞��
			 //���̍����̂܂�
				pos.y = m_pos.y - vtxObjMin.y - vtxMax.y;
				move.y = 0.0f;
				bLand = true;
			}
		}
		break;
	case CObjectX::TYPE_Z:	//Z
		if (pos.x + vtxMax.x > m_pos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < m_pos.x + vtxObjMax.x
			&& pos.y + vtxMax.y > m_pos.y + vtxObjMin.y
			&& pos.y + vtxMin.y < m_pos.y + vtxObjMax.y)
		{//�͈͓��ɂ���
			if (posOld.z + vtxMin.z >= m_pos.z + vtxObjMax.z
				&& pos.z + vtxMin.z < m_pos.z + vtxObjMax.z)
			{//�������O�ɂ߂荞��
			 //�ʒu��߂�
				move.z *= -0.0f;
				move.z *= fRefMulti;
				pos.z = m_pos.z + vtxObjMax.z - vtxMin.z + 0.1f + move.z;
				//move.z = 0.0f;
			}
			else if (posOld.z + vtxMax.z <= m_pos.z + vtxObjMin.z
				&& pos.z + vtxMax.z > m_pos.z + vtxObjMin.z)
			{//��O���牜�ɂ߂荞��
			 //�ʒu��߂�
				move.z *= -0.0f;
				move.z *= fRefMulti;
				pos.z = m_pos.z + vtxObjMin.z - vtxMax.z - 0.1f + move.z;
				//move.z = 0.0f;
			}
		}
		break;
	default:
		break;
	}

	return bLand;
}

//==========================================================
// �ʔ���`�F�b�N(�O�ώg�p)
//==========================================================
bool CObjectX::CollisionCheckCloss(D3DXVECTOR3& pos, D3DXVECTOR3& posOld, D3DXVECTOR3* posCollisioned)
{
	CXFile* pFile = CManager::GetInstance()->GetModelFile();
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	bool bCollision = false;
	D3DXVECTOR3 posCulcNear = D3DXVECTOR3(FLT_MAX, 0.0f, 0.0f);

	// �����𔽉f
	int nIdx = GetIdx();
	SetRotSize(vtxObjMax,
		vtxObjMin,
		pFile->GetMax(nIdx),
		pFile->GetMin(nIdx),
		m_rot.y);

	D3DXVECTOR3 posPoint[4] =
	{
		D3DXVECTOR3(m_pos.x + vtxObjMin.x,0.0f,m_pos.z + vtxObjMin.z),
		D3DXVECTOR3(m_pos.x + vtxObjMax.x,0.0f,m_pos.z + vtxObjMin.z),
		D3DXVECTOR3(m_pos.x + vtxObjMax.x,0.0f,m_pos.z + vtxObjMax.z),
		D3DXVECTOR3(m_pos.x + vtxObjMin.x,0.0f,m_pos.z + vtxObjMax.z)
	};

	D3DXVECTOR3 vecMove, vecLine;
	D3DXVECTOR3 vecToPos, vecToPosOld;
	float fAreaA = 1.0f, fAreaB = 1.1f;

	for (int cnt = 0; cnt < 4; cnt++)
	{
		vecMove = pos - posOld;
		vecLine = posPoint[(cnt + 1) % 4] - posPoint[cnt];	//���E���x�N�g��
		vecToPos = pos - posPoint[cnt];
		vecToPos.y = 0.0f;
		vecToPosOld = posOld - posPoint[cnt];
		vecToPosOld.y = 0.0f;

		//�ʐϋ��߂�
		fAreaA = (vecToPos.z * vecMove.x) - (vecToPos.x * vecMove.z);
		fAreaB = (vecLine.z * vecMove.x) - (vecLine.x * vecMove.z);

		if ((vecLine.z * vecToPosOld.x) - (vecLine.x * vecToPosOld.z) >= 0.0f && (vecLine.z * vecToPos.x) - (vecLine.x * vecToPos.z) < 0.0f)
		{
			if (fAreaA / fAreaB >= 0.0f && fAreaA / fAreaB <= 1.0f)
			{
				float fRate = fAreaA / fAreaB;
				D3DXVECTOR3 posXZ = posPoint[cnt];
				posXZ.x += vecLine.x * fRate;
				posXZ.y = posOld.y;
				posXZ.z += vecLine.z * fRate;
				float fTemp1 = D3DXVec3Length(&(D3DXVECTOR3(pos.x, posOld.y, pos.z) - posOld));
				float fTemp2 = D3DXVec3Length(&(D3DXVECTOR3(posXZ.x, posOld.y, posXZ.z) - posOld));
				float fRate2 = fTemp2 / fTemp1;
				D3DXVECTOR3 posCulc = posOld + (pos - posOld) * fRate2;

				if (posCulc.y >= m_pos.y + vtxObjMin.y && posCulc.y <= m_pos.y + vtxObjMax.y)
				{//������
					//�Փˈʒu���~������΂�����
					if (posCollisioned != nullptr)
					{//�ق����݂����Ȃ̂ł�����
						if (D3DXVec3Length(&(posCulc - posOld)) < D3DXVec3Length(&(posCulcNear - posOld)))
						{
							posCulcNear = posCulc;
						}
					}

					bCollision = true;
				}
			}
		}
	}

	if (bCollision == true)
	{
		*posCollisioned = posCulcNear;
	}

	return bCollision;
}

//==========================================================
// ���X�g����O��
//==========================================================
void CObjectX::ListOut(void)
{
	// ���X�g���玩�����g���폜����
	if (m_pTop == this)
	{// ���g���擪
		if (m_pNext != NULL)
		{// �������݂��Ă���
			m_pTop = m_pNext;	// ����擪�ɂ���
			m_pNext->m_pPrev = NULL;	// ���̑O�̃|�C���^���o���Ă��Ȃ��悤�ɂ���
		}
		else
		{// ���݂��Ă��Ȃ�
			m_pTop = NULL;	// �擪���Ȃ���Ԃɂ���
			m_pCur = NULL;	// �Ō�����Ȃ���Ԃɂ���
		}
	}
	else if (m_pCur == this)
	{// ���g���Ō��
		if (m_pPrev != NULL)
		{// �������݂��Ă���
			m_pCur = m_pPrev;			// �O���Ō���ɂ���
			m_pPrev->m_pNext = NULL;	// �O�̎��̃|�C���^���o���Ă��Ȃ��悤�ɂ���
		}
		else
		{// ���݂��Ă��Ȃ�
			m_pTop = NULL;	// �擪���Ȃ���Ԃɂ���
			m_pCur = NULL;	// �Ō�����Ȃ���Ԃɂ���
		}
	}
	else
	{
		if (m_pNext != NULL)
		{
			m_pNext->m_pPrev = m_pPrev;	// ���g�̎��ɑO�̃|�C���^���o��������
		}
		if (m_pPrev != NULL)
		{
			m_pPrev->m_pNext = m_pNext;	// ���g�̑O�Ɏ��̃|�C���^���o��������
		}
	}
}

void CObjectX::CollisionLand(D3DXVECTOR3 &pos)
{
	CObjectX *pObj = m_pTop;	// �擪�擾
	CXFile *pFile = CManager::GetInstance()->GetModelFile();

	while (pObj != NULL)
	{
		CObjectX *pObjNext = pObj->m_pNext;
		D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �����𔽉f
		int nIdx = pObj->GetIdx();
		pObj->SetRotSize(vtxObjMax,
			vtxObjMin,
			pFile->GetMax(nIdx),
			pFile->GetMin(nIdx),
			pObj->m_rot.y);

		if (pos.x >= pObj->m_pos.x + vtxObjMin.x
			&& pos.x <= pObj->m_pos.x + vtxObjMax.x
			&& pos.z >= pObj->m_pos.z + vtxObjMin.z
			&& pos.z <= pObj->m_pos.z + vtxObjMax.z)
		{//�͈͓��ɂ���
			//�ォ��̔���
			if (pos.y < pObj->m_pos.y + vtxObjMax.y)
			{//�ォ��߂荞��
				//��ɂ̂���
				pos.y = pObj->m_pos.y + vtxObjMax.y + 4.0f;
			}
		}

		pObj = pObjNext;
	}
}

//==========================================================
// �����蔻��
//==========================================================
D3DXVECTOR3 CObjectX::Collision(D3DXVECTOR3& pos, D3DXVECTOR3& posOld, D3DXVECTOR3& move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMinOld, D3DXVECTOR3 vtxMaxOld, COLLISION_AXIS& Axis)
{
	CObjectX *pObj = m_pTop;	// �擪�擾
	bool bLand = false;	// ���n�������ۂ�
	D3DXVECTOR3 moveOld = move;
	D3DXVECTOR3 nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���u��
	D3DXVECTOR3 posTemp = posOld;

	//Y
	posTemp.y = pos.y;
	pObj = m_pTop;
	while (pObj != NULL)
	{
		CObjectX* pObjNext = pObj->m_pNext;
		if (pObj->m_bEnableCollision == true && pObj->CollisionCheck(posTemp, posOld, move, vtxMin, vtxMax, vtxMinOld, vtxMaxOld, TYPE_Y))
		{
			bLand = true;
			Axis = TYPE_Y;
			nor.y = posTemp.y - pObj->GetPosition().y;
		}

		pObj = pObjNext;
	}

	//Z
	posTemp.z = pos.z;
	pObj = m_pTop;
	while (pObj != NULL)
	{
		CObjectX* pObjNext = pObj->m_pNext;
		if (pObj->m_bEnableCollision == true && pObj->CollisionCheck(posTemp, posOld, move, vtxMin, vtxMax, vtxMinOld, vtxMaxOld, TYPE_Z))
		{
			Axis = TYPE_Z;
			nor.z = posTemp.z - pObj->GetPosition().z;
		}

		pObj = pObjNext;
	}

	//X
	posTemp.x = pos.x;
	pObj = m_pTop;
	while (pObj != NULL)
	{
		CObjectX *pObjNext = pObj->m_pNext;
		if (pObj->m_bEnableCollision == true && pObj->CollisionCheck(posTemp, posOld, move, vtxMin, vtxMax, vtxMinOld, vtxMaxOld, TYPE_X))
		{
			bLand = true;
			Axis = TYPE_X;
			nor.x = posTemp.x - pObj->GetPosition().x;
		}

		pObj = pObjNext;
	}

	pos = posTemp;
	D3DXVec3Normalize(&nor, &nor);

	return nor;
}

//==========================================================
// �����蔻��m�F
//==========================================================
bool CObjectX::CollisionCheck(D3DXVECTOR3& pos, D3DXVECTOR3& posOld, D3DXVECTOR3& move, D3DXVECTOR3 vtxMin, D3DXVECTOR3 vtxMax, D3DXVECTOR3 vtxMinOld, D3DXVECTOR3 vtxMaxOld, COLLISION_AXIS axis)
{
	CXFile *pFile = CManager::GetInstance()->GetModelFile();
	bool bLand = false;	// ���n�������ۂ�
	D3DXVECTOR3 vtxObjMax = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vtxObjMin = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �����𔽉f
	int nIdx = GetIdx();
	SetRotSize(vtxObjMax,
		vtxObjMin,
		pFile->GetMax(nIdx),
		pFile->GetMin(nIdx),
		m_rot.y);


	switch (axis)
	{
	case CObjectX::TYPE_X:	//X
		if (pos.y + vtxMax.y > m_pos.y + vtxObjMin.y
			&& pos.y + vtxMin.y < m_pos.y + vtxObjMax.y
			&& pos.z + vtxMax.z > m_pos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < m_pos.z + vtxObjMax.z)
		{//�͈͓��ɂ���
			if (posOld.x + vtxMinOld.x >= m_pos.x + vtxObjMax.x
				&& pos.x + vtxMin.x < m_pos.x + vtxObjMax.x)
			{//�E���獶�ɂ߂荞��
				move.x *= -0.0f;
				pos.x = m_pos.x + vtxObjMax.x - vtxMin.x + move.x;
				bLand = true;
			}
			else if (posOld.x + vtxMaxOld.x <= m_pos.x + vtxObjMin.x
				&& pos.x + vtxMax.x > m_pos.x + vtxObjMin.x)
			{//������E�ɂ߂荞��
				move.x *= -0.0f;
				pos.x = m_pos.x + vtxObjMin.x - vtxMax.x + move.x;
				bLand = true;
			}
		}
		break;
	case CObjectX::TYPE_Y:	//Y
		if (pos.x + vtxMax.x > m_pos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < m_pos.x + vtxObjMax.x
			&& pos.z + vtxMax.z > m_pos.z + vtxObjMin.z
			&& pos.z + vtxMin.z < m_pos.z + vtxObjMax.z)
		{//�͈͓��ɂ���
		 //�ォ��̔���
			if (posOld.y + vtxMinOld.y >= m_pos.y + vtxObjMax.y
				&& pos.y + vtxMin.y < m_pos.y + vtxObjMax.y)
			{//�ォ��߂荞��
				pos.y = m_pos.y + vtxObjMax.y - vtxMin.y;
				move.y = 0.0f;
				bLand = true;
			}

			//������̔���
			if (posOld.y + vtxMaxOld.y <= m_pos.y + vtxObjMin.y
				&& pos.y + vtxMax.y > m_pos.y + vtxObjMin.y)
			{//������߂荞��
				pos.y = m_pos.y - vtxObjMin.y - vtxMax.y;
				move.y = 0.0f;
				bLand = true;
			}
		}
		break;
	case CObjectX::TYPE_Z:	//Z
		if (pos.x + vtxMax.x > m_pos.x + vtxObjMin.x
			&& pos.x + vtxMin.x < m_pos.x + vtxObjMax.x
			&& pos.y + vtxMax.y > m_pos.y + vtxObjMin.y
			&& pos.y + vtxMin.y < m_pos.y + vtxObjMax.y)
		{//�͈͓��ɂ���
			if (posOld.z + vtxMinOld.z >= m_pos.z + vtxObjMax.z
				&& pos.z + vtxMin.z < m_pos.z + vtxObjMax.z)
			{//�������O�ɂ߂荞��
			 //�ʒu��߂�
				move.z *= -0.0f;
				pos.z = m_pos.z + vtxObjMax.z - vtxMin.z + move.z;
				bLand = true;
			}
			else if (posOld.z + vtxMaxOld.z <= m_pos.z + vtxObjMin.z
				&& pos.z + vtxMax.z > m_pos.z + vtxObjMin.z)
			{//��O���牜�ɂ߂荞��
			 //�ʒu��߂�
				move.z *= -0.0f;
				pos.z = m_pos.z + vtxObjMin.z - vtxMax.z + move.z;
				bLand = true;
			}
		}
		break;
	default:
		break;
	}

	return bLand;
}