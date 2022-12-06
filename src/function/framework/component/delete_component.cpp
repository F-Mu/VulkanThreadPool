#include "delete_component.hpp"
#include "transform_component.hpp"
#include "function/framework/game_object.hpp"
#include "mesh_component.hpp"
#include "render_component.hpp"

namespace crp {
    void DeleteComponent::tick() {
        auto mesh = m_parent_object.lock()->tryGetComponent(MeshComponent);
        if (time <= 0) {
            m_parent_object.lock()->tryGetComponent(RenderComponent)->setDirty(true);
            mesh->setDirty(false);
            m_parent_object.lock()->setShouldTick(false);
            return;
        }
        mesh->setDirty(true);
        if (time == FRAME_TIME) {
            mesh->points.emplace_back(mesh->points.back());
        }
        float halfTime = FRAME_TIME / 2;
        if (time > FRAME_TIME / 2) {
            mesh->points[3].y -= height / halfTime;
            mesh->points[4].x += width / halfTime;
        } else {
            if (mesh->points.size() > 3) {
                mesh->points.pop_back();
                mesh->points.pop_back();
            }
            mesh->points[2].y -= height / halfTime;
            mesh->points[1].x += width / halfTime;
        }
//        auto transform = m_parent_object.lock()->tryGetComponent(TransformComponent);
//        transform->scale -= SCALE / FRAME_TIME;
        --time;
    }

    DeleteComponent::DeleteComponent(const std::weak_ptr<GameObject> &parent, float width, float height)
            : Component(parent), width{width}, height{height} {
        type = "DeleteComponent";
    }
}