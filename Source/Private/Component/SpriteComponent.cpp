#include "Components/SpriteComponent.hpp"
#include <GL/glew.h>
#include "Actor.hpp"
#include "Engine.hpp"
#include "Graphics/Renderer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Texture.hpp"

namespace oeng
{
	CSpriteComponent::CSpriteComponent(AActor& owner, const int draw_order, const int update_order)
		:CActorComponent{owner, update_order}, draw_order_{draw_order}
	{
	}

	CSpriteComponent::~CSpriteComponent()
	{
		GetRenderer().UnregisterSprite(*this);
	}

	void CSpriteComponent::BeginPlay()
	{
		GetRenderer().RegisterSprite(*this);
	}

	void CSpriteComponent::SetTexture(std::shared_ptr<Texture>&& texture)
	{
		texture_ = std::move(texture);
	}

	void CSpriteComponent::SetTexture(const std::shared_ptr<Texture>& texture)
	{
		texture_ = texture;
	}

	void CSpriteComponent::SetTexture(Path file)
	{
		SetTexture(GetEngine().GetTexture(file));
	}

	Renderer& CSpriteComponent::GetRenderer() const noexcept
	{
		return GetEngine().GetRenderer();
	}

	void CSpriteComponent::Draw(Shader& shader) const
	{
		static const Name name = "uWorldTransform";
		shader.SetMatrixUniform(name, MakeScale<4>({texture_->Size(), 1}) * GetOwner().GetTransformMatrix());
		texture_->Activate();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
	}
}
