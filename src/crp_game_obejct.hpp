#pragma once

#include "crp_model.hpp"

//libs
#include <glm/gtc/matrix_transform.hpp>
//std
#include <memory>
#include <unordered_map>

namespace crp {
    struct TransformComponent {
        glm::vec3 translation{};
        glm::vec3 scale{1.f, 1.f, 1.f};
        glm::vec3 rotation;

        // Matrix corrsponds to Translate * Ry * Rx * Rz * Scale
        // Rotations correspond to Tait-bryan angles of Y(1), X(2), Z(3)
        // https://en.wikipedia.org/wiki/Euler_angles#Rotation_matrix
        glm::mat4 mat4();

        glm::mat3 normalMatrix();
    };

    struct PointLightComponent {
        float lightIntensity = 1.0f;
    };

    struct RectangleComponent {
        glm::vec3 x, y, z, w;
        bool fill;

        RectangleComponent(glm::vec3 x, glm::vec3 y, glm::vec3 z, glm::vec3 w, bool fill) : x{x}, y{y}, z{z}, w{w},
                                                                                            fill{fill} {}

        RectangleComponent(std::vector<glm::vec3>&v, bool fill) : x{v[0]}, y{v[1]}, z{v[2]}, w{v[3]},
                                                                                            fill{fill} {}
    };

    class CrpGameObject {
    public:
        using id_t = unsigned int;
        using Map = std::unordered_map<id_t, CrpGameObject>;

        static CrpGameObject createGameObject() {
            static id_t currentId = 0;
            return {currentId++};
        }

        static CrpGameObject makePointLight(
                float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

        static CrpGameObject makeRectangle(
                CrpDevice &device, glm::vec3 x, glm::vec3 y, glm::vec3 z, glm::vec3 w, bool fill,
                glm::vec3 color = glm::vec3(1.f));

        static CrpGameObject makeRectangle(CrpDevice &device, std::vector<glm::vec3> &v, bool fill,
                                           glm::vec3 color = glm::vec3(1.f));

        CrpGameObject(const CrpGameObject &) = delete;

        CrpGameObject &operator=(const CrpGameObject &) = delete;

        CrpGameObject(CrpGameObject &&) = default;

        CrpGameObject &operator=(CrpGameObject &&) = default;

        id_t getId() { return id; }

        glm::vec3 color{};
        TransformComponent transform{};

        std::shared_ptr<CrpModel> model{};
        std::unique_ptr<PointLightComponent> pointLight = nullptr;
        std::unique_ptr<RectangleComponent> rectangle = nullptr;

    private:
        CrpGameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}