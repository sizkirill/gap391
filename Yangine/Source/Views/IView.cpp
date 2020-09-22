#include "IView.h"

#include <Logic/Actor/Actor.h>
using yang::IView;


IView::IView()
    :m_index(0)
{
	
}

IView::~IView()
{

}

void yang::IView::SetControlledActor(std::shared_ptr<Actor> pActor)
{
    pActor->SetOwningView(this);
    m_pActor = pActor;
}

void yang::IView::DetachActor()
{
    m_pActor.reset();
}
