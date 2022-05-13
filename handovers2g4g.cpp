#include "handovers2g4g.h"

namespace {
    QMap<QString, QStringList> uniqueExtCells;
    QString ericssonGSMLTETemplate;
    QString huaweiGSMLTEExternalCellTemplate;
    QString huaweiGSMLTEExternalHOTemplate;

    QString huaweiLTETemplateConstText;
    QString huaweiLTETemplateFrequency;
    QString huaweiLTETemplateExtCell;
    QString huaweiLTETemplateGSMHandover;

    QString ericssonLTETemplateConstText;
    QString ericssonLTETemplateCreateFrequency;
    QString ericssonLTETemplateCreateExtCell;
    QString ericssonLTETemplateMasterGeranCellId;
    QString ericssonLTETemplateGSMHandover;

    QVector<QString> uniqueFrequency;
    QVector<QString> uniqueExtCellsForLTE;

    QString HuaweiLTEGSMHandover(const QString &cellLTE,
                                 const QString &LAC_GSM,
                                 const QString &cellGSM)
    {
        return huaweiLTETemplateGSMHandover.arg(handovers::helpers::lteLocalCellId(cellLTE),
                                       LAC_GSM, handovers::helpers::getCellId(cellGSM));
    }

    QString HuaweiLTEExtCell(const QString &gsmCellName,
                             const QString &gsm_LAC,
                             const QString &gsm_BCCH,
                             const QString &gsm_BSIC)
    {
        if (uniqueExtCellsForLTE.contains(gsmCellName)) {
            return "";
        } else {
            uniqueExtCellsForLTE.push_back(gsmCellName);
            return huaweiLTETemplateExtCell.arg(handovers::helpers::getCellId(gsmCellName), gsm_LAC,
                                               gsm_BCCH, handovers::helpers::NCC(gsm_BSIC),
                                                         handovers::helpers::BCC(gsm_BSIC), gsmCellName);
        }
    }

    QString HuaweiLTEFrequency(const QString &LTECellname,
                               const QString &BCCH_GSM)
    {
        if (uniqueFrequency.contains(BCCH_GSM)) {
            return "";
        } else {
            uniqueFrequency.push_back(BCCH_GSM);
            return huaweiLTETemplateFrequency.arg(handovers::helpers::lteLocalCellId(LTECellname),
                                                  BCCH_GSM);
        }
    }

    QString EricssonLTEcreateGSMHandover(const QString &LTEcellname,
                                         const QString &GSMcellname)
    {
        return ericssonLTETemplateGSMHandover.arg(handovers::helpers::lteLocalCellId(LTEcellname),
                                                  handovers::helpers::getCellId(GSMcellname));
    }

    QString EricssonLTEcreateExternalCell(const QString &cellGSM, const QString &LAC,
                               const QString &BSIC, const QString &BCCH)
    {
        if (uniqueExtCellsForLTE.contains(cellGSM)) {
            return "";
        } else {
            uniqueExtCellsForLTE.push_back(cellGSM);
            return ericssonLTETemplateCreateExtCell.arg(handovers::helpers::getCellId(cellGSM), LAC,
                                                        handovers::helpers::NCC(BSIC), handovers::helpers::BCC(BSIC), BCCH) +
                   ericssonLTETemplateMasterGeranCellId.arg(handovers::helpers::getCellId(cellGSM));
        }
    }

    QString EricssonLTECreateFrequency(const QString &BCCH)
    {
        if (uniqueFrequency.contains(BCCH)) {
            return "";
        } else {
            uniqueFrequency.push_back(BCCH);
            return ericssonLTETemplateCreateFrequency.arg(BCCH);
        }
    }

    QString makeGSMLTEHandover(const QString &gsmController, const QString &gsmCellName)
    {
        if (handovers::helpers::isNotUnique(uniqueExtCells, gsmController, gsmCellName)) {
            return "";
        }
        return ericssonGSMLTETemplate.arg(gsmCellName);
    }

