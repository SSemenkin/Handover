#include "handovers3g3g.h"
#include "mainwindow.h"

namespace {
    QString interHOTemplate;
    QString intraHOTemplate;

    QString makeInterHO(const QString &cellA,
                        const QString &cellB)
    {
        return interHOTemplate.arg(cellA, cellB);
    }
    QString makeIntraHO(const QString &cellA,
                        const QString &cellB)
    {
        return intraHOTemplate.arg(cellA, cellB);
    }

}

QString Handovers3G3G::make(const QStringList &rows)
{
    if (!loadTemplates()) {
        return "Failed to load templates files.";
    }

    QString errors = "ERRORS WHILE EXECUTING\n" + QString(20, '=') + '\n';
    QMap<QString, QString> cellIdToLAC = MainWindow::cellIdToLAC();

    QString intraHandovers = "<p style=\"color:red;font-size:18px\">UMTS INTRA HANDOVERS\n" + QString(20, '=') + "</p>\n";
    QString interHandovers = "<p style=\"color:red;font-size:18px\">UMTS INTER HANDOVERS\n" + QString(20, '=') + "</p>\n";

    QMap<ColumnRole, std::size_t> colRoles = columnRoles();

    for (int i = 1; i < rows.size(); ++i) {
        QStringList elements = rows[i].split(csv_delimeter, Qt::SkipEmptyParts);

        if (!handovers::helpers::isRowFits(elements, neighbourShift() * 2)) {
            errors += "Not enought arguments to make HO;\nRow :" + rows[i] + "\n";
            continue;
        }

        handovers::helpers::removeSpaces(elements[colRoles[ColumnRole::CellId]]);
        handovers::helpers::removeSpaces(elements[colRoles[ColumnRole::CellId] + neighbourShift()]);

        const QString &cellIdA = elements.at(colRoles[ColumnRole::CellId]);
        const QString &cellIdB = elements.at(colRoles[ColumnRole::CellId] + neighbourShift());

        interHandovers += makeInterHO(cellIdA, cellIdB);
        interHandovers += makeInterHO(cellIdB, cellIdA);
        intraHandovers += makeIntraHO(cellIdA, cellIdB);
        intraHandovers += makeIntraHO(cellIdB, cellIdA);

    }

    return interHandovers + intraHandovers + errors;
}

bool Handovers3G3G::loadTemplates() const
{
    interHOTemplate = handovers::helpers::loadTemplate("templates/3g3g/1.txt");
    intraHOTemplate = handovers::helpers::loadTemplate("templates/3g3g/2.txt");

    return !intraHOTemplate.isEmpty() && !interHOTemplate.isEmpty();
}

QMap<BaseHandovers::ColumnRole, std::size_t> Handovers3G3G::columnRoles() const
{
    return {
        {ColumnRole::Cell, 0},
        {ColumnRole::DwnFrequency, 2},
        {ColumnRole::ScrCode, 3},
        {ColumnRole::CellId, 1}
    };
}

size_t Handovers3G3G::neighbourShift() const
{
    return 4;
}
