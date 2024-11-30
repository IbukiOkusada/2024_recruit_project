using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using TMPro;

public class UIManager : MonoBehaviour
{
    // メンバ変数定義
    [SerializeField] private TextMeshProUGUI m_LifeText;   // 体力用テキスト
    [SerializeField] private TextMeshProUGUI m_BulletText; // 弾数用テキスト
    [SerializeField] private TextMeshProUGUI m_ScoreText;  // スコア用テキスト
    [SerializeField] private int m_Life;        // 体力
    [SerializeField] private int m_Bullet;      // 弾
    private int m_Score = 0;                    // スコア

    //---------------
    // プロパティ定義
    //---------------
    // 体力
    public int Life
    {
        get { return m_Life; }
        set { m_Life = value; }
    }

    // 弾数
    public int Bullet
    {
        get { return m_Bullet; }
        set { m_Bullet = value; }
    }

    // スコア
    public int Score
    {
        get { return m_Score; }
        set { m_Score = value; }
    }

    // Start is called before the first frame update
    void Start()
    {
        // 初期テキスト設定
        AddLife(0);
        AddScore(0);
        AddBullet(0);
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    // 体力増加
    public void AddLife(int add)
    {
        if (m_LifeText == null) { return; } // テキスト無し

        Life = Life + add;

        if (Life < 0) { Life = 0; }    // 0以下のスコア

        // テキスト書き換え
        m_LifeText.text = "× " + Life.ToString("D2");
    }

    // 弾数増加
    public void AddBullet(int add)
    {
        if (m_BulletText == null) { return; } // テキスト無し

        Bullet = Bullet + add;

        if (Bullet < 0) { Bullet = 0; }    // 0以下のスコア

        // テキスト書き換え
        m_BulletText.text = "× " + Bullet.ToString("D2");
    }

    // スコア増加
    public void AddScore(int add)
    {
        if(m_ScoreText == null) { return; } // テキスト無し

        Score = Score + add;

        if(Score < 0) { Score = 0; }    // 0以下のスコア

        // テキスト書き換え
        m_ScoreText.text = "Score: " + Score.ToString("D8");
    }
}
