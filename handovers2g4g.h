#ifndef HANDOVERS2G4G_H
#define HANDOVERS2G4G_H

#include "handoversxg4g.h"

class Handovers2G4G : public HandoversXG4G
{
public:
    Handovers2G4G() = default;

    [[nodiscard]] virtual QString make(const QStringList &rows) override;
    [[nodiscard]] virtual bool loadTemplates() const override;
    [[nodiscard]] virtual QMap<ColumnRole, std::size_t> columnRoles() const override;
    [[nodiscard]] virtual size_t neighbourShift() const override;
protected:
    [[nodiscard]] virtual bool loadEricssonTemplatesLTE() const override;
    [[nodiscard]] virtual bool loadHuaweiTemplatesLTE() const override;
};

#endif // HANDOVERS2G4G_H
