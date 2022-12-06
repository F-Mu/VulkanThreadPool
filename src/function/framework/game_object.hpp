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
    class GameObject : public std::enable_shared_from_this<GameObject> {
    public:
        static GameObject createGameObject() {
            static id_t currentId = 0;
            return {currentId++};
        }

        virtual ~GameObject();

        GameObject(const GameObject &) = default;

        GameObject &operator=(const GameObject &) = default;

        GameObject(GameObject &&) = default;

        GameObject &operator=(GameObject &&) = default;

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

        void setShouldTick(bool cond) { shouldTick = cond; }

#define tryGetComponent(COMPONENT_TYPE) tryGetComponent<COMPONENT_TYPE>(#COMPONENT_TYPE)
#define tryGetComponentConst(COMPONENT_TYPE) tryGetComponentConst<const COMPONENT_TYPE>(#COMPONENT_TYPE)

    private:
        GameObject(id_t objId) : id{objId} {}

        id_t id;

        bool shouldTick{true};
    };
}