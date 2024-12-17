#pragma once
#pragma once
#include <DirectXMath.h>
#include <map>
#include <string>
#include <vector>

using namespace DirectX;

struct ANIMATED_VERTEX
{
    XMFLOAT3 pos;
    XMFLOAT3 normal;
    XMFLOAT3 tangent;
    float tu;
    float tv;
    unsigned int bonesIDs[4];
    float boneWeights[4];
};

struct Bone
{
    std::string name;
    XMMATRIX offset;
    int parentIndex;
};

struct Skeleton
{
    std::vector<Bone> bones;
    XMMATRIX globalInverse;
};

struct AnimationFrame
{
    std::vector<XMFLOAT3> positions;
    std::vector<XMVECTOR> rotations;
    std::vector<XMFLOAT3> scales;
};

class AnimationSequence
{
public:
    std::vector<AnimationFrame> frames;
    float ticksPerSecond;

    XMFLOAT3 interpolate(const XMFLOAT3& p1, const XMFLOAT3& p2, float t) {
        return XMFLOAT3(
            p1.x * (1.0f - t) + p2.x * t,
            p1.y * (1.0f - t) + p2.y * t,
            p1.z * (1.0f - t) + p2.z * t
        );
    }

    XMVECTOR interpolate(const XMVECTOR& q1, const XMVECTOR& q2, float t) {
        return XMQuaternionSlerp(q1, q2, t);
    }

    float duration() const {
        return frames.size() / ticksPerSecond;
    }

    void calcFrame(float t, int& frame, float& interpolationFact) {
        interpolationFact = t * ticksPerSecond;
        frame = static_cast<int>(floorf(interpolationFact));
        interpolationFact -= frame;
        frame = min(frame, static_cast<int>(frames.size() - 1));
    }

    int nextFrame(int frame) const {
        return min(frame + 1, static_cast<int>(frames.size() - 1));
    }

    XMMATRIX interpolateBoneToGlobal(const XMMATRIX* matrices, int baseFrame, float interpolationFact, const Skeleton* skeleton, int boneIndex) {
        XMFLOAT3 scale = interpolate(frames[baseFrame].scales[boneIndex], frames[nextFrame(baseFrame)].scales[boneIndex], interpolationFact);
        XMVECTOR rotation = interpolate(XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&frames[baseFrame].rotations[boneIndex])),
            XMLoadFloat4(reinterpret_cast<const XMFLOAT4*>(&frames[nextFrame(baseFrame)].rotations[boneIndex])), interpolationFact);
        XMFLOAT3 translation = interpolate(frames[baseFrame].positions[boneIndex], frames[nextFrame(baseFrame)].positions[boneIndex], interpolationFact);

        XMMATRIX local =  XMMatrixScaling(scale.x, scale.y, scale.z)*
            XMMatrixRotationQuaternion(rotation) *XMMatrixTranslation(translation.x, translation.y, translation.z)
            ;

        if (skeleton->bones[boneIndex].parentIndex > -1) {
            return local* matrices[skeleton->bones[boneIndex].parentIndex]  ;
        }
        return local;
    }
};

class Animation
{
public:
    std::map<std::string, AnimationSequence> animations;
    Skeleton skeleton;

    void calcFrame(const std::string& name, float t, int& frame, float& interpolationFact) {
        animations[name].calcFrame(t, frame, interpolationFact);
    }

    XMMATRIX interpolateBoneToGlobal(const std::string& name, XMMATRIX* matrices, int baseFrame, float interpolationFact, int boneIndex) {
        return animations[name].interpolateBoneToGlobal(matrices, baseFrame, interpolationFact, &skeleton, boneIndex);
    }

    void calcFinalTransforms(XMMATRIX* matrices) {
        for (size_t i = 0; i < skeleton.bones.size(); ++i) {
            matrices[i] =  /*skeleton.bones[i].offset*/ matrices[i] * skeleton.globalInverse;
        }
    }


    

    
};

class AnimationInstance
{
public:
    Animation* animation;
    std::string currentAnimation;
    float t;
    XMMATRIX matrices[256];


    AnimationInstance() {
        for (int i = 0; i < 256; ++i) {
            matrices[i] = XMMatrixIdentity();
        }
    }
    void resetAnimationTime() {
        t = 0;
    }

    bool animationFinished() const {
        return t > animation->animations.at(currentAnimation).duration();
    }

    void update(const std::string& name, float dt) {
        if (name == currentAnimation) {
            t += dt;
        }
        else {
            currentAnimation = name;
            resetAnimationTime();
        }

        if (animationFinished()) {
            resetAnimationTime();
        }

        int frame = 0;
        float interpolationFact = 0;
        animation->calcFrame(name, t, frame, interpolationFact);

        for (size_t i = 0; i < animation->skeleton.bones.size(); ++i) {
            matrices[i] = animation->interpolateBoneToGlobal(name, matrices, frame, interpolationFact, static_cast<int>(i));
        }
        animation->calcFinalTransforms(matrices);
    }
};
