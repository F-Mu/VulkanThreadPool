#include "rectangle.hpp"

#include <utility>
#include "component/component.hpp"
#include "component/transform_component.hpp"
#include "component/move_component.hpp"
#include "component/mesh_component.hpp"
#include "component/render_component.hpp"
#include "function/global/global_context.hpp"
#include "resources/manager/game_object_manager.hpp"
#include "component/delete_component.hpp"

namespace crp {
    void Rectangle::init(std::vector<glm::vec3> &points, glm::vec3 color, bool fill, bool moveAble) {
        gameObject->addComponent<TransformComponent>();
        gameObject->addComponent<MeshComponent>(points, color, fill);
        if (moveAble)
            gameObject->addComponent<MoveComponent>();
        gameObject->addComponent<RenderComponent>(*globalContext.device);
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

    Rectangle::Rectangle() : gameObject{std::make_shared<CrpGameObject>(CrpGameObject::createGameObject())} {}

    Rectangle Rectangle::MakeRectangle(std::vector<glm::vec3> points, glm::vec3 color, bool fill,
                                       bool moveAble) {
        Rectangle rectangle;
        rectangle.init(points, color, fill, moveAble);
        rectangle.registerManager();
        return rectangle;
    }

    glm::vec3 Rectangle::getCenter() const {
        return gameObject->tryGetComponentConst(TransformComponent)->translation;
    }

    bool Rectangle::isMove() {
        auto moveComponent = gameObject->tryGetComponent(MoveComponent);
        return moveComponent && moveComponent->move;
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
        gameObject->addComponent<DeleteComponent>();
    }
}