    QString makeHuaweiGSMLTEExtCell(const QString &controller,
                                    const QString &cellnameLTE,
                                    const QString &PCI)
    {
        if (handovers::helpers::isNotUnique(uniqueExtCells, controller, cellnameLTE)) {
            return "";
        }

        QString localCellId = handovers::helpers::lteLocalCellId(cellnameLTE);
        QString lteRBS = handovers::helpers::lteRBSName(cellnameLTE);

        return huaweiGSMLTEExternalCellTemplate.arg(cellnameLTE,
               handovers::helpers::makeGeranCellId(lteRBS, localCellId), PCI);
    }

    QString makeHuaweiGSMLTEHandover(const QString &gsmCell,
                                     const QString &lteCell)
    {
        return huaweiGSMLTEExternalHOTemplate.arg(gsmCell, lteCell);
    }
}

QString Handovers2G4G::make(const QStringList &rows)
{
    if (!loadTemplates()) {
        return "Failed to load templates files.";
    }

    if (vendorLTE() == VendorLTE::Invalid) {
        return "Vendor LTE is undefined.";
    } else if (!loadTemplatesLTE()) {
        return "Failed to load LTE templates files.";
    }

    uniqueExtCells.clear();
    uniqueFrequency.clear();
    uniqueExtCellsForLTE.clear();

    QMap<QString, EricssonHandovers> handovers2G;
    QString huaweiExternalHandovers = "<p style=\"color:red;font-size:18px\">HUAWEI EXTERNAL HANDOVERS\n" + QString(20, '=') + "</p>\n";
    QString huaweiExternalCells =  "<p style=\"color:red;font-size:18px\">HUAWEI EXTERNAL CELLS\n" + QString(20, '=') + "</p>\n";

    QString errors = "ERRORS WHILE EXECUTING\n" + QString(20, '=') + '\n';
    QString lteHandovers = "<p style=\"color:red;font-size:18px\">LTE HANDOVERS\n" + QString(20, '=') + "</p>\n";

    QMap<ColumnRole, std::size_t> colRoles = columnRoles();

    if (vendorLTE() == VendorLTE::Ericsson) {
        lteHandovers += ericssonLTETemplateConstText;
    } else {
        lteHandovers += huaweiLTETemplateConstText;
    }

    for (int i = 1; i < rows.size(); ++i) {

        QStringList elements = rows[i].split(csv_delimeter, Qt::SkipEmptyParts);

        if (!handovers::helpers::isRowFits(elements, neighbourShift() * 2)) {
            errors += "Not enought arguments to make HO;\nRow :" + rows[i] + "\n";
            continue;
        }

        handovers::helpers::transformCell(elements[colRoles[ColumnRole::Cell]]);

        const QString &controllerGSM = elements[colRoles[ColumnRole::Controller]];
        const QString &cellGSM = elements[colRoles[ColumnRole::Cell]];
        const QString &BCCH_GSM = elements[colRoles[ColumnRole::BCCH]];
        const QString &BSIC_GSM = elements[colRoles[ColumnRole::BSIC]];
        const QString &LAC_GSM = elements[colRoles[ColumnRole::LAC]];
        const QString &cellLTE = elements[colRoles[ColumnRole::Cell] + neighbourShift()];
        const QString &PCI_LTE = elements[colRoles[ColumnRole::PCI]];

        if (controllerGSM == "BSC06") {
            huaweiExternalCells += makeHuaweiGSMLTEExtCell(controllerGSM, cellLTE, PCI_LTE);
            huaweiExternalHandovers += makeHuaweiGSMLTEHandover(cellGSM, cellLTE);
        } else {
            handovers2G[controllerGSM].extHandovers += makeGSMLTEHandover(controllerGSM, cellGSM);
        }

        if (vendorLTE() == VendorLTE::Ericsson) {
            lteHandovers += EricssonLTECreateFrequency(BCCH_GSM);
            lteHandovers += EricssonLTEcreateExternalCell(cellGSM, LAC_GSM, BSIC_GSM, BCCH_GSM);
            lteHandovers += EricssonLTEcreateGSMHandover(cellLTE, cellGSM);
        } else {
            lteHandovers += HuaweiLTEFrequency(cellLTE, BCCH_GSM);
            lteHandovers += HuaweiLTEExtCell(cellGSM, LAC_GSM, BCCH_GSM, BSIC_GSM);
            lteHandovers += HuaweiLTEGSMHandover(cellLTE, LAC_GSM, cellGSM);
        }
    }

    QStringList colors {"red", "blue", "green", "yellow"};
    uint colorCount = 0;
    QString ericsson;
    for (auto it = handovers2G.begin(); it != handovers2G.end();++it) {
        ericsson += "<p style=\"color:" + colors.at(colorCount++) + ";font-size:25px\">" + it.key() + "</p>\n"  + it->extHandovers;
    }

    return ericsson + huaweiExternalCells + huaweiExternalHandovers +
            lteHandovers + errors;
}

