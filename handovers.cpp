#include "handovers.h"

QString handovers::helpers::makeGeranCellId(const QString &nodeId, const QString &localCellId) noexcept
{
    int geranCellId = (nodeId.toInt() + 100'000) * 256 + localCellId.toInt();
    return QString::number(geranCellId);
}

void handovers::helpers::removeSpaces(QString &source) noexcept
{
    source.remove(' ').remove('\t');
}

QString handovers::handoversGSM_TO_GSM::make(const QStringList &rows) noexcept
{

}
