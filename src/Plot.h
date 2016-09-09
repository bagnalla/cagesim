//
// Created by alex on 9/4/16.
//

#ifndef CAGESIM_PLOT_H
#define CAGESIM_PLOT_H

#include <algorithm>
#include <cassert>
#include <vector>
#include "DrawableObject.h"
#include "Shader.h"
#include "Util.h"
#include "VertexMesh.h"

using std::size_t;

namespace cagesim
{
    class Plot : public puddi::DrawableObject
    {
    public:
        Plot(puddi::Object *par, size_t nData) : puddi::DrawableObject(par)
        {
            data = std::vector<float>(nData);

            vertexMeshes.push_back(createVertexMesh());
            updateRenderNodes();
        }

        void SetData(const std::vector<float>& d)
        {
            assert (d.size() == data.size());
            data = d;
            updateVertexData();
        }

    private:
        std::vector<float> data;
        size_t vertexOffset;

        puddi::VertexMesh* createVertexMesh()
        {
            // index offset, index count pair
            auto index_count = allocateVertexData();

            return new puddi::VertexMesh(this, puddi::Material::None(), index_count.first, index_count.second, true);
        }

        puddi::VertexData createInitialVertexData()
        {
            puddi::VertexData vData;

            // top vertices
            for (size_t i = 0; i < data.size(); ++i) {
                vData.vertices.push_back(vec4(static_cast<float>(i) / data.size(), data[i], 0.0f, 1.0f));
            }

            // bottom vertices
            for (size_t i = 0; i < data.size(); ++i) {
                vData.vertices.push_back(vec4(static_cast<float>(i) / data.size(), 0.0f, 0.0f, 1.0f));
            }

            // same for all
            for (size_t i = 0; i < data.size() * 2; ++i) {
                vData.normals.push_back(vec4(0.0f, 0.0f, -1.0f, 0.0f));
            }
            for (size_t i = 0; i < data.size() * 2; ++i) {
                vData.tangents.push_back(vec4(1.0f, 0.0f, 0.0f, 0.0f));
            }
            for (size_t i = 0; i < data.size() * 2; ++i) {
                vData.binormals.push_back(vec4(0.0f, -1.0f, 0.0f, 0.0f));
            }
            vData.textureCoordinates = std::vector<vec2>(data.size() * 2);
            vData.boneIndices = vData.boneWeights = std::vector<vec4>(data.size() * 2, vec4(-1, -1, -1, -1));

            return vData;
        }

        // only call once at Plot creation
        // returns (index offset, index count) pair
        std::pair<size_t, size_t> allocateVertexData()
        {
            vertexOffset = puddi::Shader::Vertices.size();
            size_t indexOffset = puddi::Shader::VertexIndices.size();

            // create vertex data
            puddi::VertexData vData = createInitialVertexData();

            puddi::Shader::Vertices.insert(puddi::Shader::Vertices.end(), vData.vertices.begin(), vData.vertices.end());
            puddi::Shader::Normals.insert(puddi::Shader::Normals.end(), vData.normals.begin(), vData.normals.end());
            puddi::Shader::Tangents.insert(puddi::Shader::Tangents.end(), vData.tangents.begin(), vData.tangents.end());
            puddi::Shader::Binormals.insert(puddi::Shader::Binormals.end(), vData.binormals.begin(),
                                            vData.binormals.end());
            puddi::Shader::TextureCoordinates.insert(puddi::Shader::TextureCoordinates.end(),
                                                     vData.textureCoordinates.begin(), vData.textureCoordinates.end());
            puddi::Shader::BoneIndices.insert(puddi::Shader::BoneIndices.end(), vData.boneIndices.begin(),
                                              vData.boneIndices.end());
            puddi::Shader::BoneWeights.insert(puddi::Shader::BoneWeights.end(), vData.boneWeights.begin(),
                                              vData.boneWeights.end());

            // create indices
            std::vector<size_t> indices;
            for (size_t i = 0; i < data.size(); ++i) {
                indices.push_back(vertexOffset + data.size() + i);
                indices.push_back(vertexOffset + i);
            }

            puddi::Shader::VertexIndices.insert(puddi::Shader::VertexIndices.end(), indices.begin(), indices.end());

            return std::make_pair(indexOffset, indices.size());
        }

        void updateVertexData()
        {
            auto max = std::max_element(data.begin(), data.end());
            if (max != data.end()) {
                std::vector<vec4> vertices;
                for (size_t i = 0; i < data.size(); ++i) {
                    vertices.push_back(vec4(static_cast<float>(i) / data.size(), data[i] / *max, 0.0f, 1.0f));
                }

                puddi::Shader::UpdateVertexPositions(vertexOffset, vertices);
            }
        }
    };
}

#endif //CAGESIM_PLOT_H
