#pragma once

#include <string>
#include <utility>

namespace crp {
    class GameObject;

    class Component {
    public:
        explicit Component(std::weak_ptr<GameObject> parent) : m_parent_object(std::move(parent)) {};

        std::weak_ptr<GameObject> m_parent_object;

        virtual void tick() {};

        std::string getTypeName() { return type; }

        [[nodiscard]] bool isDirty() const { return is_dirty; }

        void setDirty(bool cond) { is_dirty = cond; }

    private:
        bool is_dirty{false};
    protected:
        std::string type;
    };
}