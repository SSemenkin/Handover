#include "handovers2g3g.h"
#include "mainwindow.h"

namespace {
    QMap<QString, QStringList> uniqueExtCells;
    QString ericssonEXTHandoverTemplate;
    QString huaweiExtCellTemplate;
    QString huaweiExtHandoverTemplate;
    QString rncExtCellTemplate;
    QString rncExtHandoverTemplate;
    QString ericssonEXTCellTemplate;
    QMap<QString, QStringList> uniqueExtCellsForBSC;

    QString huaweiExtCell(const QString &toController,
                          const QString &cellname,
                          const QString &LAC,
                          const QString &cellid,
                          const QString &frequency,
                          const QString &scrCode)
    {
        if(handovers::helpers::isNotUnique(uniqueExtCells, toController, cellname)) {
            return "";
        }
        return huaweiExtCellTemplate.arg(cellname, LAC, cellid, frequency, scrCode);
    }

    QString huaweiExtHandover(const QString &huaCell,
                              const QString &rncCell)
    {
        return huaweiExtHandoverTemplate.arg(huaCell, rncCell);
    }

    QString ericssonExt3G(const QString &cellid,
                          const QString &frequency, const QString &scrCode, const QString &LAC,
                          const QString &controller) {
        if(handovers::helpers::isNotUnique(uniqueExtCellsForBSC, controller, cellid)) {
            return "";
        }
        return ericssonEXTCellTemplate.arg(cellid, frequency, scrCode, LAC, cellid);
    }

    QString ericssonExt3GHO(const QString &ericssonCellName,
                          const QString &freq,
                          const QString &scrCode,
                          const QString &cellID3G)
    {
        return ericssonEXTHandoverTemplate.arg(ericssonCellName, freq, scrCode, cellID3G);
    }


    QString make2GExt(const QString &gsmCellId, const QString &cellname,
                      const QString &LAC, const QString &BSIC, const QString &BCCH)
    {
        static QString toController = "RNC";
        if(handovers::helpers::isNotUnique(uniqueExtCells, toController, cellname)) {
            return "";
        }
        return rncExtCellTemplate.arg(gsmCellId, cellname, LAC, gsmCellId, handovers::helpers::NCC(BSIC),
                                      handovers::helpers::BCC(BSIC), BCCH);
    }

    QString make3G2GHandover(const QString &umtsCellId,
                             const QString &gsmCellId)
    {
        return rncExtHandoverTemplate.arg(umtsCellId, gsmCellId);
    }
}

