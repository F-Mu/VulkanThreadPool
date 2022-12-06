#pragma once

#include <glm/glm.hpp>
#include <vector>
#include "core/macro.hpp"
#include "game_object.hpp"
#include "render/render_device.hpp"
#include <iostream>

namespace crp {
    class Rectangle {
    public:
        static Rectangle
        MakeRectangle(std::vector<glm::vec3> points, glm::vec3 color = {1., 1., 1.},
                      bool fill = false,
                      bool moveAble = false);

        ~Rectangle();

        void Move(glm::vec3 destinations, float _time = FRAME_TIME);

        void Move(std::vector<glm::vec3> destinations, float _time = FRAME_TIME);

        void init(std::vector<glm::vec3> &points, glm::vec3 color = {1., 1., 1.}, bool fill = false,
                  bool moveAble = false);

        id_t getId() { return gameObject->getId(); }

        void setPosition(glm::vec3 position);

        glm::vec3 getCenter() const;

        bool isMove() const;

        std::shared_ptr<GameObject> gameObject;

        void registerManager();

        void setDelete();

        float width{};

        float height{};
    private:
        Rectangle();
    };
}