#ifndef HANDOVERS_H
#define HANDOVERS_H
#include "global.h"

namespace handovers::helpers {
    QString makeGeranCellId(const QString &nodeId, const QString &localCellId) noexcept;
    QString getRBSFromCellId(const QString &rbs) noexcept;
    void removeSpaces(QString &source) noexcept;
};

struct BaseHandovers
{
protected:
    enum class ColumnRole {
        Cell,
        BCCH,
        BSIC,
        LAC,
        Controller,
        CellId,
        ScrCode,
        DwnFrequency,
        PCI
    };
public:
    [[nodiscard]] virtual QString make(const QStringList &rows) noexcept = 0;
    virtual QMap<ColumnRole, std::size_t> columnRoles() const noexcept = 0;
    virtual size_t neighbourShift() const noexcept = 0;
};

#endif // HANDOVERS_H
