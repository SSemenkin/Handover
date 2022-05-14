#include "handovermaker.h"
#include "handovers2g2g.h"
#include "handovers2g3g.h"
#include "handovers3g3g.h"
#include "handovers2g4g.h"
#include "handovers3g4g.h"
#include <QFile>
#include <QDebug>

namespace {
void shift(QStringList &rows, int shift_value) {
    QList<QStringList> rowRows;
    for(int i = 1; i < rows.size(); ++i) {
        QStringList elements = rows[i].split(csv_delimeter, Qt::SkipEmptyParts);
        QStringList shifted;
        for (int i = shift_value; i < elements.size(); ++i)
            shifted << elements[i];
        for (int i = 0; i < shift_value; ++i)
            shifted << elements[i];
        rowRows.append(shifted);
    }

    rows.clear();
    for (const QStringList &r : rowRows) {
        rows.append(r.join(csv_delimeter));
    }
}
}

QString HandoverMaker::makeHandovers(HandoverType type, const QString &filename, VendorLTE lteVendor) noexcept
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
        {
            case HandoverType::UMTS_TO_GSM:
                shift(rows, 4);
            case HandoverType::GSM_TO_UMTS:
                handovers.reset(new Handovers2G3G);
            break;
        }
        case HandoverType::UMTS_TO_UMTS:
            handovers.reset(new Handovers3G3G);
        break;
        {
            case HandoverType::LTE_TO_GSM:
                shift(rows, 5);
            case HandoverType::GSM_TO_LTE:
            {
                Handovers2G4G *obj = new Handovers2G4G;
                handovers.reset(obj);
                obj->setVendorLTE(lteVendor);
            }
            break;
        }
        {
        case HandoverType::LTE_TO_UMTS:
            shift(rows, 5);
        case HandoverType::UMTS_TO_LTE:
            Handovers3G4G *obj = new Handovers3G4G;
            handovers.reset(obj);
            obj->setVendorLTE(lteVendor);
            break;
        }
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
