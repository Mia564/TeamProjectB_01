#include "collision.h"

bool Collision::boxVsBox(BoundingBox box1,BoundingBox box2) {
    if (box1.maxPosition.y < box2.minPosition.y || box1.minPosition.y > box2.maxPosition.y) return false;

    if (box1.maxPosition.x < box2.minPosition.x || box1.minPosition.x > box2.maxPosition.x) return false;

    if (box1.maxPosition.z < box2.minPosition.z || box1.minPosition.z > box2.maxPosition.z) return false;

    return true;
}

bool Collision::RayVsPolygon(DirectX::XMFLOAT3& start, DirectX::XMFLOAT3& end, HitResult& result) {
    return true;
}