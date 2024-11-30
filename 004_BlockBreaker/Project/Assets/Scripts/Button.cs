using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class Button : MonoBehaviour
{
    // 押された
    public void OnClick(string nextScene)
    {
        // 指定されたシーンに遷移
        SceneManager.LoadScene(nextScene);
    }

    // 終了ボタンが押された
    public void ClickEnd()
    {
#if UNITY_EDITOR    // エディター
        UnityEditor.EditorApplication.isPlaying = false;
#elif UNITY_STANDALONE  // 実行
        UnityEngine.Application.Quit();
#endif
    }
}
