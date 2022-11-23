#include "crp_game_obejct.hpp"
#include "crp_device.hpp"
#include <iostream>

namespace crp {
// Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
    // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
    // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
    glm::mat4 TransformComponent::mat4() {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        return glm::mat4{
                {
                        scale.x * (c1 * c3 + s1 * s2 * s3),
                                       scale.x * (c2 * s3),
                                                      scale.x * (c1 * s2 * s3 - c3 * s1),
                                                                     0.0f,
                },
                {
                        scale.y * (c3 * s1 * s2 - c1 * s3),
                                       scale.y * (c2 * c3),
                                                      scale.y * (c1 * c3 * s2 + s1 * s3),
                                                                     0.0f,
                },
                {
                        scale.z * (c2 * s1),
                                       scale.z * (-s2),
                                                      scale.z * (c1 * c2),
                                                                     0.0f,
                },
                {       translation.x, translation.y, translation.z, 1.0f}};
    }

    glm::mat3 TransformComponent::normalMatrix() {
        const float c3 = glm::cos(rotation.z);
        const float s3 = glm::sin(rotation.z);
        const float c2 = glm::cos(rotation.x);
        const float s2 = glm::sin(rotation.x);
        const float c1 = glm::cos(rotation.y);
        const float s1 = glm::sin(rotation.y);
        const glm::vec3 invScale = 1.0f / scale;
        return glm::mat3{
                {
                        invScale.x * (c1 * c3 + s1 * s2 * s3),
                        invScale.x * (c2 * s3),
                        invScale.x * (c1 * s2 * s3 - c3 * s1),
                },
                {
                        invScale.y * (c3 * s1 * s2 - c1 * s3),
                        invScale.y * (c2 * c3),
                        invScale.y * (c1 * c3 * s2 + s1 * s3),
                },
                {
                        invScale.z * (c2 * s1),
                        invScale.z * (-s2),
                        invScale.z * (c1 * c2),
                },
        };
    }

    std::vector<CrpModel::Vertex>
    fun(const glm::vec3 x, const glm::vec3 y, const glm::vec3 z, const glm::vec3 w, const glm::vec3 &color) {
        return {{x, color},
                {y, color},
                {z, color},
                {y, color},
                {z, color},
                {w, color}};
    }

    std::vector<CrpModel::Vertex>
    fun2(const glm::vec3 &x, const glm::vec3 &y, const glm::vec3 &z, const glm::vec3 &w, const glm::vec3 &color) {
        std::vector<CrpModel::Vertex> ans;
        auto up = fun(x + glm::vec3{-.005f, -.005f, 0.}, y + glm::vec3{.005f, -.005f, 0.},
                      x + glm::vec3{-.005f, .005f, 0.}, y + glm::vec3{.005f, .005f, 0.}, color);
        auto bo = fun(z + glm::vec3{-.005f, -.005f, 0.}, w + glm::vec3{.005f, -.005f, 0.},
                      z + glm::vec3{-.005f, .005f, 0.}, w + glm::vec3{.005f, .005f, 0.}, color);
        auto le = fun(x + glm::vec3{-.005f, -.005f, 0.}, x + glm::vec3{.005f, -.005f, 0.},
                      z + glm::vec3{-.005f, .005f, 0.}, z + glm::vec3{.005f, .005f, 0.}, color);
        auto ri = fun(y + glm::vec3{-.005f, -.005f, 0.}, y + glm::vec3{.005f, -.005f, 0.},
                      w + glm::vec3{-.005f, .005f, 0.}, w + glm::vec3{.005f, .005f, 0.}, color);
        ans.insert(ans.end(), up.begin(), up.end());
        ans.insert(ans.end(), le.begin(), le.end());
        ans.insert(ans.end(), ri.begin(), ri.end());
        ans.insert(ans.end(), bo.begin(), bo.end());
        return ans;
    }


    CrpGameObject
    CrpGameObject::makeRectangle(CrpDevice &device, glm::vec3 x, glm::vec3 y, glm::vec3 z, glm::vec3 w, bool fill,
                                 glm::vec3 color) {
        CrpGameObject gameObj = CrpGameObject::createGameObject();
        gameObj.color = color;
        gameObj.rectangle = std::make_unique<RectangleComponent>(x, y, z, w, fill);
        if (fill)
            gameObj.model = CrpModel::createModelFromVertices(device,
                                                              fun(x, y, z, w, color)
            );
        else
            gameObj.model = CrpModel::createModelFromVertices(device,
                                                              fun2(x, y, z, w, color)
            );
        return gameObj;
    }


    CrpGameObject
    CrpGameObject::makeRectangle(CrpDevice &device, const std::vector<glm::vec3> &v, const glm::vec3 &center,
                                 bool fill,
                                 const glm::vec3 color) {
        CrpGameObject gameObj = CrpGameObject::createGameObject();
        gameObj.color = color;
        gameObj.rectangle = std::make_unique<RectangleComponent>(v, fill);
        if (fill)
            gameObj.model = CrpModel::createModelFromVertices(device,
                                                              fun(v[0], v[1], v[2], v[3], color)
            );
        else
            gameObj.model = CrpModel::createModelFromVertices(device,
                                                              fun2(v[0], v[1], v[2], v[3], color)
            );
        gameObj.transform.translation = center;
        return gameObj;
    }
}