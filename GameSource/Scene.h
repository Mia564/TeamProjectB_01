#pragma once

class Scene {
private:
    bool ready = false;
public:
    Scene() {}
    virtual ~Scene() {}

    // ������
    virtual void Initialize() = 0;

    // �I����
    virtual void Finalize() = 0;

    // �X�V����
    virtual void Update(float elapsedTime) = 0;

    // �`�揈��
    virtual void Render() = 0;

    bool IsReady() const { return ready; }

    // ���������ݒ�
    void SetReady() { ready = true; }
};