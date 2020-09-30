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

yang::FColor yang::IColor::ToFColor() const
{
    FColor retColor;
    retColor.r = static_cast<f32>(Red()) / 255.f;
    retColor.g = static_cast<f32>(Green()) / 255.f;
    retColor.b = static_cast<f32>(Blue()) / 255.f;
    retColor.a = static_cast<f32>(Alpha()) / 255.f;

    return retColor;
}
