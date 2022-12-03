#pragma once

#include <string>
#include <utility>

namespace crp {
    class CrpGameObject;

    class Component {
    public:
        Component(std::weak_ptr<CrpGameObject> parent) : m_parent_object(std::move(parent)) {};

        std::weak_ptr<CrpGameObject> m_parent_object;

        virtual void tick() {};

        std::string getTypeName() { return type; }

    protected:
        std::string type;
    };
}