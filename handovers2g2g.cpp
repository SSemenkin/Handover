#include "handovers2g2g.h"

namespace {

    QMap<QString, QStringList> uniqueExtCells;

    QString huaweiHandoverTemplate;
    QString huaweiExtCellTemplate;
    QString ericssonDefaultHandoverTemplate;
    QString ericssonExtCellTemplate;
    QString ericssonExtHandoverTemplate;

    QString huaweiExternalCell(const QString &toController,
                               const QString &cellname,
                               const QString &LAC,
                               const QString &BCCH,
                               const QString &BSIC) {
        if(handovers::helpers::check(uniqueExtCells, toController, cellname)) {
            return "";
        }
        return huaweiExtCellTemplate.arg(cellname, LAC, handovers::helpers::getCellId(cellname), BCCH, handovers::helpers::NCC(BSIC), handovers::helpers::BCC(BSIC));
    }

    QString huaweiHandover(const QString &cellA, const QString &cellB) {
        return huaweiHandoverTemplate.arg(cellA, cellB);
    }

    QString ericssonExternalCell(const QString &toController,
                                 const QString &cellname, const QString &LAC,
                                 const QString &BSIC,
                                 const QString &BCCH) {

        if(handovers::helpers::check(uniqueExtCells, toController, cellname)) {
            return "";
        }
        const QString gsmStandart = BCCH.toInt() >= 512 && BCCH.toInt() <= 885 ? "GSM1800" : "GSM900";
        const QString power = gsmStandart == "GSM900" ? "47" : "45";
        return ericssonExtCellTemplate.arg(cellname, LAC, handovers::helpers::getCellId(cellname), BSIC, BCCH, gsmStandart, power);
    }

    QString ericssonExternalHandover(const QString &cellA, const QString &cellB,
                                     const QString &BCCH_B) {
        return ericssonExtHandoverTemplate.arg(cellA, cellB, BCCH_B);
    }

    QString ericssonDefaultHandover(const QString &cellA, const QString &cellB,
                                    const QString &BCCH_A, const QString &BCCH_B) {
        const QString CS = handovers::helpers::getRBSFromCellId(cellA) == handovers::helpers::getRBSFromCellId(cellB) ? "YES" : "NO";
        return ericssonDefaultHandoverTemplate.arg(cellA, cellB, CS, BCCH_B, BCCH_A);
    }

}

