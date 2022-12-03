#include "delete_component.hpp"
#include "transform_component.hpp"
#include "function/framework/crp_game_obejct.hpp"

namespace crp {
    void DeleteComponent::tick() {
        if (!time)return;
        auto transform = m_parent_object.lock()->tryGetComponent(TransformComponent);
        transform->scale -= SCALE / FRAME_TIME;
        --time;
    }
}