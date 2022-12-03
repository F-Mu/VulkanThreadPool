#include "mesh_component.hpp"
#include "transform_component.hpp"
#include "function/framework/crp_game_obejct.hpp"

namespace crp {
    std::vector<Vertex>
    fillMesh(const std::vector<glm::vec3> &points, const glm::vec3 &color, const glm::mat4 &modelMatrix) {
        std::vector<Vertex> vertices;
        for (auto point: points) {
//            PRINT(point);
//            if(modelMatrix[3][2]==THREAD_LAYER) {
//                std::cout << "!!" << std::endl;
//                for(int i=0;i<3;++i)std::cout<<modelMatrix[3][i]<<std::endl;
//                PRINT(point);
//                PRINT4(modelMatrix * glm::vec4(point, 1.));
//                std::cout << "!!" << std::endl;
//            }
            vertices.emplace_back(modelMatrix * glm::vec4(point, 1.), color);
        }
        return vertices;
    }

    std::vector<Vertex>
    unfilledMesh(std::vector<glm::vec3> &points, const glm::vec3 &color, const glm::mat4 &modelMatrix) {
        const float offset = 0.001f;
        std::vector<Vertex> vertices;

        for (int i = 0; i + 1 < points.size(); ++i) {
            //向线的法向方向平移从而画出带厚度的线
            auto tt = points[i + 1] - points[i];
            tt = {-tt.y, tt.x, tt.z};
            glm::vec3 off = offset * glm::normalize(tt);
            auto tmp = std::move(fillMesh(
                    {
                            points[i] - off, points[i] + off, points[i + 1] - off, points[i + 1] + off
                    }, color, modelMatrix));
            vertices.insert(vertices.end(), tmp.begin(), tmp.end());
        }

        auto tt = points.back() - points[0];
        tt = {-tt.y, tt.x, tt.z};
        glm::vec3 off = offset * glm::normalize(tt);
        auto tmp = std::move(fillMesh(
                {
                        points.back() - off, points.back() + off, points[0] - off, points[0] + off
                }, color, modelMatrix));
        vertices.insert(vertices.end(), tmp.begin(), tmp.end());
        return vertices;
    }

    MeshComponent::MeshComponent(const std::weak_ptr<CrpGameObject> &parent, std::vector<glm::vec3> &_points,
                                 glm::vec3 &_color, bool _fill)
            : Component(parent), points{_points}, color{_color}, fill{_fill} {
        type = "MeshComponent";
        getWorld();
    }

    void MeshComponent::tick() {
        getWorld();
    }

    void MeshComponent::getWorld() {
        TransformComponent transformComponent = *m_parent_object.lock()->tryGetComponentConst(TransformComponent);
        auto modelMatrix = transformComponent.mat4();
        if (fill) {
            worldPoints = std::move(fillMesh(points, color, modelMatrix));
        } else {
            worldPoints = std::move(unfilledMesh(points, color, modelMatrix));
        }
        getIndices();
    }

    void MeshComponent::getIndices() {
        worldIndices.clear();
        for (int i = 0; i + 2 < worldPoints.size(); ++i) {
            for (int j = i; j <= i + 2; ++j)worldIndices.emplace_back(j);
        }
    }
}