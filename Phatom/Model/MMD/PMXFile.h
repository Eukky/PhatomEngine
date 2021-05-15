#ifndef PHATOM_PMXFILEDEFINE_H
#define PHATOM_PMXFILEDEFINE_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Common/File.h"

namespace phatom {

template <size_t Size>
struct MMDFileString
{
    MMDFileString()
    {
        Clear();
    }

    void Clear()
    {
        for (auto& ch : m_buffer)
        {
            ch = '\0';
        }
    }

    void Set(const char* str)
    {
        size_t i = 0;
        while (i < Size && str[i] != '\0')
        {
            m_buffer[i] = str[i];
            i++;
        }

        for (; i < Size + 1; i++)
        {
            m_buffer[i] = '\0';
        }
    }

    const char* ToCString() const {
        return m_buffer;
    }
    std::string ToString() const {
        return std::string(m_buffer);
    }
    std::string ToUtf8String() const;

    char    m_buffer[Size + 1];
};

template <size_t Size>
bool Read(MMDFileString<Size>* str, File& file)
{
    return file.Read(str->m_buffer, Size);
}

struct PMXHeader {
    MMDFileString<4> mSignature;
    float mVersion;

    uint8_t mDataSize;
    uint8_t mEncode;
    uint8_t mAddUVNum;

    uint8_t mVertexIndexSize;
    uint8_t mTextureIndexSize;
    uint8_t mMaterialIndexSize;
    uint8_t mBoneIndexSize;
    uint8_t mMorphIndexSize;
    uint8_t mRigidbodyIndexSize;
};

struct PMXInfo {
    std::string mModelName;
    std::string mEnglishModelName;
    std::string mComment;
    std::string mEnglishComment;
};

enum class PMXVertexWeight : uint8_t {
    BDEF1,
    BDEF2,
    BDEF4,
    SDEF,
    QDEF,
};

struct PMXVertex {
    glm::vec3 mPosition;
    glm::vec3 mNormal;
    glm::vec2 mUV;

    glm::vec4 mAddUV;

    PMXVertexWeight mWeightType;
    int32_t mBoneIndices[4];
    float mBoneWeights[4];
    glm::vec3 mSDEFC;
    glm::vec3 mSDEFR0;
    glm::vec3 mSDEFR1;
    float mEdgeScale;
};

struct PMXFace {
    uint32_t mVertices[3];
};

struct PMXTexture {
    std::string mTextureName;
};

enum class PMXDrawModeFlag : uint8_t {
    NoCull = 0x00,
    BothFace = 0x01,
    GroundShadow = 0x02,
    DrawShadow = 0x04,
    RecieveShadow = 0x08,
    HasEdge = 0x10,
    VertexColor = 0x20,
    PointDrawing = 0x40,
    LineDrawing = 0x80,
};

enum class PMXSphereMode : uint8_t {
    None,
    Multiply,
    Additive,
    SubTexture,
};

enum class PMXToonMode : uint8_t {
    Separate,
    Common,
};

struct PMXMaterial {
    std::string mName;
    std::string mEnglishName;

    glm::vec4 mDiffuse;
    glm::vec3 mSpecular;
    float mSpecularPower;
    glm::vec3 mAmbient;

    PMXDrawModeFlag mDrawMode;

    glm::vec4 mEdgeColor;
    float mEdgeSize;

    int32_t mTextureIndex;
    int32_t mSphereTextureIndex;
    PMXSphereMode mSphereMode;

    PMXToonMode mToonMode;
    int32_t mToonTextureIndex;

    std::string mMemo;

    int32_t mFaceNum;
};

enum class PMXBoneFlag : uint16_t {
    TargetShowMode = 0x0001,
    AllowRotate = 0x0002,
    AllowTranslate = 0x0004,
    Visible = 0x0008,
    AllowControl = 0x0010,
    IK = 0x0020,
    AppendLocal = 0x0080,
    AppendRotate = 0x0100,
    AppendTranslate = 0x0200,
    FixedAxis = 0x0400,
    LocalAxis = 0x800,
    DeformAfterPhysics = 0x1000,
    DeformOuterParent = 0x2000,
};

struct PMXIKLink {
    int32_t mIKBoneIndex;
    unsigned char mEnableLimit;

