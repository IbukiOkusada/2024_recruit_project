using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    // 静的メンバ変数
    private static int MAX_BULLET = 4;  // 最大まで画面上に発射できる弾数

    // メンバ変数定義
    [SerializeField] private float m_Speed;    // 移動速度
    [SerializeField] private float m_Iner;     // 移動慣性
    [SerializeField] private float m_MaxSize;  // 最大高さ
    [SerializeField] private GameObject m_bulletprefab; // 弾のプレハブ
    private Vector3 m_move;         // 移動量
    private Rigidbody m_rigitBody;  // 自身のリジットボディ

    // Start is called before the first frame update
    void Start()
    {
        // 取得
        m_rigitBody = GetComponent<Rigidbody>();
    }

    // Update is called once per frame
    void Update()
    {
        // リセット
        if(m_rigitBody != null){ m_rigitBody.velocity = new Vector3(0.0f, 0.0f, 0.0f); }

        // 移動
        Move();

        // 発射
        Shot();
    }

    // 移動
    void Move()
    {
        if (Input.GetKey(KeyCode.W))
        {// Wキーが押されたとき
            m_move.y += Mathf.Cos(Mathf.PI * 0.0f) * (m_Speed * Time.deltaTime);
        }
        else if (Input.GetKey(KeyCode.S))
        {// Sキーが押されたとき
            m_move.y += Mathf.Cos(Mathf.PI * 1.0f) * (m_Speed * Time.deltaTime);
        }

        // 移動量減衰
        m_move.x -= m_move.x * m_Iner;  //x座標
        m_move.y -= m_move.y * m_Iner;	//y座標

        // 座標更新
        transform.Translate(m_move.x, m_move.y, m_move.z, Space.World); // 移動量を加算
    }

    // 弾発射
    void Shot()
    {
        // Spaceキーが押されたときのみ進む
        if (!Input.GetKeyDown(KeyCode.Space)) { return; }

        // 画面上の弾数確認
        {
            GameObject[] bullet = GameObject.FindGameObjectsWithTag("Bullet");

            if(bullet.Length > MAX_BULLET) { return; }   // 画面上に指定数以上
        }

        // エラーチェック
        if (m_bulletprefab == null) { return; }  // プレハブが存在しない

        // UIマネージャーを取得する
        GameObject obj = GameObject.FindWithTag("UIManager");
        if (obj == null) { return; } // UIマネージャータグが取得できなかった
        UIManager ui = obj.GetComponent<UIManager>();
        if (ui == null) { return; }  // UIマネージャーが取得できなかった

        // 残段数を確認する
        if (ui.Bullet <= 0) // 残弾がない
        {
            // 発射失敗サウンドを鳴らす
            AudioClip clip = Instantiate((AudioClip)Resources.Load("se_noshot"));
            AudioSource.PlayClipAtPoint(clip, transform.position);

            return; 
        }

        // 弾を発射
        Vector3 SetPos = transform.position;
        SetPos.x += 0.5f;
        Instantiate(m_bulletprefab, SetPos, Quaternion.identity);
        ui.AddBullet(-1);   // 残弾を減らす

        // 発射サウンドを鳴らす
        {
            AudioClip clip = Instantiate((AudioClip)Resources.Load("se_shot"));
            AudioSource.PlayClipAtPoint(clip, transform.position);
        }
    }

    // ダメージ
    public void Damage()
    {
        // UIManagerタグのオブジェを取得
        GameObject obj = GameObject.FindWithTag("UIManager");
        if (obj == null) { return; }    // 取得できなかった

        // UIManagerを取得
        UIManager ui = obj.GetComponent<UIManager>();
        if (ui == null) { return; }    // 取得できなかった
        ui.AddLife(-1); // 体力を減らす
    }
}
