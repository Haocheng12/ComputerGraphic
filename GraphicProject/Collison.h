#pragma once
#include"mathlib.h"


using namespace std;



class AABB {
public:
    DirectX::XMFLOAT3 max;
    DirectX::XMFLOAT3 min;

    AABB() { reset(); }

    void reset() {
        max = DirectX::XMFLOAT3(-0.3f, -FLT_MAX, -0.3f);  
        min = DirectX::XMFLOAT3(0.3f, FLT_MAX, 0.3f);    
    }

    void setFromCenterAndSize(const DirectX::XMFLOAT3& center) {
        max = DirectX::XMFLOAT3(center.x + 0.2f, center.y, center.z + 0.2f);  
        min = DirectX::XMFLOAT3(center.x - 0.2f, center.y, center.z - 0.2f); 
    }
    void setFromCenterAndSize(const DirectX::XMFLOAT3& center,float size) {
        max = DirectX::XMFLOAT3(center.x + size, center.y, center.z + size);  
        min = DirectX::XMFLOAT3(center.x - size, center.y, center.z - size);  
    }

    // Optional: AABB intersects check
    bool intersects(const AABB& other) {
        return !(max.x < other.min.x ||  max.z < other.min.z ||
            min.x > other.max.x || min.z > other.max.z);
    }

};
