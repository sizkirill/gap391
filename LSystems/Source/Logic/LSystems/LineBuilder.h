#pragma once
#include <vector>
#include <Utils/Vector2.h>
#include <Utils/Color.h>

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
    void SetTransform(yang::FVec2 position, yang::FVec2 forward);

    const auto& GetLineList() const { return m_lineList; }
    static void ExposeToLua(const yang::LuaManager& luaenv);

    void Reset();
private:
    Transform m_initialTransform;
    Transform m_transform;
    std::vector<Transform> m_transformStack;
    std::vector<std::pair<yang::FVec2,yang::IColor>> m_lineList;
};