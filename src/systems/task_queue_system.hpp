#pragma once

#include <glm/glm.hpp>
#include <queue>
#include <list>
#include "../crp_device.hpp"
#include "../core/macro.hpp"
#include "../resources/game_object_manager.hpp"
#include "../core/rectangle.hpp"
#include "../crp_frame_info.hpp"

namespace crp {
    template<typename Fun, typename... Args>
    class task {
    public:
        Rectangle rectangle;
    };

    class TaskQueueSystem {
    public:
        TaskQueueSystem(CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager);

        void addMoveTask(Rectangle &task, glm::vec3 &point);

        void addMoveTask(Rectangle &task, std::vector<glm::vec3> &point);

        void addDeleteTask(Rectangle &task);

        void tick(FrameInfo &frameInfo);

        void roundTick();

        void addRunTask(Rectangle &task, glm::vec3 &point);

        void addTask(CrpDevice &crpDevice, const std::shared_ptr<GameObjectManager> &manager);

        void sortTasks();

        std::vector<glm::vec3> points;
        std::list<Rectangle> tasksInQueue;
        std::queue<Rectangle> tasksWait;
        std::vector<std::unique_ptr<TaskToMove>> moveTasks;
        std::vector<std::pair<GameObjectManager::id_t, float>> deleteTasks;
        std::vector<GameObjectManager::id_t> shouldDelete;
    private:
        float up = -1.1f, down = 1.1f, left = 1.f, right = 1.5f;
        glm::vec3 x{left, up, QUEUE_LAYER}, y{right, up, QUEUE_LAYER}, z{left, down, QUEUE_LAYER}, w{right, down,
                                                                                                     QUEUE_LAYER};
        glm::vec3 taskInitPosition;
        float taskWidth, taskHeight;
        GameObjectManager::id_t id;
    };
}
