#include "handovers.h"
#include <QMessageBox>
#include <QFile>

QString handovers::helpers::makeGeranCellId(const QString &nodeId, const QString &localCellId)
{
    int geranCellId = (nodeId.toInt() + 100'000) * 256 + localCellId.toInt();
    return QString::number(geranCellId);
}

void handovers::helpers::removeSpaces(QString &source)
{
    source.remove(' ').remove('\t');
}

QString handovers::helpers::getRBSFromCellId(const QString &rbs)
{
    return rbs.left(rbs.length() - 1);
}

QString handovers::helpers::NCC(const QString &BSIC)
{
    return BSIC.length() == 1 ? "0" : QString(BSIC.at(0));
}

QString handovers::helpers::BCC(const QString &BSIC)
{
    return BSIC.right(1);
}

QString handovers::helpers::loadTemplate(const QString &filepath)
{
    QFile f(filepath);
    if (!f.exists()) {
        QMessageBox::information(nullptr, "Template not found", "Template " + filepath + " not found.");
        return QString();
    }
    if (!f.open(QIODevice::ReadOnly)) {
        QMessageBox::information(nullptr, "Cant open file", "Cannot open template " + f.fileName() + " for reading.\n" + f.errorString());
        return QString();
    }

    QString r = f.readAll();
    return r;
}

void handovers::helpers::transformCell(QString &cell)
{
    bool ok{false};
    cell.remove('_');
    int sector = cell.rightRef(1).toInt(&ok);
    if (cell.contains("LU") && ok) {
        cell = handovers::helpers::getRBSFromCellId(cell);
        cell += sector == 1 ? "A" : sector == 2 ? "B" : "C";
    }
}

bool handovers::helpers::isRowFits(const QStringList &list, std::size_t colCount)
{
    return (std::size_t)list.size() == colCount;
}

QString handovers::helpers::getCellId(const QString &cellname)
{
    QString result;
    for (auto &c : cellname) {
        if(c.isDigit()) {
            result += c;
        }
    }
    if (cellname.right(1) == "A") {
        result += "1";
    } else if (cellname.right(1) == "B") {
        result += "2";
    } else if (cellname.right(1) == "C") {
        result += "3";
    }

    return result;
}

bool handovers::helpers::isNotUnique(QMap<QString, QStringList> &contrianer, const QString &toController, const QString &cellname)
{
    bool result = contrianer[toController].contains(cellname);
    if (!result) contrianer[toController].push_back(cellname);
    return result;
}

QString handovers::helpers::lteRBSName(const QString &lteCellName)
{
    int index = lteCellName.indexOf('_');
    if (index == -1) {
        QMessageBox::information(nullptr, "Error", "Cannot take LTE RBS Name from : " + lteCellName);
        return "";
    }
    QString leftPart = lteCellName.left(index);
    QString result;
    for (auto &c : leftPart) {
        if (c.isDigit()) {
            result += c;
        }
    }
    return result;
}

QString handovers::helpers::lteLocalCellId(const QString &cellName)
{
    int localCellId = 10 + (cellName[cellName.length() -1].toLatin1() - '0');

    if (localCellId >= 11 && localCellId <= 13) {
        return QString::number(localCellId);
    }

    QMessageBox::information(nullptr, "Error", "LOCAL CELL ID IS "  + QString::number(localCellId));
    return "";
}

QString handovers::helpers::lteCellIndex(const QString &cellname)
{
    return lteRBSName(cellname) + cellname.rightRef(1);
}

QString handovers::helpers::umtsGetUplinkFrequency(const QString &dnlFrequency)
{
    bool ok {false};
    QString result = QString::number(dnlFrequency.toInt(&ok) - 950);

    if (!ok) {
        QMessageBox::warning(nullptr, "Error", "Cannot convert " + dnlFrequency + " to integer.");
        return "";
    }

    return result;
}
