using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    // �����o�ϐ���`
    [SerializeField] protected float m_SceneTimer;    // �J�ڃ^�C�}�[
    private float m_NowTimer;     // ���݂̈ړ��^�C�}�[
    private float m_AddTimer;     // �^�C�}�[���Z��
    private bool m_IsClear;       // �N���A�t���O
    private bool m_End = false;   // �I���t���O

    // Start is called before the first frame update
    void Start()
    {
        // �l�̏����ݒ�
        m_IsClear = false;
        m_NowTimer = 0.0f;
        m_AddTimer = 1.0f / m_SceneTimer;
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.Escape))
        {// Esc�L�[�������ꂽ�Ƃ�
            Quit();
        }

        // �J�ڊm�F
        if (!m_End) // �t���O�������Ă��Ȃ�
        {
            // �I���m�F
            CheckEnd();
            return;
        }

        // �^�C�}�[��i�߂�
        m_NowTimer += m_AddTimer * Time.deltaTime;

        if(m_NowTimer < 1.0f) { return; }   // �^�C�}�[�K��l�O

        // ���U���g�p�̃p�����[�^�[��n��
        ResultUIManager.m_IsClear = m_IsClear;  // �N���A�ݒ�
        SetScore(); // �X�R�A�ݒ�

        // ���U���g�ɑJ��
        SceneManager.LoadScene("Result");
    }

    // �I��
    void Quit()
    {
#if UNITY_EDITOR    // �G�f�B�^�[
        UnityEditor.EditorApplication.isPlaying = false;
#elif UNITY_STANDALONE  // ���s
        UnityEngine.Application.Quit();
#endif
    }

    // �I���ݒ�
    public void SetEnd(bool IsClear)
    {
        m_End = true;
        m_IsClear = IsClear;
    }

    // ���U���g�p�X�R�A�ݒ�
    private void SetScore()
    {
        // UIManager�^�O�̃I�u�W�F���擾
        GameObject obj = GameObject.FindWithTag("UIManager");
        if (obj == null) { return; }    // �擾�ł��Ȃ�����

        // UIManager���擾
        UIManager ui = obj.GetComponent<UIManager>();
        if (ui == null) { return; }    // �擾�ł��Ȃ�����

        // �X�R�A�̐ݒ�
        ResultUIManager.m_Score = ui.Score;
    }

    // �I���m�F
    private void CheckEnd()
    {
        // �{�X�����݂��Ă��邩�`�F�b�N����
        {
            GameObject obj = GameObject.FindWithTag("Boss");

            if (obj == null)    // �{�X�����݂��Ȃ�
            {
                // �N���A�ݒ�
                SetEnd(true);
                return;
            }
        }

        // �e�����݂��Ă��邩�`�F�b�N����
        BulletCheck();

        // �̗͂��܂����邩�`�F�b�N����
        LifeCheck();
    }

    // �e�����݂��Ă��邩�m�F����
    private void BulletCheck()
    {
        // �e�^�O��S�Ď擾
        GameObject[] obj = GameObject.FindGameObjectsWithTag("Bullet");
        if (obj.Length > 0) { return; }  // �e����ʏ�ɑ��݂���

        // UIManager�^�O�̃I�u�W�F���擾
        GameObject uiobj = GameObject.FindWithTag("UIManager");
        if (uiobj == null) { return; }    // �擾�ł��Ȃ�����

        // UIManager���擾
        UIManager ui = uiobj.GetComponent<UIManager>();
        if (ui == null) { return; }    // �擾�ł��Ȃ�����
        if (ui.Bullet > 0) { return; }   // �c�e������

        // �Q�[���I�[�o�[�ݒ�
        SetEnd(false);
    }

    // �̗͂��܂����邩�`�F�b�N����
    private void LifeCheck()
    {
        // UIManager�^�O�̃I�u�W�F���擾
        GameObject obj = GameObject.FindWithTag("UIManager");
        if (obj == null) { return; }    // �擾�ł��Ȃ�����

        // UIManager���擾
        UIManager ui = obj.GetComponent<UIManager>();
        if (ui == null) { return; }    // �擾�ł��Ȃ�����
        if (ui.Life > 0) { return; }   // �̗͂�����

        // �Q�[���I�[�o�[�ݒ�
        SetEnd(false);
    }
}
