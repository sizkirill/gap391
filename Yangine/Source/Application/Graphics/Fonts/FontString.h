#pragma once
#include <vector>
#include <memory>
#include <string>
#include <Utils/Vector2.h>
#include <Utils/Color.h>

namespace yang
{
    class Sprite;
    class IFont;
    class IGraphics;

    struct FontString
    {
    public:
        bool Render(yang::IGraphics* pGraphics, FVec2 position, yang::IColor color);
        std::vector<std::shared_ptr<Sprite>> m_text;
        std::string m_string;
        std::weak_ptr<const IFont> m_pFont;
    };
}