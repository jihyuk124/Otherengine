#include "Components/MeshComponent.hpp"
#include "Engine.hpp"
#include "Mesh.hpp"

namespace oeng
{
	MeshComponent::MeshComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order},
		mesh_{GetRenderer().GetDefaultMesh()},
		material_{GetRenderer().GetDefaultMaterial()}
	{
	}

	MeshComponent::~MeshComponent()
	{
		if (HasBegunPlay()) GetRenderer().UnregisterMesh(*this);
	}

	void MeshComponent::SetMesh(Path file)
	{
		auto mesh = GetRenderer().GetMesh(file);
		material_ = mesh->GetMaterialShared();
		SetMesh(std::move(mesh));
	}

	void MeshComponent::SetMesh(SharedRef<Mesh> mesh)
	{
		if (!mesh) throw std::invalid_argument{"Mesh cannot be set to nullptr"};
		mesh_ = std::move(mesh);
		ReRegister();
	}

	void MeshComponent::SetMaterial(Path path)
	{
		SetMaterial(GetRenderer().GetMaterial(path));
	}

	void MeshComponent::SetMaterial(SharedRef<Material> material)
	{
		if (!material) throw std::invalid_argument{"Material cannot be set to nullptr"};
		material_ = std::move(material);
		ReRegister();
	}

	Renderer& MeshComponent::GetRenderer() const noexcept
	{
		return GetEngine().GetRenderer();
	}

	void MeshComponent::OnBeginPlay()
	{
		GetRenderer().RegisterMesh(*this);
	}

	void MeshComponent::ReRegister() const
	{
		if (HasBegunPlay())
		{
			auto& renderer = GetRenderer();
			renderer.UnregisterMesh(*this);
			renderer.RegisterMesh(*this);
		}
	}
}
