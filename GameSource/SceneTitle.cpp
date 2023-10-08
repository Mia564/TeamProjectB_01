#include "SceneTitle.h"

// 初期化
void SceneTitle::Initialize() {
    // スプライト初期化
}

// 終了化
void SceneTitle::Finalize() {
    // スプライト終了化
    if (sprite != nullptr) {
        delete sprite;
        sprite = nullptr;
    }
}

// 更新処理
void SceneTitle::Update(float elapsedTime) {

}

// 描画処理
void SceneTitle::Render() {

}