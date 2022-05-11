#ifndef HANDOVERS2G2G_H
#define HANDOVERS2G2G_H
#include "handovers.h"

class Handovers2G2G : public BaseHandovers
{
public:
    Handovers2G2G() = default;

    [[nodiscard]] virtual QString make(const QStringList &rows) noexcept override;
    virtual QMap<BaseHandovers::ColumnRole, std::size_t> columnRoles() const noexcept override;\
    virtual size_t neighbourShift() const noexcept override;
};

#endif // HANDOVERS2G2G_H
