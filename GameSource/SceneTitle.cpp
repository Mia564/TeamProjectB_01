#include "SceneTitle.h"

// ������
void SceneTitle::Initialize() {
    // �X�v���C�g������
}

// �I����
void SceneTitle::Finalize() {
    // �X�v���C�g�I����
    if (sprite != nullptr) {
        delete sprite;
        sprite = nullptr;
    }
}

// �X�V����
void SceneTitle::Update(float elapsedTime) {

}

// �`�揈��
void SceneTitle::Render() {

}