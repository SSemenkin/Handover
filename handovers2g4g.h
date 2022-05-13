#ifndef HANDOVERS2G4G_H
#define HANDOVERS2G4G_H

#include "handoversxg4g.h"

class Handovers2G4G : public HandoversXG4G
{
public:
    Handovers2G4G() = default;

    virtual QString make(const QStringList &rows) override;
    virtual bool loadTemplates() const override;
    virtual QMap<ColumnRole, std::size_t> columnRoles() const override;
    virtual size_t neighbourShift() const override;
};

#endif // HANDOVERS2G4G_H
