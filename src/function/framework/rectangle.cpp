#include "rectangle.hpp"

#include "component/particle_component.hpp"
#include "component/component.hpp"
#include "component/transform_component.hpp"
#include "component/move_component.hpp"
#include "component/mesh_component.hpp"
#include "component/render_component.hpp"
#include "function/global/global_context.hpp"
#include "resources/manager/game_object_manager.hpp"
#include "component/delete_component.hpp"

namespace crp {
    Rectangle::~Rectangle() {
        gameObject.reset();
    }

    void Rectangle::init(std::vector<glm::vec3> &points, glm::vec3 color, bool fill, bool moveAble) {
        gameObject->addComponent(TransformComponent);
        gameObject->addComponent(MeshComponent, points, color, fill);
        if (moveAble)
            gameObject->addComponent(MoveComponent);
        gameObject->addComponent(RenderComponent);
    }

    void Rectangle::Move(glm::vec3 destination, float _time) {
        auto moveComponent = gameObject->tryGetComponent(MoveComponent);
        if (!moveComponent)return;
        moveComponent->setDestination(destination, _time);
    }

    void Rectangle::Move(std::vector<glm::vec3> destinations, float _time) {
        auto moveComponent = gameObject->tryGetComponent(MoveComponent);
        if (!moveComponent)return;
        moveComponent->setDestination(destinations, _time);
    }

    Rectangle::Rectangle() : gameObject{std::make_shared<GameObject>(GameObject::createGameObject())} {}

    Rectangle Rectangle::MakeRectangle(std::vector<glm::vec3> points, glm::vec3 color, bool fill,
                                       bool moveAble) {
        Rectangle rectangle;
        rectangle.init(points, color, fill, moveAble);
        rectangle.registerManager();
        for (int i = 1; i < points.size(); ++i) {
            if (points[i].x == points[0].x) {
                rectangle.height = fabs(points[i].y - points[0].y);
            } else if (points[i].y == points[i].y) {
                rectangle.width = fabs(points[i].x - points[0].x);
            }
        }
        return rectangle;
    }

    glm::vec3 Rectangle::getCenter() const {
        return gameObject->tryGetComponentConst(TransformComponent)->translation;
    }

    bool Rectangle::isMove() {
        auto moveComponent = gameObject->tryGetComponent(MoveComponent);
        return moveComponent && (!moveComponent->isFinished());
    }

    void Rectangle::setPosition(glm::vec3 position) {
        gameObject->tryGetComponent(TransformComponent)->setPosition(position);
    }

    void Rectangle::registerManager() {
        globalContext.gameObjectManager->registerGO(gameObject->shared_from_this());
    }

    void Rectangle::setDelete() {
        auto deleteComponent = gameObject->tryGetComponent(DeleteComponent);
        if (deleteComponent)return;
        gameObject->addComponent(DeleteComponent, width, height);
        gameObject->addComponent(ParticleComponent);
        auto number = gameObject->tryGetComponent(NumberComponent);
        if (number)number->setDirty(true);
    }
}