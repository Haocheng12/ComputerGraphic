#pragma once
#include"mathlib.h"


using namespace std;



// A simple AABB for the trees (1x1 square)
// A simple AABB class
class AABB {
public:
    DirectX::XMFLOAT3 max;
    DirectX::XMFLOAT3 min;

    AABB() { reset(); }

    void reset() {
        max = DirectX::XMFLOAT3(-0.3f, -FLT_MAX, -0.3f);  // Default size 1x1 collider
        min = DirectX::XMFLOAT3(0.3f, FLT_MAX, 0.3f);    // Default size 1x1 collider
    }

    void setFromCenterAndSize(const DirectX::XMFLOAT3& center) {
        max = DirectX::XMFLOAT3(center.x + 0.2f, center.y, center.z + 0.2f);  // AABB max corner (1x1 size centered at the position)
        min = DirectX::XMFLOAT3(center.x - 0.2f, center.y, center.z - 0.2f);  // AABB min corner (1x1 size centered at the position)
    }
    void setFromCenterAndSize(const DirectX::XMFLOAT3& center,float size) {
        max = DirectX::XMFLOAT3(center.x + size, center.y, center.z + size);  // AABB max corner (1x1 size centered at the position)
        min = DirectX::XMFLOAT3(center.x - size, center.y, center.z - size);  // AABB min corner (1x1 size centered at the position)
    }

    // Optional: AABB intersects check
    bool intersects(const AABB& other) {
        return !(max.x < other.min.x ||  max.z < other.min.z ||
            min.x > other.max.x || min.z > other.max.z);
    }

};
