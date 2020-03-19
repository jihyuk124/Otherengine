#include "Component/CircleComponent.h"
#include "Actor/Actor.h"
#include "Engine.h"

namespace Game
{
	CCircleComponent::CCircleComponent(AActor& owner, int updateOrder)
		:CActorComponent{owner, updateOrder}
	{
	}

	CCircleComponent::~CCircleComponent()
	{
		GetEngine().UnregisterCollision(*this);
	}

	void CCircleComponent::BeginPlay()
	{
		GetEngine().RegisterCollision(*this);
	}

	void CCircleComponent::TestOverlap(CCircleComponent& other)
	{
		if (!onOverlap_ && !other.onOverlap_) return;
		
		const auto distsqr = GetOwner().GetPos().DistSqr(other.GetOwner().GetPos());
		const auto r = radius_ + other.radius_;

		if (distsqr < r*r)
		{
			if (onOverlap_) onOverlap_(other);
			if (other.onOverlap_) other.onOverlap_(*this);
		}
	}

	void CCircleComponent::BindOnOverlap(std::function<void(CCircleComponent&)>&& onOverlap) noexcept
	{
		onOverlap_ = std::move(onOverlap);
	}
}