#include "handovermaker.h"
#include "handovers2g2g.h"
#include "handovers2g3g.h"
#include <QFile>

QString HandoverMaker::makeHandovers(HandoverType type, const QString &filename, VendorLTE) noexcept
{
    QStringList rows = getRowsFromFile(filename);

    QScopedPointer<BaseHandovers> handovers;

    switch(type)
    {
        case HandoverType::INVALID:
            return "IVALID HANDOVER TYPE";
        break;
        case HandoverType::GSM_TO_GSM:
            handovers.reset(new Handovers2G2G);
        break;
        case HandoverType::GSM_TO_UMTS:
            handovers.reset(new Handovers2G3G);
        break;
        default:
            return "IN WORK...";
        break;
    }

    return handovers.data()->make(rows);
}

QStringList HandoverMaker::getRowsFromFile(const QString &filename) noexcept
{
    const char *delitemeter = "\r\n";
    QStringList rows;
    QFile file(filename);
    if (file.exists() &&
        file.open(QIODevice::ReadOnly)) {
        rows = QString(file.readAll()).split(delitemeter, Qt::SkipEmptyParts);
        file.close();
    }

    for (QString &row : rows) {
        handovers::helpers::removeSpaces(row);
    }

    return rows;
}
