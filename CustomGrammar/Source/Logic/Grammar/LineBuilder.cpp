#include "LineBuilder.h"
#include <cassert>
#include <Utils/Matrix.h>
#include <Utils/Math.h>
#include <Logic/Scripting/LuaManager.h>
#include <Utils/Random.h>

void LineBuilder::MoveTransform(float amount)
{
    m_transform.m_position += m_transform.m_forward * amount;
}

void LineBuilder::RotateTransform(float amount)
{
    yang::Matrix m;
    m.Rotate(Math::ToRadians(amount));
    m_transform.m_forward = m.TransformPoint(m_transform.m_forward);
}

void LineBuilder::PushTransform()
{
    m_transformQueue.push(m_transform);
}

void LineBuilder::PopTransform()
{
    assert(!m_transformQueue.empty());
    m_transform = m_transformQueue.back();
    m_transformQueue.pop();
}

void LineBuilder::AddVertex(uint32_t color)
{
    m_lineList.emplace_back(m_transform.m_position, yang::IColor(color));
}

void LineBuilder::AddRect(int width, int height, uint32_t color)
{
    yang::IRect dest;
    dest.x = (int)m_transform.m_position.x - width / 2;
    dest.y = (int)m_transform.m_position.y - height / 2;
    dest.width = width;
    dest.height = height;

    m_rectList.emplace_back(dest, yang::IColor(color));
}

void LineBuilder::SetTransform(yang::FVec2 position, yang::FVec2 forward)
{
    m_initialTransform = { position, forward };
    m_transform = m_initialTransform;
}

void LineBuilder::ExposeToLua(const yang::LuaManager& luaenv)
{
    luaenv.ExposeToLua("AddVertex", &LineBuilder::AddVertex);
    luaenv.ExposeToLua("PushTransform", &LineBuilder::PushTransform);
    luaenv.ExposeToLua("PopTransform", &LineBuilder::PopTransform);
    luaenv.ExposeToLua("RotateTransform", &LineBuilder::RotateTransform);
    luaenv.ExposeToLua("MoveTransform", &LineBuilder::MoveTransform);
    luaenv.ExposeToLua("AddRect", &LineBuilder::AddRect);
}

void LineBuilder::Reset()
{
    m_transform = m_initialTransform;
    while (!m_transformQueue.empty())
    {
        m_transformQueue.pop();
    }
    m_lineList.clear();
    m_rectList.clear();
}
