using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class ResultUIManager : MonoBehaviour
{
    // �ÓI�����o�ϐ�
    public static int m_Score = 0;  // ���U���g�X�R�A
    public static bool m_IsClear = false;   // �N���A�t���O

    // �����o�ϐ���`
    [SerializeField] private TextMeshProUGUI m_GameOverText;   // �̗͗p�e�L�X�g
    [SerializeField] private TextMeshProUGUI m_GameClearText; // �e���p�e�L�X�g
    [SerializeField] private TextMeshProUGUI m_ScoreText;  // �X�R�A�p�e�L�X�g

    // Start is called before the first frame update
    void Start()
    {
        // �N���A�����ݒ�
        SetText();

        // �X�R�A�ݒ�
        SetScore();
    }

    // Update is called once per frame
    void Update()
    {

    }

    // �N���A�e�L�X�g�ݒ�
    private void SetText()
    {
        bool GameOver = false;
        bool GameClear = false;

        // �N���A�m�F
        if(m_IsClear)   // �Q�[���N���A
        {
            GameClear = true;
        }
        else  // ���[�ނ��[�΁[
        {
            GameOver = true;
        }

        // �\����\���ݒ�
        m_GameOverText.enabled = GameOver;
        m_GameClearText.enabled = GameClear;

        // �t���O���Z�b�g
        m_IsClear = false;
    }

    // �X�R�A�ݒ�
    private void SetScore()
    {
        // �e�L�X�g��������
        m_ScoreText.text = "Score: " + m_Score.ToString("D8");

        // �X�R�A���Z�b�g
        m_Score = 0;
    }
}
