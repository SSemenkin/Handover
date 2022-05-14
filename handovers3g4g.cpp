#include "handovers3g4g.h"
#include "mainwindow.h"

namespace {

    QString umtsExtCellTemplate;
    QString umtsLTEHandoverTemplate;

    QString huaweiLTEFrequencyTemplate;
    QString huaweiLTEExtCellTemplate;
    QString huaweiLTEUTMSHandoverTemplate;

    QString ericssonLTEConstTextTemplate;
    QString ericssonLTEFrequencyTemplate;
    QString ericssonLTEExtCellTemplate;
    QString ericssonLTEReselectionTemplate;
    QString ericssonLTENeighbourTemplate;

    QVector<QString> uniqueLTENeighbours;
    QVector<QString> uniqueLTEFrequency;
    QVector<QString> uniqueLTEExtCells;
    QVector<QString> uniqueReselection;

    QString EricssonLTEUMTSHandover(const QString &LTE_Cell,
                                    const QString &UMTS_Freq,
                                    const QString &UMTS_CellId)
    {
        return ericssonLTENeighbourTemplate.arg(handovers::helpers::lteLocalCellId(LTE_Cell), UMTS_Freq, UMTS_CellId);
    }

    QString EricssonResection(const QString &LTE_Cell,
                              const QString &freq) {
        if (uniqueReselection.contains(LTE_Cell + freq)) {
            return "";
        } else {
            uniqueReselection.push_back(LTE_Cell + freq);
            return ericssonLTEReselectionTemplate.arg(handovers::helpers::lteLocalCellId(LTE_Cell),
                                                      freq);
        }
    }

    QString EricssonExtCell(const QString &UMTS_FREQ,
                            const QString &UMTS_cellId,
                            const QString &UMTS_LAC,
                            const QString &UMTS_SCR)
    {
        if (uniqueLTEExtCells.contains(UMTS_cellId)) {
            return "";
        } else {
            uniqueLTEExtCells.push_back(UMTS_cellId);
            return ericssonLTEExtCellTemplate.arg(UMTS_FREQ, UMTS_cellId, UMTS_LAC, UMTS_SCR);
        }
    }

    QString EricssonLTEFrequency(const QString &dnl_Frequency) {
        if (uniqueLTEFrequency.contains(dnl_Frequency)) {
            return "";
        } else {
            uniqueLTEFrequency.push_back(dnl_Frequency);
            return ericssonLTEFrequencyTemplate.arg(dnl_Frequency);
        }
    }

    QString HuaweiLTEUMTSHandover(const QString &LTE_Cell,
                                  const QString &UMTS_CellId)
    {
        return huaweiLTEUTMSHandoverTemplate.arg(handovers::helpers::lteLocalCellId(LTE_Cell),
                                                 UMTS_CellId);
    }

    QString HuaweiLTEExtCellAdd(const QString &cellid_UMTS,
                                const QString &frequency,
                                const QString &scr_Code,
                                const QString &LAC_UMTS)
    {
        if (uniqueLTEExtCells.contains(cellid_UMTS)) {
            return "";
        } else {
            uniqueLTEExtCells.push_back(cellid_UMTS);
            return huaweiLTEExtCellTemplate.arg(cellid_UMTS, frequency,
                                                handovers::helpers::umtsGetUplinkFrequency(frequency),
                                                scr_Code, LAC_UMTS);
        }
    }

    QString HuaweiLTEAddFrequencys(const QString &LTE_Cell,
                                   const QString &dnl_Frequency)
    {
        QString localCellId = handovers::helpers::lteLocalCellId(LTE_Cell);
        if (uniqueLTEFrequency.contains(localCellId + dnl_Frequency)) {
            return "";
        } else {
            uniqueLTEFrequency.push_back(localCellId + dnl_Frequency);
            return huaweiLTEFrequencyTemplate.arg(localCellId, dnl_Frequency,
                                                  handovers::helpers::umtsGetUplinkFrequency(dnl_Frequency));
        }
    }

    QString createLTEExternal(const QString &LTE_Cell,
                              const QString &PCI)
    {
        if (uniqueLTENeighbours.contains(LTE_Cell)) {
            return "";
        } else {
            uniqueLTENeighbours.push_back(LTE_Cell);
            return umtsExtCellTemplate.arg(handovers::helpers::lteCellIndex(LTE_Cell),
                    LTE_Cell, handovers::helpers::makeGeranCellId(handovers::helpers::lteRBSName(LTE_Cell),
                              handovers::helpers::lteLocalCellId(LTE_Cell)), PCI);
        }
    }

    QString createUMTSLTEHandover(const QString &UMTS_cellid,
                                  const QString &LTE_Cell)
    {
        return umtsLTEHandoverTemplate.arg(UMTS_cellid, handovers::helpers::lteCellIndex(LTE_Cell));
    }

}



