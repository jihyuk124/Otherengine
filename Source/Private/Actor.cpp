#include "Actor.hpp"
#include "Assert.hpp"
#include "Components/SceneComponent.hpp"
#include "World.hpp"

namespace oeng
{
	AActor::AActor(CWorld& world)
		:world_{world}
	{
	}

	AActor::~AActor()
	{
		GetWorld().GetTimerManager().RemoveTimer(lifespan_timer_);
	}

	void AActor::BeginPlay()
	{
		for (const auto& c : comps_)
			c->BeginPlay();

		begun_play_ = true;
		SetLifespan(init_lifespan_);
	}

	void AActor::Update(const float delta_seconds)
	{
		UpdateComponents(delta_seconds);
		UpdateActor(delta_seconds);
	}

	void AActor::UpdateComponents(const float delta_seconds)
	{
		for (const auto& comp : comps_)
		{
			comp->Update(delta_seconds);
		}
	}

	Engine& AActor::GetEngine() const noexcept
	{
		return world_.GetEngine();
	}

	TimerManager& AActor::GetTimerManager() const noexcept
	{
		return world_.GetTimerManager();
	}

	void AActor::RegisterComponent(SharedPtr<ActorComponent>&& comp)
	{
		auto cmp = [](auto&& a, auto&& b)
		{
			return a->GetUpdateOrder() <= b->GetUpdateOrder();
		};
		const auto pos = std::lower_bound(comps_.begin(), comps_.end(), comp, cmp);
		comps_.insert(pos, std::move(comp));
	}

	void AActor::Destroy()
	{
		pending_kill_ = true;
	}

	void AActor::SetRootComponent(WeakPtr<SceneComponent> new_root) noexcept
	{
		CHECK(new_root.Expired() || &new_root.Lock()->GetOwner() == this);
		root_ = std::move(new_root);
	}

	void AActor::SetEnabled(bool enable)
	{
		for (auto& c : comps_)
		{
			c->SetEnabled(enable);
		}
	}

	void AActor::SetLifespan(float in_seconds)
	{
		init_lifespan_ = in_seconds;
		if (!begun_play_) return;

		auto& timer = GetWorld().GetTimerManager();
		if (timer.IsTimerExists(lifespan_timer_))
		{
			if (init_lifespan_ > 0)
			{
				timer.UpdateTimer(lifespan_timer_, init_lifespan_);
			}
			else
			{
				timer.RemoveTimer(lifespan_timer_);
			}
		}
		else if (init_lifespan_ > 0)
		{
			lifespan_timer_ = timer.SetTimer(init_lifespan_, [self = WeakFromThis()]
			{
				if (auto ptr = self.Lock())
				{
					ptr->Destroy();
				}
			});
		}
	}

	float AActor::GetLifespan() const noexcept
	{
		auto& timer = GetWorld().GetTimerManager();
		return timer.IsTimerExists(lifespan_timer_) ? timer.TimeLeft(lifespan_timer_) : 0;
	}

	void AActor::SetTransform(const Transform& new_transform, bool recalc_matrix) const noexcept
	{
		if (auto r = root_.Lock()) r->SetRelTransform(new_transform, recalc_matrix);
	}

	const Transform& AActor::GetTransform() const noexcept
	{
		const auto r = root_.Lock();
		return r ? r->GetRelTransform() : Transform::identity;
	}

	void AActor::SetPos(const Vec3& new_pos, bool recalc_matrix) const noexcept
	{
		if (auto r = root_.Lock()) r->SetRelPos(new_pos, recalc_matrix);
	}

	const Vec3& AActor::GetPos() const noexcept
	{
		const auto r = root_.Lock();
		return r ? r->GetRelPos() : Vec3::zero;
	}

	void AActor::SetRot(const Quat& new_rot, bool recalc_matrix) const noexcept
	{
		if (auto r = root_.Lock()) r->SetRelRot(new_rot, recalc_matrix);
	}

	const Quat& AActor::GetRot() const noexcept
	{
		const auto r = root_.Lock();
		return r ? r->GetRelRot() : Quat::identity;
	}

	void AActor::SetScale(const Vec3& scale, bool recalc_matrix) const noexcept
	{
		if (auto r = root_.Lock()) r->SetRelScale(scale, recalc_matrix);
	}

	const Vec3& AActor::GetScale() const noexcept
	{
		const auto r = root_.Lock();
		return r ? r->GetRelScale() : Vec3::one;
	}

	void AActor::RecalcMatrix() const noexcept
	{
		if (auto r = root_.Lock()) r->RecalcWorldTransform();
	}

	const Mat4& AActor::GetTransformMatrix() const noexcept
	{
		const auto r = root_.Lock();
		return r ? r->GetWorldTransform() : Mat4::identity;
	}
}
