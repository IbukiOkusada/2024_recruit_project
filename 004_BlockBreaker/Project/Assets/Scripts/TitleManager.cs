using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class TitleManager : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
       
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.Escape))
        {// Esc�L�[�������ꂽ�Ƃ�
            Quit();
        }
    }

    void Quit()
    {
#if UNITY_EDITOR    // �G�f�B�^�[
        UnityEditor.EditorApplication.isPlaying = false;
#elif UNITY_STANDALONE  // ���s
        UnityEngine.Application.Quit();
#endif
    }
}
