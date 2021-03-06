#pragma once
#include "Camera/CameraTypes.hpp"
#include "TimerManager.hpp"
#include <unordered_set>

namespace logcat
{
extern ENGINE_API const LogCategory kActor;
}

namespace oeng
{
inline namespace engine
{

class World;
class Engine;
class ActorComponent;
class SceneComponent;

class ENGINE_API AActor : public Object
{
    CLASS_BODY(AActor)

  public:
    void Destroy();

    template <class T> T& AddComponent()
    {
        auto ptr = MakeShared<T>();
        auto& ref = *ptr;
        RegisterComponent(std::move(ptr));
        return ref;
    }

    template <class T> T& AddComponent(int update_order)
    {
        auto ptr = MakeShared<T>();
        auto& ref = *ptr;
        ref.update_order_ = update_order;
        RegisterComponent(std::move(ptr));
        return ref;
    }

    [[nodiscard]] virtual ViewInfo CalcCamera() const;

    /**
     * Add tag.
     * @param tag Name of the tag to be added.
     * @return `true` if added successfully. `false` if already exists.
     */
    bool AddTag(Name tag)
    {
        return tags_.insert(tag).second;
    }

    /**
     * Remove tag.
     * @param tag Name of the tag to be removed.
     * @return `true` if removed successfully. `false` if not found.
     */
    bool RemoveTag(Name tag)
    {
        return tags_.erase(tag);
    }

    /**
     * Check if the actor has given tag.
     * @param tag Name of the tag.
     * @return `true` if found.
     */
    [[nodiscard]] bool HasTag(Name tag) const noexcept
    {
        return tags_.contains(tag);
    }

    /**
     * Set root component of this actor. Root component represents this actor's transform.
     * @param new_root New root component. It can be null or MUST be owned by this actor.
     */
    void SetRootComponent(SceneComponent* new_root) noexcept;

    [[nodiscard]] SceneComponent* GetRootComponent() const noexcept
    {
        return root_;
    }

    [[nodiscard]] bool IsPendingKill() const noexcept
    {
        return pending_kill_;
    }

    /**
     * Set actor's lifespan. Default is 0 (infinite). Timer is updated when called.
     * @param in_seconds New lifespan in seconds. <=0 means infinite.
     */
    void SetLifespan(Float in_seconds);
    [[nodiscard]] Float GetLifespan() const noexcept;

    [[nodiscard]] Float GetInitialLifespan() const noexcept
    {
        return init_lifespan_;
    }

    void SetPos(const Vec3& pos) const noexcept;
    void SetRot(const Quat& rot) const noexcept;
    void SetScale(const Vec3& scale) const noexcept;
    void SetTrsf(const Transform& trsf) const noexcept;
    [[nodiscard]] const Vec3& GetPos() const noexcept;
    [[nodiscard]] const Quat& GetRot() const noexcept;
    [[nodiscard]] const Vec3& GetScale() const noexcept;
    [[nodiscard]] const Transform& GetTrsf() const noexcept;

    [[nodiscard]] UVec3 GetForward() const noexcept;
    [[nodiscard]] UVec3 GetRight() const noexcept;
    [[nodiscard]] UVec3 GetUp() const noexcept;

    [[nodiscard]] bool HasBegunPlay() const noexcept
    {
        return begun_play_;
    }

    void SetOwner(WeakPtr<AActor> new_owner);

    [[nodiscard]] auto& GetOwner() const noexcept
    {
        return owner_;
    }

    [[nodiscard]] World& GetWorld() const noexcept
    {
        assert(world_);
        return *world_;
    }

    [[nodiscard]] Engine& GetEngine() const noexcept;

    /**
     * If false, the actor will not being updated. `true` by default.
     * Components are not affected by this property.
     */
    bool update_enabled = true;

    /**
     * @brief If true, invoking Destroy() will not delete the actor.
     */
    bool immortal = false;

  protected:
    virtual void OnBeginPlay()
    {
    }

    virtual void OnEndPlay()
    {
    }

    virtual void OnUpdate([[maybe_unused]] Float delta_seconds)
    {
    }

    virtual void OnSetOwner()
    {
    }

  private:
    friend World;

    void BeginPlay();
    void EndPlay();
    void Update(Float delta_seconds);

    void RegisterComponent(SharedRef<ActorComponent>&& comp);

    World* world_ = nullptr;
    SceneComponent* root_ = nullptr;
    WeakPtr<AActor> owner_;

    std::vector<SharedRef<ActorComponent>> comps_;
    std::unordered_set<Name> tags_;

    TimerHandle lifespan_timer_;
    Float init_lifespan_ = 0;

    bool pending_kill_ = false;
    bool begun_play_ = false;
};

} // namespace engine
} // namespace oeng
