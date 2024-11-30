using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class GameManager : MonoBehaviour
{
    // メンバ変数定義
    [SerializeField] protected float m_SceneTimer;    // 遷移タイマー
    private float m_NowTimer;     // 現在の移動タイマー
    private float m_AddTimer;     // タイマー加算量
    private bool m_IsClear;       // クリアフラグ
    private bool m_End = false;   // 終了フラグ

    // Start is called before the first frame update
    void Start()
    {
        // 値の初期設定
        m_IsClear = false;
        m_NowTimer = 0.0f;
        m_AddTimer = 1.0f / m_SceneTimer;
    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetKey(KeyCode.Escape))
        {// Escキーが押されたとき
            Quit();
        }

        // 遷移確認
        if (!m_End) // フラグが立っていない
        {
            // 終了確認
            CheckEnd();
            return;
        }

        // タイマーを進める
        m_NowTimer += m_AddTimer * Time.deltaTime;

        if(m_NowTimer < 1.0f) { return; }   // タイマー規定値前

        // リザルト用のパラメーターを渡す
        ResultUIManager.m_IsClear = m_IsClear;  // クリア設定
        SetScore(); // スコア設定

        // リザルトに遷移
        SceneManager.LoadScene("Result");
    }

    // 終了
    void Quit()
    {
#if UNITY_EDITOR    // エディター
        UnityEditor.EditorApplication.isPlaying = false;
#elif UNITY_STANDALONE  // 実行
        UnityEngine.Application.Quit();
#endif
    }

    // 終了設定
    public void SetEnd(bool IsClear)
    {
        m_End = true;
        m_IsClear = IsClear;
    }

    // リザルト用スコア設定
    private void SetScore()
    {
        // UIManagerタグのオブジェを取得
        GameObject obj = GameObject.FindWithTag("UIManager");
        if (obj == null) { return; }    // 取得できなかった

        // UIManagerを取得
        UIManager ui = obj.GetComponent<UIManager>();
        if (ui == null) { return; }    // 取得できなかった

        // スコアの設定
        ResultUIManager.m_Score = ui.Score;
    }

    // 終了確認
    private void CheckEnd()
    {
        // ボスが存在しているかチェックする
        {
            GameObject obj = GameObject.FindWithTag("Boss");

            if (obj == null)    // ボスが存在しない
            {
                // クリア設定
                SetEnd(true);
                return;
            }
        }

        // 弾が存在しているかチェックする
        BulletCheck();

        // 体力がまだあるかチェックする
        LifeCheck();
    }

    // 弾が存在しているか確認する
    private void BulletCheck()
    {
        // 弾タグを全て取得
        GameObject[] obj = GameObject.FindGameObjectsWithTag("Bullet");
        if (obj.Length > 0) { return; }  // 弾が画面上に存在する

        // UIManagerタグのオブジェを取得
        GameObject uiobj = GameObject.FindWithTag("UIManager");
        if (uiobj == null) { return; }    // 取得できなかった

        // UIManagerを取得
        UIManager ui = uiobj.GetComponent<UIManager>();
        if (ui == null) { return; }    // 取得できなかった
        if (ui.Bullet > 0) { return; }   // 残弾がある

        // ゲームオーバー設定
        SetEnd(false);
    }

    // 体力がまだあるかチェックする
    private void LifeCheck()
    {
        // UIManagerタグのオブジェを取得
        GameObject obj = GameObject.FindWithTag("UIManager");
        if (obj == null) { return; }    // 取得できなかった

        // UIManagerを取得
        UIManager ui = obj.GetComponent<UIManager>();
        if (ui == null) { return; }    // 取得できなかった
        if (ui.Life > 0) { return; }   // 体力がある

        // ゲームオーバー設定
        SetEnd(false);
    }
}
