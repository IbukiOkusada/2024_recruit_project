using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class UIManager : MonoBehaviour
{
    // �����o�ϐ���`
    [SerializeField] private TextMeshProUGUI m_LifeText;   // �̗͗p�e�L�X�g
    [SerializeField] private TextMeshProUGUI m_BulletText; // �e���p�e�L�X�g
    [SerializeField] private TextMeshProUGUI m_ScoreText;  // �X�R�A�p�e�L�X�g
    [SerializeField] private int m_Life;        // �̗�
    [SerializeField] private int m_Bullet;      // �e
    private int m_Score = 0;                    // �X�R�A

    //---------------
    // �v���p�e�B��`
    //---------------
    // �̗�
    public int Life
    {
        get { return m_Life; }
        set { m_Life = value; }
    }

    // �e��
    public int Bullet
    {
        get { return m_Bullet; }
        set { m_Bullet = value; }
    }

    // �X�R�A
    public int Score
    {
        get { return m_Score; }
        set { m_Score = value; }
    }

    // Start is called before the first frame update
    void Start()
    {
        // �����e�L�X�g�ݒ�
        AddLife(0);
        AddScore(0);
        AddBullet(0);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    // �̗͑���
    public void AddLife(int add)
    {
        if (m_LifeText == null) { return; } // �e�L�X�g����

        Life = Life + add;

        if (Life < 0) { Life = 0; }    // 0�ȉ��̃X�R�A

        // �e�L�X�g��������
        m_LifeText.text = "�~ " + Life.ToString("D2");
    }

    // �e������
    public void AddBullet(int add)
    {
        if (m_BulletText == null) { return; } // �e�L�X�g����

        Bullet = Bullet + add;

        if (Bullet < 0) { Bullet = 0; }    // 0�ȉ��̃X�R�A

        // �e�L�X�g��������
        m_BulletText.text = "�~ " + Bullet.ToString("D2");
    }

    // �X�R�A����
    public void AddScore(int add)
    {
        if(m_ScoreText == null) { return; } // �e�L�X�g����

        Score = Score + add;

        if(Score < 0) { Score = 0; }    // 0�ȉ��̃X�R�A

        // �e�L�X�g��������
        m_ScoreText.text = "Score: " + Score.ToString("D8");
    }
}
