#pragma once

#include "../Library/sprite.h"
#include "Scene.h"

// �^�C�g���V�[��
class SceneTitle : public Scene {
public:
    SceneTitle() {}
    ~SceneTitle() override {}

    // ������
    void Initialize() override;

    // �I����
    void Finalize() override;

    // �X�V����
    void Update(float elapsedTime) override;

    // �`�揈��
    void Render() override;

private:
    Sprite* sprite = nullptr;
};