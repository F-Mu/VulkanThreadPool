#include "mesh_component.hpp"
#include "transform_component.hpp"
#include "function/framework/game_object.hpp"

namespace crp {
    std::vector<Vertex>
    fillMesh(const std::vector<glm::vec3> &points, const glm::vec3 &color) {
        std::vector<Vertex> vertices;
        for (auto point: points) {
            vertices.emplace_back(point, color);
        }
        return vertices;
    }

    std::vector<Vertex>
    unfilledMesh(std::vector<glm::vec3> &points, const glm::vec3 &color) {
        const float offset = 0.0025f;
        std::vector<Vertex> vertices;

        for (int i = 0; i + 1 < points.size(); ++i) {
            //向线的法向方向平移从而画出带厚度的线
            auto tt = points[i + 1] - points[i];
            tt = {-tt.y, tt.x, tt.z};
            glm::vec3 off = offset * glm::normalize(tt);
            auto tmp = std::move(fillMesh(
                    {
                            points[i] - off, points[i] + off, points[i + 1] - off, points[i + 1] + off
                    }, color));
            vertices.insert(vertices.end(), tmp.begin(), tmp.end());
        }

        auto tt = points.back() - points[0];
        tt = {-tt.y, tt.x, tt.z};
        glm::vec3 off = offset * glm::normalize(tt);
        auto tmp = std::move(fillMesh(
                {
                        points.back() - off, points.back() + off, points[0] - off, points[0] + off
                }, color));
        vertices.insert(vertices.end(), tmp.begin(), tmp.end());
        return vertices;
    }

    MeshComponent::MeshComponent(const std::weak_ptr<GameObject> &parent, std::vector<glm::vec3> &_points,
                                 glm::vec3 &_color, bool _fill)
            : Component(parent), points{_points}, color{_color}, fill{_fill} {
        type = "MeshComponent";
        getWorld();
    }

    void MeshComponent::tick() {
        getWorld();
    }

    void MeshComponent::getWorld() {
        if (fill) {
            vertexPoints = std::move(fillMesh(points, color));
        } else {
            vertexPoints = std::move(unfilledMesh(points, color));
        }
        getIndices();
    }

    void MeshComponent::getIndices() {
        worldIndices.clear();
        for (int i = 0; i + 2 < vertexPoints.size(); ++i) {
            for (int j = i; j <= i + 2; ++j)worldIndices.emplace_back(j);
        }
    }

    void MeshComponent::setPoints(std::vector<glm::vec3> &_points) {
        points = std::move(points);
    }
}