QString Handovers3G4G::make(const QStringList &rows)
{
    if (!loadTemplates()) {
        return "Failed to load templates files.";
    } else if (!loadTemplatesLTE()) {
        return "Failed to load LTE templates files.";
    }
    uniqueLTENeighbours.clear();
    uniqueLTEFrequency.clear();
    uniqueLTEExtCells.clear();
    uniqueReselection.clear();

    QString errors = "ERRORS WHILE EXECUTING\n" + QString(20, '=') + '\n';
    QMap<QString, QString> cellIdToLAC = MainWindow::cellIdToLAC();

    QMap<ColumnRole, std::size_t> colRoles = columnRoles();

    QString umtsExtCells = "<p style=\"color:red;font-size:18px\">UMTS EXTERNAL CELLS(RNC03)\n" + QString(20, '=') + "</p>\n";
    QString umtsExtHandovers = "<p style=\"color:red;font-size:18px\">UMTS EXTERNAL HANDOVERS(RNC03)\n" + QString(20, '=') + "</p>\n";
    QString lteHandovers = "<p style=\"color:red;font-size:18px\">LTE HANDOVERS\n" + QString(20, '=') + "</p>\n";

    if (vendorLTE() == VendorLTE::Ericsson) {
        lteHandovers += ericssonLTEConstTextTemplate;
    }

    for (int i = 1; i < rows.size(); ++i) {
        QStringList elements = rows[i].split(csv_delimeter, Qt::SkipEmptyParts);

        if (!handovers::helpers::isRowFits(elements, neighbourShift() + 5)) {
            errors += "Not enought arguments to make HO;\nRow :" + rows[i] + "\n";
            continue;
        }

        const QString &cellIdUMTS = elements[colRoles[ColumnRole::CellId]];
        const QString &cell_UMTS = elements[colRoles[ColumnRole::Cell]];
        const QString &freq_UMTS = elements[colRoles[ColumnRole::DwnFrequency]];
        const QString &SCR_UMTS = elements[colRoles[ColumnRole::ScrCode]];
        const QString &cell_LTE = elements[colRoles[ColumnRole::Cell] + neighbourShift()];
        const QString &LTE_PCI = elements[colRoles[ColumnRole::PCI]];

        QString LAC_UMTS = cellIdToLAC[cellIdUMTS];

        if (LAC_UMTS.isEmpty()) {
            errors += "Undefided LAC for " + cellIdUMTS;
            continue;
        }

        umtsExtCells += createLTEExternal(cell_LTE, LTE_PCI);
        umtsExtHandovers += createUMTSLTEHandover(cellIdUMTS, cell_LTE);

        if (vendorLTE() == VendorLTE::Huawei) {
            lteHandovers += HuaweiLTEAddFrequencys(cell_LTE, freq_UMTS);
            lteHandovers += HuaweiLTEExtCellAdd(cellIdUMTS, freq_UMTS, SCR_UMTS, LAC_UMTS);
            lteHandovers += HuaweiLTEUMTSHandover(cell_LTE, cell_UMTS);
        } else {
            lteHandovers += EricssonLTEFrequency(freq_UMTS);
            lteHandovers += EricssonExtCell(freq_UMTS, cellIdUMTS, LAC_UMTS, SCR_UMTS);
            lteHandovers += EricssonResection(cell_LTE, freq_UMTS);
            lteHandovers += EricssonLTEUMTSHandover(cell_LTE, freq_UMTS, cellIdUMTS);
        }
    }

    return umtsExtCells + umtsExtHandovers + lteHandovers + errors;
}

bool Handovers3G4G::loadTemplates() const
{
    umtsExtCellTemplate = handovers::helpers::loadTemplate("templates/3g4g/1.txt");
    umtsLTEHandoverTemplate = handovers::helpers::loadTemplate("templates/3g4g/2.txt");

    return !umtsLTEHandoverTemplate.isEmpty() &&
           !umtsExtCellTemplate.isEmpty();
}

QMap<BaseHandovers::ColumnRole, std::size_t> Handovers3G4G::columnRoles() const
{
    return {
        {ColumnRole::Cell, 0},
        {ColumnRole::CellId, 1},
        {ColumnRole::DwnFrequency, 2},
        {ColumnRole::ScrCode, 3},
        {ColumnRole::PCI, 6}
    };
}

size_t Handovers3G4G::neighbourShift() const
{
    return 4;
}

bool Handovers3G4G::loadEricssonTemplatesLTE() const
{
    ericssonLTEConstTextTemplate = handovers::helpers::loadTemplate("templates/3g4g/4g ericsson/1.txt");
    ericssonLTEFrequencyTemplate = handovers::helpers::loadTemplate("templates/3g4g/4g ericsson/2.txt");
    ericssonLTEExtCellTemplate   = handovers::helpers::loadTemplate("templates/3g4g/4g ericsson/3.txt");
    ericssonLTENeighbourTemplate = handovers::helpers::loadTemplate("templates/3g4g/4g ericsson/5.txt");
    ericssonLTEReselectionTemplate = handovers::helpers::loadTemplate("templates/3g4g/4g ericsson/4.txt");

    return !ericssonLTEReselectionTemplate.isEmpty() &&
           !ericssonLTEConstTextTemplate.isEmpty() &&
           !ericssonLTEExtCellTemplate.isEmpty() &&
           !ericssonLTEFrequencyTemplate.isEmpty() &&
           !ericssonLTENeighbourTemplate.isEmpty();
}

bool Handovers3G4G::loadHuaweiTemplatesLTE() const
{
    huaweiLTEFrequencyTemplate = handovers::helpers::loadTemplate("templates/3g4g/4g huawei/1.txt");
    huaweiLTEExtCellTemplate = handovers::helpers::loadTemplate("templates/3g4g/4g huawei/2.txt");
    huaweiLTEUTMSHandoverTemplate = handovers::helpers::loadTemplate("templates/3g4g/4g huawei/3.txt");

    return !huaweiLTEUTMSHandoverTemplate.isEmpty() &&
           !huaweiLTEExtCellTemplate.isEmpty() &&
           !huaweiLTEFrequencyTemplate.isEmpty();
}
