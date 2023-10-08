#include "Character.h"

void Character::UpdateTransform() {
    using namespace DirectX;

    XMMATRIX S = XMMatrixScaling(scale.x, scale.y, scale.z);
    XMMATRIX R = XMMatrixScaling(angle.x, angle.y, angle.z);
    XMMATRIX T = XMMatrixScaling(position.x, position.y, position.z);

    XMMATRIX W = S * R * T; // ���[���h�s����쐬

    XMStoreFloat4x4(&transform, W);
}

void Character::BehaviorState(float elapsedTime) {
    if (battleFlag) {
        //Battle(); //����Ȋ����ɍ�肽�����ǂ��܂����ƈ���������Ă�����@���K�v ���ɂ���find�Ő퓬����̃f�[�^������Ă��悤�Ǝv����
    }
    if (!battleFlag) {
        Move(elapsedTime);
    }
}

void Character::Move(float elapsedTime) {
    using namespace DirectX;

    // �L�����̈ړ�
    XMVECTOR Position = XMVectorSet(position.x, position.y, position.z, 0.0f);
    XMVECTOR Forword = XMVectorSet(transform._31, transform._32, transform._33, transform._34);
    Forword = XMVector3Normalize(Forword);
    XMStoreFloat3(&position, XMVectorAdd(Position, XMVectorScale(Forword, velocity * elapsedTime)));

    // �G�⌚���Ƃ̏Փ˔���
}

void Character::Battle(Character& dst) {
    // �퓬���̏���
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

    // ���S����
    if (dst.GetHP() <= 0) {
        FlagOn(dst.deathFlag);
    }
}

// �����ō��񂩂�
void Character::SetRecastTime(float second, bool& recastFlag,float elapsedTime) {
 //   if(second > )

    FlagOff(recastFlag);
}

Character Character::Find(Character& src) {
    return src;
}