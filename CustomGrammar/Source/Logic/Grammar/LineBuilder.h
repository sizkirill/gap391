#pragma once
#include <vector>
#include <Utils/Vector2.h>
#include <Utils/Color.h>
#include <Utils/Rectangle.h>
#include <queue>

namespace yang
{
    class LuaManager;
}

class LineBuilder
{
public:
    struct Transform
    {
        yang::FVec2 m_position = {0.f,0.f};
        yang::FVec2 m_forward = { 0.f,1.f };
    };

    LineBuilder() = default;
    
    void MoveTransform(float amount);
    void RotateTransform(float amount);
    void PushTransform();
    void PopTransform();
    void AddVertex(uint32_t color);
    void AddRect(int width, int height, uint32_t color);
    void SetTransform(yang::FVec2 position, yang::FVec2 forward);

    const auto& GetLineList() const { return m_lineList; }
    const auto& GetRectList() const { return m_rectList; }
    static void ExposeToLua(const yang::LuaManager& luaenv);

    void Reset();
private:
    Transform m_initialTransform;
    Transform m_transform;
    std::queue<Transform> m_transformQueue;

    std::vector<std::pair<yang::FVec2,yang::IColor>> m_lineList;
    std::vector<std::pair<yang::IRect, yang::IColor>> m_rectList;
};