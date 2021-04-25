#include "PMXFile.h"
#include "Common/UnicodeUtil.h"

#include <vector>

namespace phatom {

template <typename T>
bool Read(T* val, File& file) {
    return file.Read(val);
}

template <typename T>
bool Read(T* valArray, size_t size, File& file) {
    return file.Read(valArray, size);
}

bool ReadString(PMXFile* pmxFile, std::string* val, File& file) {
    uint32_t bufSize;
    if(!Read(&bufSize, file)) {
        return false;
    }

    if(bufSize > 0) {
        if(pmxFile->mHeader.mEncode == 0) {
            std::u16string utf16Str(bufSize / 2, u'\0');
            if(!Read(&utf16Str[0], utf16Str.size(), file)) {
                return false;
            }
            if(!ConvU16ToU8(utf16Str, *val)) {
                return false;
            }
        } else if(pmxFile->mHeader.mEncode == 1) {
            std::string utf8Str(bufSize, '\0');
            Read(&utf8Str[0], bufSize, file);
            *val = utf8Str;
        }
    }
    return !file.IsBad();
}

bool ReadIndex(int32_t* index, uint8_t indexSize, File& file) {
    switch (indexSize) {
    case 1: {
        uint8_t idx;
        Read(&idx, file);
        if(idx != 0xFF) {
            *index = (int32_t)idx;
        } else {
            *index = -1;
        }
        break;
    }  
    case 2: {
        uint16_t idx;
        Read(&idx, file);
        if(idx != 0xFFFF) {
            *index = (int32_t)idx;
        } else {
            *index = -1;
        }
        break;
    }
    case 4: {
        uint32_t idx;
        Read(&idx, file);
        *index = (int32_t)idx;
        break;
    }
    default:
        return false;
    }
    return !file.IsBad();
}

bool ReadHeader(PMXFile* pmxFile, File& file) {
    auto& header = pmxFile->mHeader;

    Read(&header.mSignature, file);
    Read(&header.mVersion, file);
    Read(&header.mDataSize, file);
    Read(&header.mEncode, file);
    Read(&header.mAddUVNum, file);
    Read(&header.mVertexIndexSize, file);
    Read(&header.mTextureIndexSize, file);
    Read(&header.mMaterialIndexSize, file);
    Read(&header.mBoneIndexSize, file);
    Read(&header.mMorphIndexSize, file);
    Read(&header.mRigidbodyIndexSize, file);

    return !file.IsBad();
}

bool ReadInfo(PMXFile* pmxFile, File& file) {
    auto& info = pmxFile->mInfo;

    ReadString(pmxFile, &info.mModelName, file);
    ReadString(pmxFile, &info.mEnglishModelName, file);
    ReadString(pmxFile, &info.mComment, file);
    ReadString(pmxFile, &info.mEnglishComment, file);

    return !file.IsBad();
}

bool ReadVertex(PMXFile* pmxFile, File& file) {
    int32_t vertexCount;
    if(!Read(&vertexCount, file)) {
        return false;
    }
    auto& vertices = pmxFile->mVertices;
    vertices.resize(vertexCount);
    for(auto& vertex: vertices) {
        Read(&vertex.mPosition, file);
        Read(&vertex.mNormal, file);
        Read(&vertex.mUV, file);

        for(uint8_t i = 0; i < pmxFile->mHeader.mAddUVNum; ++i) {
            Read(&vertex.mAddUV[i], file);
        }

        Read(&vertex.mWeightType, file);

        switch (vertex.mWeightType) {
        case PMXVertexWeight::BDEF1: { 
            ReadIndex(&vertex.mBoneIndices[0], pmxFile->mHeader.mBoneIndexSize, file);
			break;
        }
        case PMXVertexWeight::BDEF2: {
            ReadIndex(&vertex.mBoneIndices[0], pmxFile->mHeader.mBoneIndexSize, file);
            ReadIndex(&vertex.mBoneIndices[1], pmxFile->mHeader.mBoneIndexSize, file);
            Read(&vertex.mBoneWeights[0], file);
            break;
        }
        case PMXVertexWeight::BDEF4: {
            ReadIndex(&vertex.mBoneIndices[0], pmxFile->mHeader.mBoneIndexSize, file);
            ReadIndex(&vertex.mBoneIndices[1], pmxFile->mHeader.mBoneIndexSize, file);
            ReadIndex(&vertex.mBoneIndices[2], pmxFile->mHeader.mBoneIndexSize, file);
            ReadIndex(&vertex.mBoneIndices[3], pmxFile->mHeader.mBoneIndexSize, file);
            Read(&vertex.mBoneWeights[0], file);
            Read(&vertex.mBoneWeights[1], file);
            Read(&vertex.mBoneWeights[2], file);
            Read(&vertex.mBoneWeights[3], file);
            break;
        }
        case PMXVertexWeight::SDEF: {
            ReadIndex(&vertex.mBoneIndices[0], pmxFile->mHeader.mBoneIndexSize, file);
            ReadIndex(&vertex.mBoneIndices[1], pmxFile->mHeader.mBoneIndexSize, file);
            Read(&vertex.mBoneWeights[0], file);
            Read(&vertex.mSDEFC[0], file);
            Read(&vertex.mSDEFR0[0], file);
            Read(&vertex.mSDEFR1[0], file);
            break;
        }
        case PMXVertexWeight::QDEF: {
            ReadIndex(&vertex.mBoneIndices[0], pmxFile->mHeader.mBoneIndexSize, file);
            ReadIndex(&vertex.mBoneIndices[1], pmxFile->mHeader.mBoneIndexSize, file);
            ReadIndex(&vertex.mBoneIndices[2], pmxFile->mHeader.mBoneIndexSize, file);
            ReadIndex(&vertex.mBoneIndices[3], pmxFile->mHeader.mBoneIndexSize, file);
            Read(&vertex.mBoneWeights[0], file);
            Read(&vertex.mBoneWeights[1], file);
            Read(&vertex.mBoneWeights[2], file);
            Read(&vertex.mBoneWeights[3], file);
            break;
        }
        default:
            return false;
        }
        Read(&vertex.mEdgeScale, file);
    }
    return !file.IsBad();
}

bool ReadFace(PMXFile* pmxFile, File& file) {
    int32_t faceCount = 0;
    if(!Read(&faceCount, file)) {
        return false;
    }
    faceCount /= 3;

    pmxFile->mFaces.resize(faceCount);

    switch (pmxFile->mHeader.mVertexIndexSize) {
    case 1: {
        std::vector<uint8_t> vertices(faceCount * 3);
        Read(vertices.data(), vertices.size(), file);
        for(int32_t faceIdx = 0; faceIdx < faceCount; ++faceIdx) {
            pmxFile->mFaces[faceIdx].mVertices[0] = vertices[faceIdx * 3 + 0];
            pmxFile->mFaces[faceIdx].mVertices[1] = vertices[faceIdx * 3 + 1];
            pmxFile->mFaces[faceIdx].mVertices[2] = vertices[faceIdx * 3 + 2];
        }
        break;
    }
    case 2: {
        std::vector<uint16_t> vertices(faceCount * 3);
        Read(vertices.data(), vertices.size(), file);
        for(int32_t faceIdx = 0; faceIdx < faceCount; ++faceIdx) {
            pmxFile->mFaces[faceIdx].mVertices[0] = vertices[faceIdx * 3 + 0];
            pmxFile->mFaces[faceIdx].mVertices[1] = vertices[faceIdx * 3 + 1];
            pmxFile->mFaces[faceIdx].mVertices[2] = vertices[faceIdx * 3 + 2];
        }
        break;
    }
    case 4: {
        std::vector<uint32_t> vertices(faceCount * 3);
        Read(vertices.data(), vertices.size(), file);
        for(int32_t faceIdx = 0; faceIdx < faceCount; ++faceIdx) {
            pmxFile->mFaces[faceIdx].mVertices[0] = vertices[faceIdx * 3 + 0];
            pmxFile->mFaces[faceIdx].mVertices[1] = vertices[faceIdx * 3 + 1];
            pmxFile->mFaces[faceIdx].mVertices[2] = vertices[faceIdx * 3 + 2];
        }
        break;
    }
    default:
        return false;
    }
    return !file.IsBad();
}

bool ReadTexture(PMXFile* pmxFile, File& file) {
    int32_t texCount = 0;
    if(!Read(&texCount, file)) {
        return false;
    }

    pmxFile->mTextures.resize(texCount);

    for(auto& tex : pmxFile->mTextures) {
        ReadString(pmxFile, &tex.mTextureName, file);
    }
    return !file.IsBad();
}

bool ReadMaterial(PMXFile* pmxFile, File& file) {
    int32_t matCount = 0;
    if(!Read(&matCount, file)) {
        return false;
    }

    pmxFile->mMaterials.resize(matCount);

    for(auto& mat : pmxFile->mMaterials) {
        ReadString(pmxFile, &mat.mName, file);
        ReadString(pmxFile, &mat.mEnglishName, file);

        Read(&mat.mDiffuse, file);
        Read(&mat.mSpecular, file);
        Read(&mat.mSpecularPower, file);
        Read(&mat.mAmbient, file);
        Read(&mat.mDrawMode, file);
        Read(&mat.mEdgeColor, file);
        Read(&mat.mEdgeSize, file);

        ReadIndex(&mat.mTextureIndex, pmxFile->mHeader.mTextureIndexSize, file);
        ReadIndex(&mat.mSphereTextureIndex, pmxFile->mHeader.mTextureIndexSize, file);
        Read(&mat.mSphereMode, file);

        Read(&mat.mToonMode, file);

        switch (mat.mToonMode) {
        case PMXToonMode::Separate: {
            ReadIndex(&mat.mToonTextureIndex, pmxFile->mHeader.mTextureIndexSize, file);
            break;
        }
        case PMXToonMode::Common: {
            uint8_t toonIndex;
            Read(&toonIndex, file);
            mat.mToonTextureIndex = (int32_t)toonIndex;
        }
        default:
            return false;
        }

        ReadString(pmxFile, &mat.mMemo, file);
        Read(&mat.mFaceNum, file);

    }
    return !file.IsBad();
}

bool ReadBone(PMXFile* pmxFile, File& file) {
    int32_t boneCount;
    if(!Read(&boneCount, file)) {
        return false;
    }

    pmxFile->mBones.resize(boneCount);

    for(auto& bone : pmxFile->mBones) {
        ReadString(pmxFile, &bone.mName, file);
        ReadString(pmxFile, &bone.mEnglishName, file);

        Read(&bone.mPositiion, file);
        ReadIndex(&bone.mParentBoneIndex, pmxFile->mHeader.mBoneIndexSize, file);
        Read(&bone.mDeformDepth, file);
        Read(&bone.mBoneFlag, file);

        if(((uint16_t)bone.mBoneFlag & (uint16_t)PMXBoneFlag::TargetShowMode) == 0) {
            Read(&bone.mPositionOffset, file);
        } else {
            ReadIndex(&bone.mLinkBoneIndex, pmxFile->mHeader.mBoneIndexSize, file);
        }

        if(((uint16_t)bone.mBoneFlag & (uint16_t)PMXBoneFlag::AppendRotate) || 
            ((uint16_t)bone.mBoneFlag & (uint16_t)PMXBoneFlag::AppendTranslate)) {
            ReadIndex(&bone.mAppendBoneIndex, pmxFile->mHeader.mBoneIndexSize, file);
            Read(&bone.mAppendBoneIndex, file);
        }

        if(((uint16_t)bone.mBoneFlag & (uint16_t)PMXBoneFlag::FixedAxis)) {
            Read(&bone.mFixedAxis, file);
        }

        if(((uint16_t)bone.mBoneFlag & (uint16_t)PMXBoneFlag::LocalAxis)) {
            Read(&bone.mLocalXAxis, file);
            Read(&bone.mLocalZAxis, file);
        }

        if(((uint16_t)bone.mBoneFlag & (uint16_t)PMXBoneFlag::DeformOuterParent)) {
            Read(&bone.mKeyValue, file);
        }

        if(((uint16_t)bone.mBoneFlag & (uint16_t)PMXBoneFlag::IK)) {
            ReadIndex(&bone.mIKTargetBoneIndex, pmxFile->mHeader.mBoneIndexSize, file);
            Read(&bone.mIKIterationCount, file);
            Read(&bone.mIKLimit, file);

            int32_t linkCount;
            if(!Read(&linkCount, file)) {
                return false;
            }

            bone.mIKLinks.resize(linkCount);

            for(auto& ikLink : bone.mIKLinks) {
                ReadIndex(&ikLink.mIKBoneIndex, pmxFile->mHeader.mBoneIndexSize, file);
                Read(&ikLink.mEnableLimit, file);

                if(ikLink.mEnableLimit != 0) {
                    Read(&ikLink.mLimitMin, file);
					Read(&ikLink.mLImitMax, file);
                }
            }
        }
    }
    return !file.IsBad();
}

bool ReadMorph(PMXFile* pmxFile, File& file) {
    int32_t morphCount;
    if(!Read(&morphCount, file)) {
        return false;
    }

    pmxFile->mMorphs.resize(morphCount);

    for(auto& morph : pmxFile->mMorphs) {
        ReadString(pmxFile, &morph.mName, file);
        ReadString(pmxFile, &morph.mEnglishName, file);

        Read(&morph.mControlPanel, file);
        Read(&morph.mMorphType, file);

        int32_t dataCount;
        if(!Read(&dataCount, file)) {
            return false;
        }

        if(morph.mMorphType == PMXMorphType::Position) {
            morph.mPositionMorph.resize(dataCount);
            for(auto& data : morph.mPositionMorph) {
                ReadIndex(&data.mVertexIndex, pmxFile->mHeader.mVertexIndexSize, file);
                Read(&data.mPosition, file);
            }
        } else if(morph.mMorphType == PMXMorphType::UV ||
            morph.mMorphType == PMXMorphType::AddUV1 ||
            morph.mMorphType == PMXMorphType::AddUV2 ||
            morph.mMorphType == PMXMorphType::AddUV3 ||
            morph.mMorphType == PMXMorphType::AddUV4) {
            morph.mUVMorph.resize(dataCount);
            for(auto& data : morph.mUVMorph) {
                ReadIndex(&data.mVertexIndex, pmxFile->mHeader.mVertexIndexSize, file);
                Read(&data.mUV, file);
            }
        } else if(morph.mMorphType == PMXMorphType::Bone) {
            morph.mBornMorph.resize(dataCount);
            for(auto& data : morph.mBornMorph) {
                ReadIndex(&data.mBoneIndex, pmxFile->mHeader.mBoneIndexSize, file);
                Read(&data.mPosition, file);
                Read(&data.mQuaternion, file);
            }
        } else if(morph.mMorphType == PMXMorphType::Material) {
            morph.mMaterialMorph.resize(dataCount);
            for (auto& data : morph.mMaterialMorph) {
                ReadIndex(&data.mMaterialIndex, pmxFile->mHeader.mMaterialIndexSize, file);
                Read(&data.mOpType, file);
                Read(&data.mDiffuse, file);
                Read(&data.mSpecular, file);
                Read(&data.mSpecularPower, file);
                Read(&data.mAmbient, file);
                Read(&data.mEdgeColor, file);
                Read(&data.mEdgeSize, file);
                Read(&data.mTextureFactor, file);
                Read(&data.mSphereTextureFactor, file);
                Read(&data.mToonTextureFactor, file);
            }
        } else if(morph.mMorphType == PMXMorphType::Group) {
            morph.mGroupMorph.resize(dataCount);
            for (auto& data : morph.mGroupMorph) {
                ReadIndex(&data.mMorphIndex, pmxFile->mHeader.mMorphIndexSize, file);
                Read(&data.mWeight, file);
            }
        } else if(morph.mMorphType == PMXMorphType::Filp) {
            morph.mFlipMorph.resize(dataCount);
            for (auto& data : morph.mFlipMorph) {
                ReadIndex(&data.mMorphIndex, pmxFile->mHeader.mMorphIndexSize, file);
                Read(&data.mWeight, file);
            }
        } else if(morph.mMorphType == PMXMorphType::Impluse) {
            morph.mImpulseMorph.resize(dataCount);
            for (auto& data : morph.mImpulseMorph) {
                ReadIndex(&data.mRigidbodyIndex, pmxFile->mHeader.mRigidbodyIndexSize, file);
                Read(&data.mLocalFlag, file);
                Read(&data.mTranslateVelocity, file);
                Read(&data.mRotateTorque, file);
            }
        } else {
            return false;
        }
    }
    return !file.IsBad();
}

bool ReadDisplayFrame(PMXFile* pmxFile, File& file) {
    int32_t displayFrameCount;
    if(!Read(&displayFrameCount, file)) {
        return false;
    }
    
    pmxFile->mDisplayFrames.resize(displayFrameCount);
    
    for(auto& displayFrame : pmxFile->mDisplayFrames) {
        ReadString(pmxFile, &displayFrame.mName, file);
        ReadString(pmxFile, &displayFrame.mEnglishName, file);
        
        Read(&displayFrame.mFrameType, file);
        
        int32_t targetCount;
        if(!Read(&targetCount, file)) {
            return false;
        }
        
        displayFrame.mTargets.resize(targetCount);
        for(auto& target : displayFrame.mTargets) {
            Read(&target.mType, file);
            if(target.mType == FrameTargetType::BoneIndex) {
                ReadIndex(&target.mIndex, pmxFile->mHeader.mBoneIndexSize, file);
            } else if(target.mType == FrameTargetType::MorphIndex) {
                ReadIndex(&target.mIndex, pmxFile->mHeader.mMorphIndexSize, file);
            } else {
                return false;
            }
        }
    }
    return !file.IsBad();
}

bool ReadRigidbody(PMXFile* pmxFile, File& file) {
    int32_t rbCount;
    if(!Read(&rbCount, file)) {
        return false;
    }

    pmxFile->mRigidbodies.resize(rbCount);

    for(auto& rb : pmxFile->mRigidbodies) {
        ReadString(pmxFile, &rb.mName, file);
        ReadString(pmxFile, &rb.mEnglishName, file);

        ReadIndex(&rb.mBoneIndex, pmxFile->mHeader.mBoneIndexSize, file);
        Read(&rb.mGroup, file);
        Read(&rb.mCollisionGroup, file);

        Read(&rb.mShape, file);
        Read(&rb.mShapeSize, file);

        Read(&rb.mTranslate, file);
        Read(&rb.mRotate, file);

        Read(&rb.mMass, file);
        Read(&rb.mTranslateDimmer, file);
        Read(&rb.mRotateDimmer, file);
        Read(&rb.mRepulsion, file);
        Read(&rb.mFriction, file);

        Read(&rb.mOperation, file);
    }

    return !file.IsBad();
}

bool ReadJoint(PMXFile* pmxFile, File& file) {
    int32_t jointCount;
    if(!Read(&jointCount, file)) {
        return false;
    }

    pmxFile->mJoints.resize(jointCount);

    for(auto& joint : pmxFile->mJoints) {
        ReadString(pmxFile, &joint.mName, file);
        ReadString(pmxFile, &joint.mEnglishName, file);

        Read(&joint.mJointType, file);
        ReadIndex(&joint.mRigidbodyAIndex, pmxFile->mHeader.mRigidbodyIndexSize, file);
        ReadIndex(&joint.mRigidbodyBIndex, pmxFile->mHeader.mRigidbodyIndexSize, file);

        Read(&joint.mTranslate, file);
        Read(&joint.mRotate, file);

        Read(&joint.mTranslateLowerLimit, file);
        Read(&joint.mTranslateUpperLimit, file);
        Read(&joint.mRotateLowerLimit, file);
        Read(&joint.mRotateUpperLimit, file);

        Read(&joint.mSpringTranslateFactor, file);
        Read(&joint.mSpringRotateFactor, file);
    }

    return !file.IsBad();
}

bool ReadSoftbody(PMXFile* pmxFile, File& file) {
    int32_t sbCount;
    if(!Read(&sbCount, file)) {
        return false;
    }

    pmxFile->mSoftbodies.resize(sbCount);

    for (auto& sb : pmxFile->mSoftbodies)
    {
        ReadString(pmxFile, &sb.mName, file);
        ReadString(pmxFile, &sb.mEnglishName, file);

        Read(&sb.mType, file);

        ReadIndex(&sb.mMaterialIndex, pmxFile->mHeader.mMaterialIndexSize, file);

        Read(&sb.mGroup, file);
        Read(&sb.mCollisionGroup, file);

        Read(&sb.mMask, file);

        Read(&sb.mBLinkLength, file);
        Read(&sb.mNumClusters, file);

        Read(&sb.mTotalMass, file);
        Read(&sb.mCollisionMargin, file);

        Read(&sb.mAeroModel, file);

        Read(&sb.mConfigVCF, file);
        Read(&sb.mConfigDP, file);
        Read(&sb.mConfigDG, file);
        Read(&sb.mConfigLF, file);
        Read(&sb.mConfigPR, file);
        Read(&sb.mConfigVC, file);
        Read(&sb.mConfigDF, file);
        Read(&sb.mConfigMT, file);
        Read(&sb.mConfigCHR, file);
        Read(&sb.mConfigKHR, file);
        Read(&sb.mConfigSHR, file);
        Read(&sb.mConfigAHR, file);

        Read(&sb.mClusterSRHR_CL, file);
        Read(&sb.mClusterSKHR_CL, file);
        Read(&sb.mClusterSSHR_CL, file);
        Read(&sb.mClusterSR_SPLT_CL, file);
        Read(&sb.mClusterSK_SPLT_CL, file);
        Read(&sb.mClusterSS_SPLT_CL, file);

        Read(&sb.mInterationV_IT, file);
        Read(&sb.mInterationP_IT, file);
        Read(&sb.mInterationD_IT, file);
        Read(&sb.mInterationC_IT, file);

        Read(&sb.mMaterialLST, file);
        Read(&sb.mMaterialAST, file);
        Read(&sb.mMaterialVST, file);

        int32_t arCount;
        if(!Read(&arCount, file)) {
            return false;
        }
        sb.mAnchorRigidbodies.resize(arCount);
        for(auto& ar : sb.mAnchorRigidbodies) {
            ReadIndex(&ar.mRigidbodyIndex, pmxFile->mHeader.mRigidbodyIndexSize, file);
            ReadIndex(&ar.mVertexIndex, pmxFile->mHeader.mRigidbodyIndexSize, file);
            Read(&ar.mNearMode, file);
        }

        int32_t pvCount;
        if(!Read(&pvCount, file)) {
            return false;
        }
        sb.mPinVertexIndices.resize(pvCount);
        for(auto& pv : sb.mPinVertexIndices) {
            ReadIndex(&pv, pmxFile->mHeader.mVertexIndexSize, file);
        }
    }

    return !file.IsBad();
}

bool ReadPMXFile(PMXFile* pmxFile, File& file) {
    if(!ReadHeader(pmxFile, file)) {
        return false;
    }

    if(!ReadInfo(pmxFile, file)) {
        return false;
    }

    if(!ReadVertex(pmxFile, file)) {
        return false;
    }

    if(!ReadFace(pmxFile, file)) {
        return false;
    }

    if(!ReadTexture(pmxFile, file)) {
        return false;
    }

    if(!ReadMaterial(pmxFile, file)) {
        return false;
    }

    if(!ReadBone(pmxFile, file)) {
        return false;
    }

    if(!ReadMorph(pmxFile, file)) {
        return false;
    }

    if(!ReadDisplayFrame(pmxFile, file)) {
        return false;
    }

    if(!ReadRigidbody(pmxFile, file)) {
        return false;
    }

    if(!ReadJoint(pmxFile, file)) {
        return false;
    }

    if(!ReadSoftbody(pmxFile, file)) {
        return false;
    }
}

bool ReadPMXFile(PMXFile* pmxFile, const char* fileName) {
    File file;
    if(!file.OpenBinary(fileName)) {
        return false;
    }

    if(ReadPMXFile(pmxFile, file)) {
        return false;
    }

    return true;
}
} // namespace phatom
