#include "handovermaker.h"
#include <QFile>

QString HandoverMaker::makeHandovers(HandoverType type, const QString &filename, VendorLTE) noexcept
{
    QStringList rows = getRowsFromFile(filename);

}

QStringList HandoverMaker::getRowsFromFile(const QString &filename) noexcept
{
    const char *delitemeter = "\r\n";
    QStringList rows;
    QFile file(filename);
    if (file.exists() &&
        file.open(QIODevice::ReadOnly)) {
        rows = QString(file.readAll()).split(delitemeter);
        file.close();
    }

    for (QString &row : rows) {
        handovers::helpers::removeSpaces(row);
    }

    return rows;
}
