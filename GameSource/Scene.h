#pragma once

class Scene {
private:
    bool ready = false;
public:
    Scene() {}
    virtual ~Scene() {}

    // ‰Šú‰»
    virtual void Initialize() = 0;

    // I—¹‰»
    virtual void Finalize() = 0;

    // XVˆ—
    virtual void Update(float elapsedTime) = 0;

    // •`‰æˆ—
    virtual void Render() = 0;

    bool IsReady() const { return ready; }

    // €”õŠ®—¹İ’è
    void SetReady() { ready = true; }
};