#ifndef HANDOVERS2G3G_H
#define HANDOVERS2G3G_H
#include "handovers.h"

class Handovers2G3G : public BaseHandovers
{
public:
    Handovers2G3G() = default;

    [[nodiscard]] virtual QString make(const QStringList &rows) override;
    [[nodiscard]] virtual QMap<ColumnRole, std::size_t> columnRoles() const override;
    virtual size_t neighbourShift() const override;
    virtual bool loadTemplates() const override;
};

#endif // HANDOVERS2G3G_H
