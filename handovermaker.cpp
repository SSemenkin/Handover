#include "handovermaker.h"
#include "handovers2g2g.h"
#include "handovers2g3g.h"
#include "handovers3g3g.h"
#include <QFile>

namespace {
void shift(QStringList &rows) {
    QList<QStringList> rowRows;
    for(int i = 1; i < rows.size(); ++i) {
        QStringList elements = rows[i].split(csv_delimeter, Qt::SkipEmptyParts);
        QStringList shifted;
        for (int i = 4; i < elements.size(); ++i)
            shifted << elements[i];
        for (int i = 0; i < 4; ++i)
            shifted << elements[i];
        rowRows.append(shifted);
    }

    rows.clear();
    for (const QStringList &r : rowRows) {
        rows.append(r.join(csv_delimeter));
    }
}
}

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
        case HandoverType::UMTS_TO_GSM:
            handovers.reset(new Handovers2G3G);
            shift(rows);
        break;
        case HandoverType::UMTS_TO_UMTS:
            handovers.reset(new Handovers3G3G);
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
