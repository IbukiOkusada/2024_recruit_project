using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Bullet : MonoBehaviour
{
    // メンバ変数定義
    [SerializeField] private float m_speed; // 初速度
    private Rigidbody m_rigitBody;          // リジットボディ
    private float m_StartSpeed;             // 開始速度

    // Start is called before the first frame update
    void Start()
    {
        // 初期速度を設定
        m_rigitBody = GetComponent<Rigidbody>();    // 取得
        m_rigitBody.velocity = new Vector3(m_speed, m_speed, 0);    // 設定

        // 開始速度を保存
        m_StartSpeed = m_speed;
    }

    // Update is called once per frame
    void Update()
    {
        // 画面内判定
        ScreenCollision();

        // 速度確認
        SpeedCheck();
    }

    // 画面範囲内かを確認
    void ScreenCollision()
    {
        Vector3 pos = transform.position;
        Vector3 move = m_rigitBody.velocity;

        // 画面の判定座標を取得
        Vector3 leftDown = Camera.main.ScreenToWorldPoint(new Vector2(0.0f, 0.0f)); // 左下の座標
        Vector3 rightUp = Camera.main.ScreenToWorldPoint(new Vector2(Screen.width, Screen.height)); // 右上の座標

        // 当たり判定
        if(pos.x >= rightUp.x)  // 右端
        {
            move.x *= -1.0f;
        }

        // 移動量を設定
        m_rigitBody.velocity = move;
    }

    // 衝突
    private void OnCollisionEnter(Collision collider)
    {
        if (collider.gameObject.CompareTag("Player"))   // 衝突したのがプレイヤー
        {
            Vector3 PlayerPos = collider.transform.position;    // プレイヤーの座標
            Vector3 MyPos = transform.position;                 // 自身の座標

            // 当たった角度を取得
            Vector3 vec = MyPos - PlayerPos;
            vec = vec.normalized;   // 正規化

            // 速度を求めて設定
            float speed = m_rigitBody.velocity.magnitude;
            speed += 0.1f;  // 速度を加速
            m_rigitBody.velocity = vec * speed;
        }
        else if (collider.gameObject.CompareTag("DeadLine"))   // 衝突したのが死亡地点
        {
            // 死亡サウンドを鳴らす
            AudioClip clip = Instantiate((AudioClip)Resources.Load("se_bulletdead"));
            AudioSource.PlayClipAtPoint(clip, transform.position);

            // エフェクト
            Instantiate((GameObject)Resources.Load("BulletLineEffect"), transform.position, Quaternion.identity);

            // 自身の削除
            Destroy(gameObject);
            return;
        }

        // 衝突サウンドを鳴らす
        {
            AudioClip clip = Instantiate((AudioClip)Resources.Load("se_playerhit"));
            AudioSource.PlayClipAtPoint(clip, transform.position);
        }

        // エフェクトの生成
        if (!collider.gameObject.CompareTag("Bullet"))   // 弾との衝突以外
        {
            Instantiate((GameObject)Resources.Load("BulletHitEffect"), transform.position, Quaternion.identity);
        }
    }

    // 座標設定
    public void SetPosition(Vector3 pos)
    {
        transform.position = pos;
    }

    // 速度確認
    private void SpeedCheck()
    {
        float nowspeed = m_rigitBody.velocity.magnitude;    // 現在の速度

        if(nowspeed >= m_StartSpeed) { return; }    // 初速より速い

        // 現在の移動方向を取得
        Vector3 vec = m_rigitBody.velocity;
        vec = vec.normalized;   // 正規化

        // 移動量を初期値に再設定
        m_rigitBody.velocity = vec * m_StartSpeed;
    }
}
