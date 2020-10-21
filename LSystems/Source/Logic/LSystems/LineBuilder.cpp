#include "LineBuilder.h"
#include <cassert>
#include <Utils/Matrix.h>
#include <Utils/Math.h>
#include <Logic/Scripting/LuaManager.h>

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
    m_transformStack.push_back(m_transform);
}

void LineBuilder::PopTransform()
{
    assert(!m_transformStack.empty());
    m_transform = m_transformStack.back();
    m_transformStack.pop_back();
}

void LineBuilder::AddVertex(uint32_t color)
{
    m_lineList.emplace_back(m_transform.m_position, yang::IColor(color));
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
}

void LineBuilder::Reset()
{
    m_transform = m_initialTransform;
    m_transformStack.clear();
    m_lineList.clear();
}
