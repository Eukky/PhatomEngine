//#include "Graphics/VulkanInstance.h"
#include "Model/MMD/PMXFile.h"
#include "Core/Application.h"

using namespace phatom;

int main() {
    
    PMXFile *pmx = new PMXFile();
    const char* file = "../Resource/models/bbl.pmx";
    ReadPMXFile(pmx, file);

    size_t vertexCount = pmx->mVertices.size();
    std::vector<glm::vec3> positions;
    std::vector<uint16_t> indices;

    positions.reserve(vertexCount);

    for(const auto& v : pmx->mVertices) {
        glm::vec3 pos = v.mPosition * glm::vec3(1, 1, -1);
        positions.push_back(pos);
    }
    size_t indexElementSize = pmx->mHeader.mVertexIndexSize;
    indices.resize(pmx->mFaces.size() * 3);
    size_t indexCount = pmx->mFaces.size() * 3;

    switch (indexElementSize) {
    case 1: {
        break;
    }
    case 2: {
        int idx = 0;
        uint16_t* ind = (uint16_t*)indices.data();
        for(const auto& face : pmx->mFaces) {
            for(int i = 0; i < 3; ++i) {
                auto vi = face.mVertices[3 - i - 1];
                indices[idx] = (uint16_t)vi;
                idx++;
            }
        }
        break;
    }
    case 4: {
        break;
    }
    default:
        break;
    }

    Application *app = new Application();

    app->setVertices(positions);
    app->setIndices(indices);

    try {
        app->run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    delete app;
    return EXIT_SUCCESS;
}
