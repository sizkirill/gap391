#include "Color.h"

yang::FColor::FColor(f32 r, f32 g, f32 b, f32 a)
    :r(r)
    , g(g)
    , b(b)
    , a(a)
{
}

yang::IColor yang::FColor::ToIColor() const
{
    return IColor(static_cast<ui8>(r * 255.f)
        , static_cast<ui8>(g * 255.f)
        , static_cast<ui8>(b * 255.f)
        , static_cast<ui8>(a * 255.f));
}

yang::IColor::IColor(ui8 r, ui8 g, ui8 b, ui8 a)
    :m_color(r << 24 | g << 16 | b << 8 | a)
{
}

yang::IColor::IColor(uint32_t color)
    : m_color(color)
{
}

yang::FColor yang::IColor::ToFColor() const
{
    FColor retColor;
    retColor.r = static_cast<f32>(Red()) / 255.f;
    retColor.g = static_cast<f32>(Green()) / 255.f;
    retColor.b = static_cast<f32>(Blue()) / 255.f;
    retColor.a = static_cast<f32>(Alpha()) / 255.f;

    return retColor;
}

uint8_t yang::IColor::Red() const
{
    return (m_color >> 24) & 0xFF;
}

uint8_t yang::IColor::Green() const
{
    return (m_color >> 16) & 0xFF;
}

uint8_t yang::IColor::Blue() const
{
    return (m_color >> 8) & 0xFF;
}

uint8_t yang::IColor::Alpha() const
{
    return m_color & 0xFF;
}
