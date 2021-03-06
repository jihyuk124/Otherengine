#include "DynamicRHI.hpp"

namespace oeng
{
inline namespace rhi
{
OE_EXPORT DynamicRHI* dynamic_rhi = nullptr;

DynamicRHI& DynamicRHI::Get() noexcept
{
    assert(dynamic_rhi);
    return *dynamic_rhi;
}
} // namespace rhi
} // namespace oeng
