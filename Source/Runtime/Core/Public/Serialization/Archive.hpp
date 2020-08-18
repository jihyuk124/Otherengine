#pragma once

namespace oeng::core
{
class Archive
{
public:
    INTERFACE_BODY(Archive);

    [[nodiscard]] virtual bool IsLoading() const noexcept = 0;

    virtual Archive& operator<<(Json& json) = 0;
};
}