QString Handovers2G3G::make(const QStringList &rows)
{
    if (!loadTemplates()) {
        return "Failed to load templates files.";
    }
    QString errors = "ERRORS WHILE EXECUTING\n" + QString(20, '=') + '\n';

    uniqueExtCells.clear();
    uniqueExtCellsForBSC.clear();

    const QMap<QString, QString> cellIDToLACRNC = MainWindow::cellIdToLAC();
    QMap<QString, EricssonHandovers> ericssonHandovers2G;
    QString huaweiExternalHandovers = "<p style=\"color:red;font-size:18px\">HUAWEI EXTERNAL HANDOVERS\n" + QString(20, '=') + "</p>\n";
    QString huaweiExternalCells =  "<p style=\"color:red;font-size:18px\">HUAWEI EXTERNAL CELLS\n" + QString(20, '=') + "</p>\n";

    QString umtsExtCells = "<p style=\"color:red;font-size:18px\">UMTS EXTERNAL CELLS(RNC03)\n" + QString(20, '=') + "</p>\n";
    QString umtsExtHandovers = "<p style=\"color:red;font-size:18px\">UMTS EXTERNAL HANDOVERS(RNC03)\n" + QString(20, '=') + "</p>\n";

    QMap<ColumnRole, std::size_t> colRoles = columnRoles();

    for (int i = 1; i < rows.size(); ++i) {
        const QString& row = rows[i];
        QStringList elements = row.split(csv_delimeter, Qt::SkipEmptyParts);

        if (!handovers::helpers::isRowFits(elements, 9)) {
            errors += "Not enought arguments to make HO;\nRow :" + row + "\n";
            continue;
        }


        handovers::helpers::transformCell(elements[colRoles[ColumnRole::Cell]]);


        const QString &controller = elements.at(colRoles[ColumnRole::Controller]);
        const QString &cell2G = elements.at(colRoles[ColumnRole::Cell]);
        const QString &cell3G = elements.at(colRoles[ColumnRole::Cell] + neighbourShift());
        const QString &BCCH_2G = elements.at(colRoles[ColumnRole::BCCH]);
        const QString &BSIC_2G = elements.at(colRoles[ColumnRole::BSIC]);
        const QString &LAC_2G = elements.at(colRoles[ColumnRole::LAC]);
        const QString &cellId3G = elements.at(colRoles[ColumnRole::CellId]);
        const QString &DwnFreq = elements.at(colRoles[ColumnRole::DwnFrequency]);
        const QString &ScrCode = elements.at(colRoles[ColumnRole::ScrCode]);
        QString cellid2G = handovers::helpers::getCellId(cell2G);

        QString LAC_3G = cellIDToLACRNC[cellId3G];
        if (LAC_3G.isEmpty()) {
            errors += "Undefided LAC for " + cellId3G;
            continue;
        }

        if (controller == "BSC06") {
            huaweiExternalCells += huaweiExtCell(controller, cell3G, LAC_3G, cellId3G, DwnFreq, ScrCode);
            huaweiExternalHandovers += huaweiExtHandover(cell2G, cell3G);
        } else {
            ericssonHandovers2G[controller].extCells += ericssonExt3G(cellId3G, DwnFreq, ScrCode, LAC_3G, controller);
            ericssonHandovers2G[controller].extHandovers += ericssonExt3GHO(cell2G, DwnFreq, ScrCode, cellId3G);
        }
        umtsExtCells += make2GExt(cellid2G, cell2G, LAC_2G, BSIC_2G, BCCH_2G);
        umtsExtHandovers += make3G2GHandover(cellId3G, cellid2G);
    }

    QStringList colors {"red", "blue", "green", "yellow"};
    uint colorCount = 0;
    QString ericsson;
    for (auto it = ericssonHandovers2G.begin(); it != ericssonHandovers2G.end();++it) {
        ericsson += "<p style=\"color:" + colors.at(colorCount++) + ";font-size:25px\">" + it.key() + "</p>\n" + it->extHandovers;
        ericsson += "<p style=\"color:" + colors.at(colorCount++) + ";font-size:25px\">" + it.key() + "</p>\n" + it->extCells;
    }

    return ericsson + huaweiExternalCells + huaweiExternalHandovers + umtsExtCells + umtsExtHandovers + errors;
}

QMap<BaseHandovers::ColumnRole, std::size_t> Handovers2G3G::columnRoles() const
{
    return {
            {ColumnRole::Cell, 0},
            {ColumnRole::BCCH, 1},
            {ColumnRole::BSIC, 2},
            {ColumnRole::LAC, 3},
            {ColumnRole::Controller, 4},
            {ColumnRole::CellId, 8},
            {ColumnRole::DwnFrequency, 6},
            {ColumnRole::ScrCode, 7}
           };
}

size_t Handovers2G3G::neighbourShift() const
{
    return 5;
}

bool Handovers2G3G::loadTemplates() const
{
    ericssonEXTHandoverTemplate = handovers::helpers::loadTemplate("templates/2g3g/1.txt");
    huaweiExtCellTemplate = handovers::helpers::loadTemplate("templates/2g3g/2.txt");
    huaweiExtHandoverTemplate = handovers::helpers::loadTemplate("templates/2g3g/3.txt");
    rncExtCellTemplate = handovers::helpers::loadTemplate("templates/2g3g/4.txt");
    rncExtHandoverTemplate = handovers::helpers::loadTemplate("templates/2g3g/5.txt");
    ericssonEXTCellTemplate = handovers::helpers::loadTemplate("templates/2g3g/6.txt");

    return !ericssonEXTHandoverTemplate.isEmpty() &&
           !huaweiExtHandoverTemplate.isEmpty() &&
           !huaweiExtCellTemplate.isEmpty() &&
           !rncExtCellTemplate.isEmpty() &&
           !rncExtHandoverTemplate.isEmpty() &&
           !ericssonEXTCellTemplate.isEmpty();
}
