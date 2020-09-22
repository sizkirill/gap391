#include "UIScene.h"
#include <Application/Graphics/IGraphics.h>
#include <Logic/Actor/Actor.h>
#include <Logic/Components/TransformComponent.h>
#include <Logic/Components/TextComponent.h>
#include <Logic/Components/SpriteComponent.h>

using yang::UIScene;


yang::UIScene::UIScene()
	:m_pParent(nullptr)
	,m_dimensions(0.f,0.f)
	,m_relativePosition(0.f, 0.f)
{
}

yang::UIScene::UIScene(FVec2 dimensions, UIScene* pParent)
	:m_dimensions(dimensions)
	,m_pParent(pParent)
	,m_relativePosition(0.f, 0.f)
{
}

UIScene::~UIScene()
{
	m_children.clear();
	m_uiActors.clear();
}

UIScene* yang::UIScene::AddScene(FVec2 dimensions)
{
	auto& pRetVal = m_children.emplace_back(std::make_unique<UIScene>(dimensions, this));
	pRetVal->m_pParent = this;
	pRetVal->m_relativePosition = m_relativePosition;
	return pRetVal.get();
}

void yang::UIScene::AddActor(std::shared_ptr<Actor> pActor)
{
	if (auto pTransform = pActor->GetComponent<TransformComponent>(); pTransform != nullptr)
	{
		pTransform->SetPosition(FVec2(0, 0));
	}

	m_uiActors.push_back(std::move(pActor));
}

void yang::UIScene::Align(VerticalAlignment vAlign, HorizontalAlignment hAlign)
{
	FVec2 currentTop = { 0,0 };
	FVec2 currentBottom = m_dimensions;

	for (size_t i = 0; i < m_children.size(); ++i)
	{
		auto& pScene = m_children[i];
		auto sceneRelativePos = pScene->GetRelativePosition();
		auto sceneDimensions = pScene->GetDimensions();

		switch (vAlign)
		{
		case VerticalAlignment::kDown:
			currentBottom.y -= sceneDimensions.y;
			pScene->SetRelativePosition(FVec2(sceneRelativePos.x, currentBottom.y));
			break;
		case VerticalAlignment::kTop:
			pScene->SetRelativePosition(FVec2(sceneRelativePos.x, currentTop.y));
			currentTop.y += sceneDimensions.y;
			break;
		default:
			break;
		}

		switch (hAlign)
		{
		case HorizontalAlignment::kLeft:
			pScene->SetRelativePosition(FVec2(currentTop.x, sceneRelativePos.y));
			currentTop.x += sceneDimensions.x;
			break;
		case HorizontalAlignment::kRight:
			currentBottom.x -= sceneDimensions.x;
			pScene->SetRelativePosition(FVec2(currentBottom.x, sceneRelativePos.y));
		default:
			break;
		}
	}

	for (size_t i = 0; i < m_uiActors.size(); ++i)
	{
		auto& pActor = m_uiActors[i];
		auto pTransformComponent = pActor->GetComponent<TransformComponent>();

		if (!pTransformComponent)
			continue;

		auto& transformPosition = pTransformComponent->GetPosition();
		auto transformDimensions = pTransformComponent->GetDimensions();

		switch (vAlign)
		{
		case VerticalAlignment::kTop:
			transformPosition.y = currentTop.y;
			currentTop.y = transformPosition.y + transformDimensions.y;
			break;
		case VerticalAlignment::kDown:
			transformPosition.y = currentBottom.y - transformDimensions.y;
			currentBottom.y = transformPosition.y;
			break;
		default:
			break;
		}

		switch (hAlign)
		{
		case HorizontalAlignment::kLeft:
			transformPosition.x = currentTop.x;
			currentTop.x = transformPosition.x + transformDimensions.x;
			break;
		case HorizontalAlignment::kRight:
			transformPosition.x = currentBottom.x - transformDimensions.x;
			currentBottom.x = transformPosition.x;
			break;
		default:
			break;
		}
	}
}

void yang::UIScene::Render(IGraphics* pGraphics)
{
#ifdef _DEBUG
	InternalRender(pGraphics, m_relativePosition);
#endif
}

void yang::UIScene::InternalRender(IGraphics* pGraphics, FVec2 parentPos)
{
	FRect debugRect;
	debugRect.x = parentPos.x;
	debugRect.y = parentPos.y;
	debugRect.width = m_dimensions.x;
	debugRect.height = m_dimensions.y;

	pGraphics->DrawRect(debugRect, 0xFFFFFFFF);

	for (auto& pScene : m_children)
	{
		pScene->InternalRender(pGraphics, parentPos + m_relativePosition);
	}
}

void yang::UIScene::SetRelativePosition(FVec2 relativePosition)
{
	m_relativePosition = relativePosition;

	for (auto& pScene : m_children)
	{
		pScene->SetRelativePosition(m_relativePosition);
	}

	for (auto& pActor : m_uiActors)
	{
		auto pTransform = pActor->GetComponent<TransformComponent>();
		if (pTransform)
		{
			pTransform->SetPosition(pTransform->GetPosition() + m_relativePosition);
		}
	}
}
