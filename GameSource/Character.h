#pragma once

#include <DirectXMath.h>


class Character {
protected:
    DirectX::XMFLOAT3 position = { 0,0,0 };
    DirectX::XMFLOAT3 angle = { 0,0,0 };
    DirectX::XMFLOAT3 scale = { 1,1,1 };
    DirectX::XMFLOAT4X4 transform = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
    };

    float velocity = 0.0f;
    float acceleration = 1.0f;

    int attack = 1;
    int hp = 0;
    int maxHp = 5;
    bool deathFlag = false; // 死亡フラグ
    bool battleFlag = false; // 戦闘フラグ
    bool attackRecast = false; // オフなら攻撃可能

public:
    Character(){}
    virtual ~Character(){}

    // 行列更新処理
    void UpdateTransform();

    // 位置取得
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // 位置設定
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // 回転取得
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }

    // 回転設定
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    // スケール取得
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    // スケール設定
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    // HP取得
    const int GetHP() const { return hp; }

    // HP設定
    void SetHP(int& hp) { this->hp = hp; }

    // 攻撃力取得
    const int GetAttack() const { return attack; }

    // 攻撃力設定
    void SetAttack(int& attack) { this->attack = attack; }
public:
    // フラグ設定
    void FlagOn(bool& flag) { if(flag == false) flag = true; }
    void FlagOff(bool& flag) { if(flag == true) flag = false; }

    // クールタイム
    void SetRecastTime(float second,bool& recastFlag,float elapsedTime);
protected:
    // キャラの行動ステート
    virtual void BehaviorState(float elapsedTime);

    // 移動処理
    virtual void Move(float elapsedTime);

    virtual void Battle(Character& dst); // あとでtemplateに変えるかも(建物にも対応するため)

    virtual void Attack(Character& dst);

    Character Find(Character& src);
};