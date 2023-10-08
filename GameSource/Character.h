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
    bool deathFlag = false; // ���S�t���O
    bool battleFlag = false; // �퓬�t���O
    bool attackRecast = false; // �I�t�Ȃ�U���\

public:
    Character(){}
    virtual ~Character(){}

    // �s��X�V����
    void UpdateTransform();

    // �ʒu�擾
    const DirectX::XMFLOAT3& GetPosition() const { return position; }

    // �ʒu�ݒ�
    void SetPosition(const DirectX::XMFLOAT3& position) { this->position = position; }

    // ��]�擾
    const DirectX::XMFLOAT3& GetAngle() const { return angle; }

    // ��]�ݒ�
    void SetAngle(const DirectX::XMFLOAT3& angle) { this->angle = angle; }

    // �X�P�[���擾
    const DirectX::XMFLOAT3& GetScale() const { return scale; }

    // �X�P�[���ݒ�
    void SetScale(const DirectX::XMFLOAT3& scale) { this->scale = scale; }

    // HP�擾
    const int GetHP() const { return hp; }

    // HP�ݒ�
    void SetHP(int& hp) { this->hp = hp; }

    // �U���͎擾
    const int GetAttack() const { return attack; }

    // �U���͐ݒ�
    void SetAttack(int& attack) { this->attack = attack; }
public:
    // �t���O�ݒ�
    void FlagOn(bool& flag) { if(flag == false) flag = true; }
    void FlagOff(bool& flag) { if(flag == true) flag = false; }

    // �N�[���^�C��
    void SetRecastTime(float second,bool& recastFlag,float elapsedTime);
protected:
    // �L�����̍s���X�e�[�g
    virtual void BehaviorState(float elapsedTime);

    // �ړ�����
    virtual void Move(float elapsedTime);

    virtual void Battle(Character& dst); // ���Ƃ�template�ɕς��邩��(�����ɂ��Ή����邽��)

    virtual void Attack(Character& dst);

    Character Find(Character& src);
};