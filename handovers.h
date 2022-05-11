#ifndef HANDOVERS_H
#define HANDOVERS_H
#include "global.h"

namespace handovers::helpers {
    [[nodiscard]] QString makeGeranCellId(const QString &nodeId, const QString &localCellId) ;
    [[nodiscard]] QString getRBSFromCellId(const QString &rbs) ;
    [[nodiscard]] QString NCC(const QString &BSIC) ;
    [[nodiscard]] QString BCC(const QString &BSIC) ;
    [[nodiscard]] QString loadTemplate(const QString &filepath);
    [[nodiscard]] bool isRowFits(const QStringList &elements, std::size_t colCount);
    [[nodiscard]] QString getCellId(const QString &cellname);
    void transformCell(QString &cell);
    void removeSpaces(QString &source) ;
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

    struct EricssonHandovers {
        QString defaultHandovers = "<p style=\"color:red;font-size:18px\">ERICSSON DEFAULT HANDOVERS\n" + QString(20, '=') + "</p>\n";
        QString extCells = "<p style=\"color:red;font-size:18px\">ERICSSON EXT CELLS\n"  + QString(20, '=') + "</p>\n";
        QString extHandovers = "<p style=\"color:red;font-size:18px\">ERICSSON EXT HANDOVERS\n" + QString(20, '=') + "</p>\n";
    };
public:
    [[nodiscard]] virtual QString make(const QStringList &rows)  = 0;
    virtual bool loadTemplates() const = 0;
    virtual QMap<ColumnRole, std::size_t> columnRoles() const  = 0;
    virtual size_t neighbourShift() const  = 0;
};

#endif // HANDOVERS_H
