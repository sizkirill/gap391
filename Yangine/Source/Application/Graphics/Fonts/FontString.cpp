#include "FontString.h"
#include <Application/Graphics/Textures/ITexture.h>
#include <Application/Graphics/Textures/Sprite.h>
#include <Application/Graphics/IGraphics.h>
#include <Application/Graphics/Fonts/IFont.h>
#include <Utils/Rectangle.h>

bool yang::FontString::Render(yang::IGraphics* pGraphics, FVec2 position, yang::IColor color)
{
    FVec2 actualPosition = position;

    if (m_text.size() != 0)
    {
        auto pWeakFontTexture = m_text[0]->GetSourceTexture();
        auto pFont = m_pFont.lock();

        if (auto pFontTexture = pWeakFontTexture.lock(); pFontTexture != nullptr && pFont != nullptr)
        {
            yang::IColor currentTint = pFontTexture->GetTint();
            pFontTexture->SetTint(color);

            IRect dest{ 0,0,0,0 };
            int currentX = (int)position.x;
            int currentY = (int)position.y;

            for (size_t i = 0; i < m_text.size(); ++i)
            {
                dest.x = currentX + pFont->GetXOffset(m_string[i]);
                dest.y = currentY;
                dest.width = m_text[i]->GetSourceRect().width;
                dest.height = m_text[i]->GetSourceRect().height;

                pGraphics->DrawSprite(m_text[i], dest);
                currentX += pFont->GetAdvance(m_string[i]);
            }

            pFontTexture->SetTint(currentTint);
        }
    }

    return true;
}