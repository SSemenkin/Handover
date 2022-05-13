#ifndef HANDOVERS3G3G_H
#define HANDOVERS3G3G_H

#include "handovers.h"

class Handovers3G3G : public BaseHandovers
{
public:
    Handovers3G3G() = default;

    [[nodiscard]] virtual QString make(const QStringList &rows) override;
    [[nodiscard]] virtual bool loadTemplates() const override;
    virtual QMap<ColumnRole, std::size_t> columnRoles() const override;
    virtual size_t neighbourShift() const override;
};

#endif // HANDOVERS3G3G_H
