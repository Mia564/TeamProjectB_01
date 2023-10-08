#include "Character.h"

void Character::UpdateTransform() {
    using namespace DirectX;

    XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);
    XMMATRIX R = XMMatrixScaling(angle.x, angle.y, angle.z);
    XMMATRIX T = XMMatrixScaling(position.x, position.y, position.z);

    XMMATRIX W = S * R * T; // ワールド行列を作成

    XMStoreFloat4x4(&transform, W);
}

void Character::BehaviorState(float elapsedTime) {
    if (battleFlag) {
        //Battle(); //こんな感じに作りたいけどうまいこと引数を取ってくる方法が必要 下にあるfindで戦闘相手のデータを取ってこようと思索中
    }
    if (!battleFlag) {
        Move(elapsedTime);
    }
}

void Character::Move(float elapsedTime) {
    using namespace DirectX;

    // キャラの移動
    XMVECTOR Position = XMVectorSet(position.x, position.y, position.z, 0.0f);
    XMVECTOR Forword = XMVectorSet(transform._31, transform._32, transform._33, transform._34);
    Forword = XMVector3Normalize(Forword);
    XMStoreFloat3(&position, XMVectorAdd(Position, XMVectorScale(Forword, velocity * elapsedTime)));

    // 敵や建物との衝突判定
}

void Character::Battle(Character& dst) {
    // 戦闘時の処理
    if (!dst.deathFlag) {
        Attack(dst);
    }
    else {
        FlagOff(battleFlag);
    }
}

void Character::Attack(Character& dst) {
    int afterHp = dst.GetHP() - this->GetAttack();

    dst.SetHP(afterHp);

    // 死亡処理
    if (dst.GetHP() <= 0) {
        FlagOn(dst.deathFlag);
    }
}

// ここで作らんかも
void Character::SetRecastTime(float second, bool& recastFlag,float elapsedTime) {
 //   if(second > )

    FlagOff(recastFlag);
}

Character Character::Find(Character& src) {
    return src;
}