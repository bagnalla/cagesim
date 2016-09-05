//
// Created by alex on 9/4/16.
//

#ifndef CAGESIM_PLOT_H
#define CAGESIM_PLOT_H

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
        Plot(puddi::Object *par, size_t nData) : puddi::DrawableObject(par, createVertexMesh())
        {
            data = std::vector<float>(nData);
            allocateVertexData();
        }

        void SetData(const std::vector<float>& d)
        {
            assert (d.size() == data.size());
            data = d;
        }
    private:
        std::vector<float> data;
        size_t vertexOffset;
        size_t indexOffset;
        size_t indexCount;

        puddi::VertexMesh createVertexMesh()
        {
            return puddi::VertexMesh();
        }

        void allocateVertexData()
        {
            vertexOffset = puddi::Shader::Vertices.size();
            indexOffset = puddi::Shader::VertexIndices.size();
            indexCount = data.size();

            std::vector<vec4> vertices;
            for (size_t i = 0; i < data.size(); ++i) {
                vertices.push_back(vec4(data[i], 1.0f / data.size(), 0.0f, 1.0f));
            }
            std::vector<vec4> normals;
            for (size_t i = 0; i < data.size(); ++i) {
                auto norm = normalize(vec3(vertices[i].x, vertices[i].y, vertices[i].z));
                normals.push_back(vec4(norm.x, norm.y, norm.z, 0.0f));
            }
            std::vector<vec4> tangents;
            for (size_t i = 0; i < data.size(); ++i) {
                tangents.push_back(puddi::Util::RotateZ(-M_PI / 2.0f) * normals[i]);
            }
            std::vector<vec4> binormals;
            for (size_t i = 0; i < data.size(); ++i) {
                auto crossP = cross(vec3(normals[i].x, normals[i].y, normals[i].z),
                                    vec3(tangents[i].x, tangents[i].y, tangents[i].z));
                binormals.push_back(vec4(crossP.x, crossP.y, crossP.z, 0.0f));
            }

            std::vector<vec2> texCoords = std::vector<vec2>(data.size());
            std::vector<vec4> boneData = std::vector<vec4>(data.size(), vec4(-1, -1, -1, -1));

            std::vector<size_t> indices;
            for (size_t i = 0; i < indexCount; ++i)
                indices.push_back(vertexOffset + i);

            puddi::Shader::VertexIndices.insert(puddi::Shader::VertexIndices.end(), indices.begin(), indices.end());

            puddi::Shader::TextureCoordinates.insert(puddi::Shader::TextureCoordinates.end(), texCoords.begin(), texCoords.end());
            puddi::Shader::BoneIndices.insert(puddi::Shader::BoneIndices.end(), boneData.begin(), boneData.end());
            puddi::Shader::BoneWeights.insert(puddi::Shader::BoneWeights.end(), boneData.begin(), boneData.end());
        }

        void updateVertexData()
        {
            std::vector<vec4> vertices;
            for (size_t i = 0; i < data.size(); ++i) {
                vertices.push_back(vec4(data[i], 1.0f / data.size(), 0.0f, 1.0f));
            }
            std::vector<vec4> normals;
            for (size_t i = 0; i < data.size(); ++i) {
                auto norm = normalize(vec3(vertices[i].x, vertices[i].y, vertices[i].z));
                normals.push_back(vec4(norm.x, norm.y, norm.z, 0.0f));
            }
            std::vector<vec4> tangents;
            for (size_t i = 0; i < data.size(); ++i) {
                tangents.push_back(puddi::Util::RotateZ(-M_PI / 2.0f) * normals[i]);
            }
            std::vector<vec4> binormals;
            for (size_t i = 0; i < data.size(); ++i) {
                auto crossP = cross(vec3(normals[i].x, normals[i].y, normals[i].z),
                                    vec3(tangents[i].x, tangents[i].y, tangents[i].z));
                binormals.push_back(vec4(crossP.x, crossP.y, crossP.z, 0.0f));
            }

            puddi::Shader::Vertices.insert(puddi::Shader::Vertices.end(), vertices.begin(), vertices.end());
            puddi::Shader::Normals.insert(puddi::Shader::Normals.end(), normals.begin(), normals.end());
            puddi::Shader::Tangents.insert(puddi::Shader::Tangents.end(), tangents.begin(), tangents.end());
            puddi::Shader::Binormals.insert(puddi::Shader::Binormals.end(), binormals.begin(), binormals.end());
        }
    };
}

#endif //CAGESIM_PLOT_H
