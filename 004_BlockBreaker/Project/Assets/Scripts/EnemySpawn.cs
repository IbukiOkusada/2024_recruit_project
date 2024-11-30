using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// �G�o���N���X
[System.Serializable]
public class EnemySpawn
{
    // �����o�ϐ���`
    [SerializeField] private List<EnemyData> m_EnemyList;            // �G�z�u���
    [SerializeField] private List<SpawnLevelData> m_LevelInfoList;   // �e���x���z�u���
    [SerializeField] private int m_SpawnTimer;          // �����܂ł̃^�C�}�[
    [SerializeField] private float m_FrameCnt;          // �J�E���g������
    [SerializeField] private float m_NowCnt;            // �J�E���g
    [SerializeField] private float[] m_aPositionY;      // �G�z�u�������
    private int m_NowLevel;     // ���݂̃��x��
    private int m_DeadEnemyCnt; // �������ꂽ�G�̐�

    // Start is called before the first frame update
    void Start()
    {
        // �����l�ݒ�
        SetTimer(); // �^�C�}�[
    }

    // Update is called once per frame
    void Update()
    {

    }

    // �G�̐����J�E���g����(��{���W)
    public void SpawnAddCnt(Vector3 pos)
    {
        // �J�E���g����
        m_NowCnt += m_FrameCnt * Time.deltaTime;

        GameObject[] EnemyTagObj = GameObject.FindGameObjectsWithTag("Enemy");  // �G�^�O��S�Ď擾

        if (EnemyTagObj == null || m_NowCnt >= 1.0f) // �G�����Ȃ��������͓G���K��l
        {
            m_NowCnt = 0.0f;
            SpawnCheck(pos);   // �����m�F
        }
    }

    // �����m�F(��{���W)
    private void SpawnCheck(Vector3 pos)
    {
        // ���݂̃��x���̐�������G���x���܂ŌJ��Ԃ�
        for (int i = 0; i < m_LevelInfoList[m_NowLevel].m_aCnt.Length; i++)
        {
            Spawn(i, m_LevelInfoList[m_NowLevel].m_aCnt[i], pos);   // ����
        }
    }

    // �w�背�x���̓G�𐶐�
    public void Spawn(int Level, int Num, Vector3 pos)
    {
        int cnt = 0;    // �w�背�x���Ɠ����G�̐�

        // �f�[�^�𒊏o
        for (int i = 0; i < m_EnemyList.Count; i++)
        {
            if (m_EnemyList[i].m_Level == Level) // ���x������v
            {
                cnt++;
            }
        }

        if (cnt <= 0) { Debug.Log("enemynull!"); return; }    // ���݂��Ă��Ȃ�����

        // �G���i�[����ꏊ��p��
        EnemyData[] ExEnemy = new EnemyData[cnt];
        int excnt = 0;  // �i�[�ꏊ�̔z��

        // �f�[�^���ؓ�
        for (int i = 0; i < m_EnemyList.Count; i++)
        {
            if (m_EnemyList[i].m_Level == Level) // ���x������v
            {
                ExEnemy[excnt] = m_EnemyList[i];
                excnt++;
            }
        }

        // �w�萔�����_���Ŕz�u
        for (int i = 0; i < Num; i++)
        {
            // ���ꂼ�ꃉ���_���ŗv�f���o��
            int rand = Random.Range(0, cnt);  // ���
            int Heightrand = Random.Range(0, m_aPositionY.Length);  // ����

            // ���W��ݒ�
            Vector3 SetPos = pos;
            SetPos.y += m_aPositionY[Heightrand];
            SetPos.x += -0.5f;

            // �G��z�u
            GameObject.Instantiate(ExEnemy[rand].m_Prefab, SetPos, Quaternion.identity);
        }
    }

    // ����������
    public void AddDeadEnemy()
    {
        if (m_NowLevel + 1 >= m_LevelInfoList.Count) { return; }    // �ő僌�x��

        // ���S������
        m_DeadEnemyCnt++;

        if (m_DeadEnemyCnt % 2 == 0)    // 2�̂Ɉ��
        {
            // �o�����x�����グ��
            m_NowLevel++;
        }
    }

    // �^�C�}�[�l�ݒ�
    public void SetTimer()
    {
        m_NowLevel = 0;
        m_FrameCnt = 1.0f / m_SpawnTimer;
    }
}