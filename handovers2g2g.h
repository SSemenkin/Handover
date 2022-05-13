#ifndef HANDOVERS2G2G_H
#define HANDOVERS2G2G_H
#include "handovers.h"

class Handovers2G2G : public BaseHandovers
{
public:
    Handovers2G2G() = default;

    [[nodiscard]] virtual QString make(const QStringList &rows) override;
    [[nodiscard]] virtual QMap<ColumnRole, std::size_t> columnRoles() const override;\
    [[nodiscard]] virtual size_t neighbourShift() const override;
    [[nodiscard]] virtual bool loadTemplates() const override;
};

#endif // HANDOVERS2G2G_H