QString Handovers2G2G::make(const QStringList &rows)
{

    if (!loadTemplates()) {
        return "Failed to load templates files.";
    }

    uniqueExtCells.clear();

    QMap<QString, EricssonHandovers> ericssonHandovers;

    QString huaweiDefaultHandovers = "<p style=\"color:red;font-size:18px\">HUAWEI DEFAULT HANDOVERS\n" + QString(20, '=') + "</p>\n";
    QString huaweiExternalHandovers = "<p style=\"color:red;font-size:18px\">HUAWEI EXTERNAL HANDOVERS\n" + QString(20, '=') + "</p>\n";
    QString huaweiExternalCells =  "<p style=\"color:red;font-size:18px\">HUAWEI EXTERNAL CELLS\n" + QString(20, '=') + "</p>\n";

    QString errors = "ERRORS WHILE EXECUTING\n" + QString(20, '=') + '\n';

    for (int i = 1; i < rows.size(); ++i) {
        QString row = rows[i];
        QStringList elements = row.split(csv_delimeter, Qt::SkipEmptyParts);
        if (!handovers::helpers::isRowFits(elements, columnRoles().size() * 2)) {
            errors += "Not enought arguments to make HO;\nRow :" + row + "\n";
            continue;
        }


        const QMap colRoles = columnRoles();

        // removes '_' and replace '1' by 'A', etc.
        handovers::helpers::transformCell(elements[colRoles[ColumnRole::Cell]]);
        handovers::helpers::transformCell(elements[colRoles[ColumnRole::Cell] + neighbourShift()]);



        const QString &controllerA = elements.at(colRoles[ColumnRole::Controller]);
        const QString &controllerB = elements.at(colRoles[ColumnRole::Controller] + neighbourShift());

        const QString &cellA = elements.at(colRoles[ColumnRole::Cell]);
        const QString &cellB = elements.at(colRoles[ColumnRole::Cell] + neighbourShift());
        const QString &BCCH_A = elements.at(colRoles[ColumnRole::BCCH]);
        const QString &BCCH_B = elements.at(colRoles[ColumnRole::BCCH] + neighbourShift());
        const QString &BSIC_A = elements.at(colRoles[ColumnRole::BSIC]);
        const QString &BSIC_B = elements.at(colRoles[ColumnRole::BSIC] + neighbourShift());
        const QString &LAC_A = elements.at(colRoles[ColumnRole::LAC]);
        const QString &LAC_B = elements.at(colRoles[ColumnRole::LAC] + neighbourShift());

        if (controllerA == controllerB) {
            if(controllerA != "BSC06") {
                ericssonHandovers[controllerA].defaultHandovers += ericssonDefaultHandover(cellA,
                                                                                           cellB,
                                                                                           BCCH_A,
                                                                                           BCCH_B);
            } else if (controllerA == "BSC06") {
                huaweiDefaultHandovers += huaweiHandover(cellA, cellB);
            } else {
                errors += "Неизвестный контроллер\n" + row + '\n';
            }
        } else {
            if (controllerA == "BSC06") {
                huaweiExternalCells += huaweiExternalCell(controllerA, cellB, LAC_B, BCCH_B, BSIC_B);
                huaweiExternalHandovers += huaweiHandover(cellA, cellB);
                ericssonHandovers[controllerB].extCells += ericssonExternalCell(controllerB, cellA, LAC_A, BSIC_A, BCCH_A);
                ericssonHandovers[controllerB].extHandovers += ericssonExternalHandover(cellB, cellA, BCCH_A);
            } else if (controllerB == "BSC06") {
                huaweiExternalCells += huaweiExternalCell(controllerA, cellA, LAC_A, BCCH_A, BSIC_A);
                huaweiExternalHandovers += huaweiHandover(cellB, cellA);
                ericssonHandovers[controllerA].extCells += ericssonExternalCell(controllerA, cellB, LAC_B, BSIC_B, BCCH_B);
                ericssonHandovers[controllerA].extHandovers += ericssonExternalHandover(cellA, cellB, BCCH_B);
            } else {
                ericssonHandovers[controllerA].extCells += ericssonExternalCell(controllerA, cellB, LAC_B, BSIC_B, BCCH_B);
                ericssonHandovers[controllerA].extHandovers += ericssonExternalHandover(cellA, cellB, BCCH_B);

                ericssonHandovers[controllerB].extCells += ericssonExternalCell(controllerB, cellA, LAC_A, BSIC_A, BCCH_A);
                ericssonHandovers[controllerB].extHandovers += ericssonExternalHandover(cellB, cellA, BCCH_A);
            }
        }

    }

    QStringList colors {"red", "blue", "green", "yellow"};
    uint colorCount = 0;
    QString ericsson;
    for (auto it = ericssonHandovers.begin(); it != ericssonHandovers.end();++it) {
        ericsson += "<p style=\"color:" + colors.at(colorCount++) + ";font-size:25px\">" + it.key() + "</p>\n" + it->defaultHandovers + it->extCells + it->extHandovers;
    }

    return ericsson +
           huaweiDefaultHandovers +
           huaweiExternalCells +
           huaweiExternalHandovers +
           errors;
}

QMap<BaseHandovers::ColumnRole, std::size_t> Handovers2G2G::columnRoles() const
{
    return {
        {ColumnRole::Cell, 0},
        {ColumnRole::BCCH, 1},
        {ColumnRole::BSIC, 2},
        {ColumnRole::LAC, 4},
        {ColumnRole::Controller, 3}
    };
}

size_t Handovers2G2G::neighbourShift() const
{
    return columnRoles().size();
}

bool Handovers2G2G::loadTemplates() const
{
    huaweiHandoverTemplate = handovers::helpers::loadTemplate("templates/2g2g/2.txt");
    huaweiExtCellTemplate = handovers::helpers::loadTemplate("templates/2g2g/1.txt");
    ericssonDefaultHandoverTemplate = handovers::helpers::loadTemplate("templates/2g2g/5.txt");
    ericssonExtCellTemplate = handovers::helpers::loadTemplate("templates/2g2g/3.txt");
    ericssonExtHandoverTemplate = handovers::helpers::loadTemplate("templates/2g2g/4.txt");

    return !huaweiHandoverTemplate.isEmpty() &&
           !huaweiExtCellTemplate.isEmpty() &&
           !ericssonDefaultHandoverTemplate.isEmpty() &&
           !ericssonExtCellTemplate.isEmpty() &&
           !ericssonExtHandoverTemplate.isEmpty();
}
