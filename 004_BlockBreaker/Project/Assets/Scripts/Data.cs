using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// �G���N���X
[System.Serializable]   // �N���X�̃f�[�^���C���X�y�N�^�ɕ\�������邽�߂ɕK�v
public class EnemyData
{
    // ��������v���n�u
    public GameObject m_Prefab;

    // �o�����x��
    public int m_Level;
}

// �������x���N���X
[System.Serializable]   // �N���X�̃f�[�^���C���X�y�N�^�ɕ\�������邽�߂ɕK�v
public class SpawnLevelData
{
    // �e���x���z�u��
    public int[] m_aCnt;
}