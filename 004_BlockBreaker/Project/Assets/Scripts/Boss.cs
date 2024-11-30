using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Boss : Enemy
{
    // 静的メンバ変数
    private static float MOVE = 1.2f;   // 一度の移動量

    // メンバ変数定義
    [SerializeField] private EnemySpawn m_Spawn;        // スポーン情報
    [SerializeField] private int m_StartSetEnemy = 1;   // 初期配置敵数

    // Start is called before the first frame update
    void Start()
    {
        // 初期設定
        SetParent();        // 参照設定
        SetTimer();         // タイマー設定
        SetMovePosition();  // 移動開始地点リセット
        m_Spawn.SetTimer(); // タイマー設定
        m_Spawn.Spawn(0, m_StartSetEnemy, transform.position); // 一体配置
    }

    // Update is called once per frame
    void Update()
    {
        // スポーンカウント増加
        m_Spawn.SpawnAddCnt(transform.position);

        // タイマー増加
        AddTimer();

        // 移動
        Move();
    }

    // 前進する
    public void Forward()
    {
        // 座標を移動
        Vector3 pos = EndPos;
        pos.x -= MOVE;
        NextPosition(pos);
        Debug.Log("NextPositon");

        // 移動サウンドを鳴らす
        AudioClip clip = Instantiate((AudioClip)Resources.Load("se_bossmove"));
        AudioSource.PlayClipAtPoint(clip, transform.position);
    }

    // 討伐敵数増加
    public void AddDeadEnemy()
    {
        m_Spawn.AddDeadEnemy();
    }
}