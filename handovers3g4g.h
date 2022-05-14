#ifndef HANDOVERS3G4G_H
#define HANDOVERS3G4G_H

#include "handoversxg4g.h"

class Handovers3G4G : public HandoversXG4G
{
public:
    Handovers3G4G() = default;

    [[nodiscard]] virtual QString make(const QStringList &rows) override;
    [[nodiscard]] virtual bool loadTemplates() const override;
    [[nodiscard]] virtual QMap<ColumnRole, std::size_t> columnRoles() const override;
    size_t neighbourShift() const override;

protected:
    [[nodiscard]] virtual bool loadEricssonTemplatesLTE() const override;
    [[nodiscard]] virtual bool loadHuaweiTemplatesLTE() const override;
};

#endif // HANDOVERS3G4G_H