    glm::vec3 mLimitMin;
    glm::vec3 mLImitMax;
};

struct PMXBone {
    std::string mName;
    std::string mEnglishName;

    glm::vec3 mPositiion;
    int32_t mParentBoneIndex;
    int32_t mDeformDepth;

    PMXBoneFlag mBoneFlag;

    glm::vec3 mPositionOffset;
    int32_t mLinkBoneIndex;

    int32_t mAppendBoneIndex;
    float mAppendWeight;

    glm::vec3 mFixedAxis;

    glm::vec3 mLocalXAxis;
    glm::vec3 mLocalZAxis;

    int32_t mKeyValue;

    int32_t mIKTargetBoneIndex;
    int32_t mIKIterationCount;
    float mIKLimit;

    std::vector<PMXIKLink> mIKLinks;
};

enum class PMXMorphType : uint8_t {
    Group,
    Position,
    Bone,
    UV,
    AddUV1,
    AddUV2,
    AddUV3,
    AddUV4,
    Material,
    Filp,
    Impluse,
};

struct PositionMorph {
    int32_t mVertexIndex;
    glm::vec3 mPosition;
};

struct UVMorph {
    int32_t mVertexIndex;
    glm::vec4 mUV;
};

struct BoneMorph {
    int32_t		mBoneIndex;
    glm::vec3	mPosition;
    glm::quat	mQuaternion;
};

enum class MaterialMotphOpType : uint8_t {
    Multiply,
    Additive,
};

struct MaterialMorph {
    int32_t mMaterialIndex;
    MaterialMotphOpType mOpType;
    glm::vec4 mDiffuse;
    glm::vec3 mSpecular;
    float mSpecularPower;
    glm::vec3 mAmbient;
    glm::vec4 mEdgeColor;
    float mEdgeSize;
    glm::vec4 mTextureFactor;
    glm::vec4 mSphereTextureFactor;
    glm::vec4 mToonTextureFactor;
};

struct GroupMorph {
    int32_t mMorphIndex;
    float mWeight;
};

struct FlipMorph {
    int32_t mMorphIndex;
    float mWeight;
};

struct ImpulseMorph {
    int32_t mRigidbodyIndex;
    uint8_t mLocalFlag;
    glm::vec3 mTranslateVelocity;
    glm::vec3 mRotateTorque;
};

struct PMXMorph {
    std::string mName;
    std::string mEnglishName;

    uint8_t mControlPanel;
    PMXMorphType mMorphType;

    std::vector<PositionMorph> mPositionMorph;
    std::vector<UVMorph> mUVMorph;
    std::vector<BoneMorph> mBornMorph;
    std::vector<MaterialMorph> mMaterialMorph;
    std::vector<GroupMorph> mGroupMorph;
    std::vector<FlipMorph> mFlipMorph;
    std::vector<ImpulseMorph> mImpulseMorph;
};

enum class FrameTargetType : uint8_t {
    BoneIndex,
    MorphIndex,
};

struct FrameTarget {
    FrameTargetType mType;
    int32_t mIndex;
};

enum class FrameType : uint8_t {
    DefaultFrame,
    SpecialFrame,
};

struct PMXDisplayFrame {
    std::string mName;
    std::string mEnglishName;

    std::vector<FrameTarget> mTargets;
    FrameType mFrameType;
};

enum class RigidbodyShape: uint8_t {
    Sphere,
    Box,
    Capsule,
};

enum class RigidbodyOperation : uint8_t {
    Static,
    Dynamic,
    DynamicAndBoneMerge,
};

struct PMXRigidbody {
    std::string mName;
    std::string mEnglishName;

