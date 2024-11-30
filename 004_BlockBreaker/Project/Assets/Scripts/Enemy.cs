using System.Collections;
using System.Collections.Generic;
using UnityEngine;
 
public class Enemy : MonoBehaviour
{
    // 静的メンバ変数
    private static float MOVE = -1.0f;  // 一回の移動量

    // メンバ変数定義
    [SerializeField] private EnemyCore m_core;       // コアオブジェクト
    [SerializeField] private List<Block> m_blocks;    // ブロックリスト
    [SerializeField] private int m_MultiPly = 1;      // スコア倍率
    [SerializeField] protected int m_MoveTimer;       // 移動タイマー
    private float m_NowTimer;                         // 現在の移動タイマー
    private float m_AddTimer;                         // タイマー加算量
    private Vector3 m_StartPos;                       // 開始地点
    private Vector3 m_EndPos;                         // 目標地点

    // 目標地点プロパティの設定
    public Vector3 EndPos
    {
        get { return m_EndPos; }
        set { m_EndPos = value; }
    }

    // 現在のタイマープロパティの設定
    public float NowTimer
    {
        get { return m_NowTimer; }
        set { m_NowTimer = value; }
    }

    // Start is called before the first frame update
    void Start()
    {
        // 参照設定
        SetParent();

        // タイマー値設定
        SetTimer();

        // 初期目標地点設定
        SetMovePosition();
    }

    // Update is called once per frame
    void Update()
    {
        // タイマー増加
        AddTimer();

        // タイマー増加
        NextTimer();

        Move();
    }

    // リストから廃棄
    public void BlockListOut(Block block)
    {
        if(block != null)   // 存在している
        {
            m_blocks.Remove(block); // リストから削除
        }
    }

    // コアが破壊
    public void CoreDestroy(EnemyCore core)
    {
        if (m_core != core) { return; }  // 一致していない

        // ブロックを破壊する
        for (int i = 0; i < m_blocks.Count; i++)
        {
            m_blocks[i].Dead();
        }

        // コアも破壊する
        m_core.Dead();
        m_core = null;

        // 自身を廃棄する
        Destroy(gameObject);
    }

    // 死亡地点での削除
    public void DeadLineDestroy()
    {
        Destroy(gameObject);
    }

    // アイテムで破壊
    public void ItemDestroy()
    {
        // ブロックを破壊する
        for (int i = 0; i < m_blocks.Count; i++)
        {
            m_blocks[i].Dead();
        }

        // コアも破壊する
        m_core.Dead();
        m_core = null;

        // 自身を廃棄する
        Destroy(gameObject);
    }

    // タイマー増加
    protected void AddTimer()
    {
        if(m_NowTimer >= 1.0f) { return; }  // タイマー増加終了済み

        m_NowTimer += m_AddTimer * Time.deltaTime;

        if(m_NowTimer > 1.0f) { m_NowTimer = 1.0f; }    // タイマーが規定値を超えた
    }

    // タイマー移動
    protected void NextTimer()
    {
        if (m_NowTimer >= 1.0f)  // 移動タイマー規定値
        {
            m_NowTimer = 0.0f;  // タイマーリセット
            m_StartPos = m_EndPos;  // 移動開始地点を前回のゴール地点に
            m_EndPos.x += MOVE;     // ゴール地点を前へ進める
        }
    }

    // 移動
    protected void Move()
    {
        Vector3 pos = m_StartPos;   // 開始座標
        pos += (m_EndPos - m_StartPos) * m_NowTimer;    // 差分を補完
        transform.position = pos;   // 値を設定
    }

    // 移動後の座標を設定
    protected void NextPosition(Vector3 pos)
    {
        // 現在の位置から指定した地点をゴールにする
        m_NowTimer = 0.0f;  // タイマーリセット
        m_StartPos = transform.position;
        m_EndPos = pos;
    }

    // タイマー値設定
    protected void SetTimer()
    {
        m_AddTimer = 1.0f / m_MoveTimer;
        m_NowTimer = 0.0f;
    }

    // 開始終了地点リセット
    protected void SetMovePosition()
    {
        m_StartPos = transform.position;
        m_EndPos = m_StartPos;
    }

    // ブロックに参照設定
    protected void SetParent()
    {
        // ブロックに自身を渡す
        for (int i = 0; i < m_blocks.Count; i++)
        {
            m_blocks[i].SetParent(this, m_MultiPly);
        }

        // コアに自身を渡す
        if (m_core != null)
        {
            m_core.SetParent(this, m_MultiPly);
        }
    }
}
