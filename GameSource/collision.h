#pragma once
#include <DirectXMath.h>

struct BoundingBox {
    DirectX::XMFLOAT3 minPosition;
    DirectX::XMFLOAT3 maxPosition;
};

struct HitResult {
    DirectX::XMFLOAT3   position = { 0,0,0 }; // レイとポリゴンの交点
    DirectX::XMFLOAT3   normal = { 0,0,0 };   // 衝突したポリゴンの法線ベクトル
    float               distance = 0.0f;      // レイの始点から交点までの距離
    int                 materialIndex = -1;   // 衝突したポリゴンのマテリアル番号
};

class Collision {
public:
    static bool boxVsBox(BoundingBox box1,BoundingBox box2);
    
    static bool RayVsPolygon(DirectX::XMFLOAT3& start,DirectX::XMFLOAT3& end,HitResult& result);
};