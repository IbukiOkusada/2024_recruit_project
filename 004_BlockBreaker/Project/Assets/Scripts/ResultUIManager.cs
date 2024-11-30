using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;

public class ResultUIManager : MonoBehaviour
{
    // 静的メンバ変数
    public static int m_Score = 0;  // リザルトスコア
    public static bool m_IsClear = false;   // クリアフラグ

    // メンバ変数定義
    [SerializeField] private TextMeshProUGUI m_GameOverText;   // 体力用テキスト
    [SerializeField] private TextMeshProUGUI m_GameClearText; // 弾数用テキスト
    [SerializeField] private TextMeshProUGUI m_ScoreText;  // スコア用テキスト

    // Start is called before the first frame update
    void Start()
    {
        // クリア文字設定
        SetText();

        // スコア設定
        SetScore();
    }

    // Update is called once per frame
    void Update()
    {

    }

    // クリアテキスト設定
    private void SetText()
    {
        bool GameOver = false;
        bool GameClear = false;

        // クリア確認
        if(m_IsClear)   // ゲームクリア
        {
            GameClear = true;
        }
        else  // げーむおーばー
        {
            GameOver = true;
        }

        // 表示非表示設定
        m_GameOverText.enabled = GameOver;
        m_GameClearText.enabled = GameClear;

        // フラグリセット
        m_IsClear = false;
    }

    // スコア設定
    private void SetScore()
    {
        // テキスト書き換え
        m_ScoreText.text = "Score: " + m_Score.ToString("D8");

        // スコアリセット
        m_Score = 0;
    }
}
