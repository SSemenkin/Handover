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
    return BSIC.length() == 1 ? "0" : BSIC.left(1);
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
