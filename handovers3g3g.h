#ifndef HANDOVERS3G3G_H
#define HANDOVERS3G3G_H

#include "handovers.h"

class Handovers3G3G : public BaseHandovers
{
public:
    Handovers3G3G();

    [[nodiscard]] virtual QString make(const QStringList &rows) override;
    [[nodiscard]] virtual bool loadTemplates() const override;
    QMap<ColumnRole, std::size_t> columnRoles() const override;
    size_t neighbourShift() const override;
};

#endif // HANDOVERS3G3G_H