    int32_t mBoneIndex;
    uint8_t mGroup;
    uint16_t mCollisionGroup;

    RigidbodyShape mShape;
    glm::vec3 mShapeSize;

    glm::vec3 mTranslate;
    glm::vec3 mRotate;

    float mMass;
    float mTranslateDimmer;
    float mRotateDimmer;
    float mRepulsion;
    float mFriction;

    RigidbodyOperation mOperation;
};

enum class JointType : uint8_t {
    SpringDOF6,
    DOF6,
    P2P,
    ConeTwist,
    Slider,
    Hinge,
};

struct PMXJoint {
    std::string mName;
    std::string mEnglishName;

    JointType mJointType;
    int32_t mRigidbodyAIndex;
    int32_t mRigidbodyBIndex;

    glm::vec3 mTranslate;
    glm::vec3 mRotate;

    glm::vec3 mTranslateLowerLimit;
    glm::vec3 mTranslateUpperLimit;

    glm::vec3 mRotateLowerLimit;
    glm::vec3 mRotateUpperLimit;

    glm::vec3 mSpringTranslateFactor;
    glm::vec3 mSpringRotateFactor;
};

enum class SoftbodyType : uint8_t {
    TriMesh,
    Rope,
};

enum class SoftbodyMask : uint8_t {
    BLink = 0x01,
    Cluster = 0x02,
    HybridLink = 0x04,
};

enum class SoftbodyAeroModel : int32_t {
    VTwoSided,
    VOneSided,
    FTwoSided,
    FOneSided,
};

struct AnchorRigidbody {
    int32_t mRigidbodyIndex;
    int32_t mVertexIndex;
    uint8_t mNearMode;
};

struct PMXSoftbody {
    std::string mName;
    std::string mEnglishName;

    SoftbodyType mType;
    int32_t mMaterialIndex;

    uint8_t mGroup;
    uint16_t mCollisionGroup;

    SoftbodyMask mMask;

    int32_t mBLinkLength;
    int32_t mNumClusters;

    float mTotalMass;
    float mCollisionMargin;

    int32_t mAeroModel;

    float	mConfigVCF;
    float	mConfigDP;
    float	mConfigDG;
    float	mConfigLF;
    float	mConfigPR;
    float	mConfigVC;
    float	mConfigDF;
    float	mConfigMT;
    float	mConfigCHR;
    float	mConfigKHR;
    float	mConfigSHR;
    float	mConfigAHR;

    float	mClusterSRHR_CL;
    float	mClusterSKHR_CL;
    float	mClusterSSHR_CL;
    float	mClusterSR_SPLT_CL;
    float	mClusterSK_SPLT_CL;
    float	mClusterSS_SPLT_CL;

    int32_t	mInterationV_IT;
    int32_t	mInterationP_IT;
    int32_t	mInterationD_IT;
    int32_t	mInterationC_IT;

    float	mMaterialLST;
    float	mMaterialAST;
    float	mMaterialVST;

    std::vector<AnchorRigidbody> mAnchorRigidbodies;
    std::vector<int32_t> mPinVertexIndices;
};

struct PMXFile {
    PMXHeader mHeader;
    PMXInfo mInfo;

    std::vector<PMXVertex> mVertices;
    std::vector<PMXFace> mFaces;
    std::vector<PMXTexture>	mTextures;
    std::vector<PMXMaterial> mMaterials;
    std::vector<PMXBone> mBones;
    std::vector<PMXMorph> mMorphs;
    std::vector<PMXDisplayFrame> mDisplayFrames;
    std::vector<PMXRigidbody> mRigidbodies;
    std::vector<PMXJoint> mJoints;
    std::vector<PMXSoftbody> mSoftbodies;
};

bool ReadPMXFile(PMXFile* pmxFile, const char* fileName);
    
} // namespace phatom


#endif //PHATOM_PMXFILEDEFINE_H
