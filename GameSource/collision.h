#pragma once
#include <DirectXMath.h>

struct BoundingBox {
    DirectX::XMFLOAT3 minPosition;
    DirectX::XMFLOAT3 maxPosition;
};

struct HitResult {
    DirectX::XMFLOAT3   position = { 0,0,0 }; // ���C�ƃ|���S���̌�_
    DirectX::XMFLOAT3   normal = { 0,0,0 };   // �Փ˂����|���S���̖@���x�N�g��
    float               distance = 0.0f;      // ���C�̎n�_�����_�܂ł̋���
    int                 materialIndex = -1;   // �Փ˂����|���S���̃}�e���A���ԍ�
};

class Collision {
public:
    static bool boxVsBox(BoundingBox box1,BoundingBox box2);
    
    static bool RayVsPolygon(DirectX::XMFLOAT3& start,DirectX::XMFLOAT3& end,HitResult& result);
};