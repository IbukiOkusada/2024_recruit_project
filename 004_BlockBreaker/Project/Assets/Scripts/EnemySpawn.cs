using System.Collections;
using System.Collections.Generic;
using UnityEngine;

// 敵出現クラス
[System.Serializable]
public class EnemySpawn
{
    // メンバ変数定義
    [SerializeField] private List<EnemyData> m_EnemyList;            // 敵配置情報
    [SerializeField] private List<SpawnLevelData> m_LevelInfoList;   // 各レベル配置情報
    [SerializeField] private int m_SpawnTimer;          // 生成までのタイマー
    [SerializeField] private float m_FrameCnt;          // カウント増加数
    [SerializeField] private float m_NowCnt;            // カウント
    [SerializeField] private float[] m_aPositionY;      // 敵配置高さ情報
    private int m_NowLevel;     // 現在のレベル
    private int m_DeadEnemyCnt; // 討伐された敵の数

    // Start is called before the first frame update
    void Start()
    {
        // 初期値設定
        SetTimer(); // タイマー
    }

    // Update is called once per frame
    void Update()
    {

    }

    // 敵の生成カウント増加(基本座標)
    public void SpawnAddCnt(Vector3 pos)
    {
        // カウント増加
        m_NowCnt += m_FrameCnt * Time.deltaTime;

        GameObject[] EnemyTagObj = GameObject.FindGameObjectsWithTag("Enemy");  // 敵タグを全て取得

        if (EnemyTagObj == null || m_NowCnt >= 1.0f) // 敵がいないもしくは敵が規定値
        {
            m_NowCnt = 0.0f;
            SpawnCheck(pos);   // 生成確認
        }
    }

    // 生成確認(基本座標)
    private void SpawnCheck(Vector3 pos)
    {
        // 現在のレベルの生成する敵レベルまで繰り返し
        for (int i = 0; i < m_LevelInfoList[m_NowLevel].m_aCnt.Length; i++)
        {
            Spawn(i, m_LevelInfoList[m_NowLevel].m_aCnt[i], pos);   // 生成
        }
    }

    // 指定レベルの敵を生成
    public void Spawn(int Level, int Num, Vector3 pos)
    {
        int cnt = 0;    // 指定レベルと同じ敵の数

        // データを抽出
        for (int i = 0; i < m_EnemyList.Count; i++)
        {
            if (m_EnemyList[i].m_Level == Level) // レベルが一致
            {
                cnt++;
            }
        }

        if (cnt <= 0) { Debug.Log("enemynull!"); return; }    // 存在していなかった

        // 敵を格納する場所を用意
        EnemyData[] ExEnemy = new EnemyData[cnt];
        int excnt = 0;  // 格納場所の配列№

        // データを借入
        for (int i = 0; i < m_EnemyList.Count; i++)
        {
            if (m_EnemyList[i].m_Level == Level) // レベルが一致
            {
                ExEnemy[excnt] = m_EnemyList[i];
                excnt++;
            }
        }

        // 指定数ランダムで配置
        for (int i = 0; i < Num; i++)
        {
            // それぞれランダムで要素を出す
            int rand = Random.Range(0, cnt);  // 種類
            int Heightrand = Random.Range(0, m_aPositionY.Length);  // 高さ

            // 座標を設定
            Vector3 SetPos = pos;
            SetPos.y += m_aPositionY[Heightrand];
            SetPos.x += -0.5f;

            // 敵を配置
            GameObject.Instantiate(ExEnemy[rand].m_Prefab, SetPos, Quaternion.identity);
        }
    }

    // 討伐数増加
    public void AddDeadEnemy()
    {
        if (m_NowLevel + 1 >= m_LevelInfoList.Count) { return; }    // 最大レベル

        // 死亡数増加
        m_DeadEnemyCnt++;

        if (m_DeadEnemyCnt % 2 == 0)    // 2体に一回
        {
            // 出現レベルを上げる
            m_NowLevel++;
        }
    }

    // タイマー値設定
    public void SetTimer()
    {
        m_NowLevel = 0;
        m_FrameCnt = 1.0f / m_SpawnTimer;
    }
}