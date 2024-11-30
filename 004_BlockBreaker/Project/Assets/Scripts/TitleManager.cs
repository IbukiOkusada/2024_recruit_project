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
        {// Escキーが押されたとき
            Quit();
        }
    }

    void Quit()
    {
#if UNITY_EDITOR    // エディター
        UnityEditor.EditorApplication.isPlaying = false;
#elif UNITY_STANDALONE  // 実行
        UnityEngine.Application.Quit();
#endif
    }
}
