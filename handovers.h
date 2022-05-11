#ifndef HANDOVERS_H
#define HANDOVERS_H
#include "global.h"

namespace handovers::handoversGSM_TO_GSM {
   QString make(const QStringList &rows) noexcept;

   static const QMap<QString, unsigned> columnOrder {
                                                      {"CELL", 0}
                                                     ,{"CELL_BCCH", 1}
                                                     ,{"CELL_BSIC", 2}
                                                     ,{"CELL_CONTROLLER", 3}
                                                     ,{"CELL_LAC", 4}
                                                     };
};

namespace handovers::helpers {
    QString makeGeranCellId(const QString &nodeId, const QString &localCellId) noexcept;
    void removeSpaces(QString &source) noexcept;
};

#endif // HANDOVERS_H
