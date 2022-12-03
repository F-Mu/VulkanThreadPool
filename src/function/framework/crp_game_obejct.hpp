#pragma once

#include "component/component.hpp"
#include "component/transform_component.hpp"
#include "component/move_component.hpp"
//libs
#include <glm/gtc/matrix_transform.hpp>
//std
#include <memory>
#include <unordered_map>
#include <list>
#include <iostream>

namespace crp {


//    struct RectangleComponent {
//        glm::vec3 x, y, z, w;
//        bool fill;
//
//        RectangleComponent(glm::vec3 x, glm::vec3 y, glm::vec3 z, glm::vec3 w, bool fill) : x{x}, y{y}, z{z}, w{w},
//                                                                                            fill{fill} {}
//
//        RectangleComponent(const std::vector<glm::vec3> &v, bool fill) : x{v[0]}, y{v[1]}, z{v[2]}, w{v[3]},
//                                                                         fill{fill} {}
//    };

    class CrpGameObject : public std::enable_shared_from_this<CrpGameObject> {
    public:
        static CrpGameObject createGameObject() {
            static id_t currentId = 0;
            return {currentId++};
        }

//        static CrpGameObject makeRectangle(
//                CrpDevice &device, glm::vec3 x, glm::vec3 y, glm::vec3 z, glm::vec3 w, bool fill,
//                glm::vec3 color = glm::vec3(1.f));
//
//        static CrpGameObject
//        makeRectangle(CrpDevice &device, const std::vector<glm::vec3> &v, const glm::vec3 &center, bool fill,
//                      glm::vec3 color = glm::vec3(1.f));

        CrpGameObject(const CrpGameObject &) = default;

//
        CrpGameObject &operator=(const CrpGameObject &) = default;

        CrpGameObject(CrpGameObject &&) = default;

        CrpGameObject &operator=(CrpGameObject &&) = default;

        template<typename TComponent>
        TComponent *tryGetComponent(const std::string &component_type_name) {
            for (auto &component: components) {
                if (component->getTypeName() == component_type_name) {
                    return static_cast<TComponent *>(component.operator->());
                }
            }
            return nullptr;
        }

        template<typename TComponent>
        const TComponent *tryGetComponentConst(const std::string &component_type_name) const {
            for (const auto &component: components) {
                if (component->getTypeName() == component_type_name) {
                    return static_cast<const TComponent *>(component.operator->());
                }
            }
            return nullptr;
        }


        template<typename TComponent, typename ...Args>
        void addComponent(Args &...args) {
            auto now = new TComponent(weak_from_this(), args...);
            components.emplace_back(now);
        }

        void tick();

        id_t getId() { return id; }

        std::list<std::shared_ptr<Component>> components;


#define tryGetComponent(COMPONENT_TYPE) tryGetComponent<COMPONENT_TYPE>(#COMPONENT_TYPE)
#define tryGetComponentConst(COMPONENT_TYPE) tryGetComponentConst<const COMPONENT_TYPE>(#COMPONENT_TYPE)
//#define tryAddComponent(COMPONENT_TYPE) addComponent<COMPONENT_TYPE>()
//        TransformComponent transform{};

//        std::unique_ptr<RectangleComponent> rectangle = nullptr;

    private:
        CrpGameObject(id_t objId) : id{objId} {}

        id_t id;
    };
}