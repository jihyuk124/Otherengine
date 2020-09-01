#include "Engine/Mesh.hpp"
#include "AssetManager.hpp"
#include "DynamicRHI.hpp"
#include "RHIMesh.hpp"
#include "Vertex.hpp"
#include "Materials/MaterialInterface.hpp"

namespace oeng
{
inline namespace engine
{
void Mesh::Activate() const noexcept
{
    rhi_->Activate();
}

void Mesh::from_json(const Json& json)
{
    const auto vertices = json.at("vertices").get<std::vector<Vertex>>();
    const auto indices = json.at("indices").get<std::vector<Vec3u16>>();

    material_ = AssetManager::Get().Load<MaterialInterface>(json.at("material").get<Path>());
    rhi_.reset(DynamicRHI::Get().CreateMesh(vertices, indices));

    auto max = 0_f;
    for (const auto& v : vertices)
        max = Max(max, v.pos.DistSqr(Vec3::zero));
    radius_ = std::sqrt(max);
}
}
}