bool Handovers2G4G::loadTemplates() const
{
    ericssonGSMLTETemplate = handovers::helpers::loadTemplate("templates/2g4g/1.txt");
    huaweiGSMLTEExternalCellTemplate = handovers::helpers::loadTemplate("templates/2g4g/2.txt");
    huaweiGSMLTEExternalHOTemplate = handovers::helpers::loadTemplate("templates/2g4g/3.txt");


    return !ericssonGSMLTETemplate.isEmpty() &&
           !huaweiGSMLTEExternalCellTemplate.isEmpty() &&
           !huaweiGSMLTEExternalHOTemplate.isEmpty();
}

QMap<BaseHandovers::ColumnRole, std::size_t> Handovers2G4G::columnRoles() const
{
    return {
        {ColumnRole::Cell, 0},
        {ColumnRole::BCCH, 1},
        {ColumnRole::BSIC, 2},
        {ColumnRole::LAC, 3},
        {ColumnRole::Controller, 4},
        {ColumnRole::PCI, 7}
    };
}

size_t Handovers2G4G::neighbourShift() const
{
    return 5;
}

bool Handovers2G4G::loadEricssonTemplatesLTE()
{
    ericssonLTETemplateConstText = handovers::helpers::loadTemplate("templates/2g4g/4g ericsson/1.txt");
    ericssonLTETemplateCreateFrequency = handovers::helpers::loadTemplate("templates/2g4g/4g ericsson/2.txt");
    ericssonLTETemplateCreateExtCell = handovers::helpers::loadTemplate("templates/2g4g/4g ericsson/3.txt");
    ericssonLTETemplateMasterGeranCellId = handovers::helpers::loadTemplate("templates/2g4g/4g ericsson/4.txt");
    ericssonLTETemplateGSMHandover = handovers::helpers::loadTemplate("templates/2g4g/4g ericsson/5.txt");

    return !ericssonLTETemplateConstText.isEmpty() &&
           !ericssonLTETemplateCreateFrequency.isEmpty() &&
           !ericssonLTETemplateCreateExtCell.isEmpty() &&
           !ericssonLTETemplateMasterGeranCellId.isEmpty() &&
           !ericssonLTETemplateGSMHandover.isEmpty();
}

bool Handovers2G4G::loadHuaweiTemplatesLTE()
{
    huaweiLTETemplateConstText = handovers::helpers::loadTemplate("templates/2g4g/4g huawei/1.txt");
    huaweiLTETemplateFrequency = handovers::helpers::loadTemplate("templates/2g4g/4g huawei/2.txt");
    huaweiLTETemplateExtCell = handovers::helpers::loadTemplate("templates/2g4g/4g huawei/3.txt");
    huaweiLTETemplateGSMHandover = handovers::helpers::loadTemplate("templates/2g4g/4g huawei/4.txt");

    return !huaweiLTETemplateGSMHandover.isEmpty() &&
           !huaweiLTETemplateConstText.isEmpty() &&
           !huaweiLTETemplateExtCell.isEmpty() &&
           !huaweiLTETemplateFrequency.isEmpty();
}

bool Handovers2G4G::loadTemplatesLTE()
{
    return m_VendorLTE == VendorLTE::Ericsson ? loadEricssonTemplatesLTE() : loadHuaweiTemplatesLTE();
}
