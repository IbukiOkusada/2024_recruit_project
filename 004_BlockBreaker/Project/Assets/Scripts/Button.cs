using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Button : MonoBehaviour
{
    // �����ꂽ
    public void OnClick(string nextScene)
    {
        // �w�肳�ꂽ�V�[���ɑJ��
        SceneManager.LoadScene(nextScene);
    }

    // �I���{�^���������ꂽ
    public void ClickEnd()
    {
#if UNITY_EDITOR    // �G�f�B�^�[
        UnityEditor.EditorApplication.isPlaying = false;
#elif UNITY_STANDALONE  // ���s
        UnityEngine.Application.Quit();
#endif
    }
}
