#include "handovers2g2g.h"

namespace {
    QString getCellId(const QString &cellname) {
        QString result;
        for (auto &c : cellname) {
            if(c.isDigit()) {
                result += c;
            }
        }
        return result;
    }

    QString NCC(const QString &BSIC) {
        return BSIC.length() == 1 ? "0" : BSIC.left(1);
    }

    QString BCC(const QString &BSIC) {
        return BSIC.right(1);
    }

    QString huaweiExternalCell(const QString &cellname,
                               const QString &LAC,
                               const QString &BCCH,
                               const QString &BSIC) {
        static QString t =
        "ADD GEXT2GCELL:EXT2GCELLNAME=\"%1\",MCC=\"255\",MNC=\"99\",LAC=%2,CI=%3,BCCH=%4,NCC=%5,BCC=%6,RA=1;\n";
        return t.arg(cellname, LAC, getCellId(cellname), BCCH, NCC(BSIC), BCC(BSIC));
    }

    QString huaweiHandover(const QString &cellA, const QString &cellB) {
        static QString t =
        "ADD G2GNCELL:IDTYPE=BYNAME,SRC2GNCELLNAME=\"%1\",NBR2GNCELLNAME=\"%2\",NCELLTYPE=HANDOVERNCELL,SRCHOCTRLSWITCH=HOALGORITHM1;\n";
        return t.arg(cellA, cellB);
    }

    QString ericssonExternalCell(const QString &cell, const QString &LAC,
                                 const QString &BSIC,
                                 const QString &BCCH) {
        static QString t =
                "RLDEI:CELL=%1,CSYSTYPE=%6,EXT;\n"
                "RLDEC:CELL=%1,CGI=255-99-%2-%3,BSIC=%4 ,BCCHNO=%5;\n"
                "RLLOC:CELL=%1,BSPWR=%7,BSRXMIN=83,BSRXSUFF=73,MSRXMIN=83,MSRXSUFF=47,BSTXPWR=%7;\n\n";
        const QString gsmStandart = BCCH.toInt() >= 512 && BCCH.toInt() <= 885 ? "GSM1800" : "GSM900";
        const QString power = gsmStandart == "GSM900" ? "47" : "45";
        return t.arg(cell, LAC, ::getCellId(cell), BSIC, BCCH, gsmStandart, power);
    }
    QString ericssonExternalHandover(const QString &cellA, const QString &cellB,
                                     const QString &BCCH_B) {
        static QString t =
                "RLNRI:CELL=%1,CELLR=%2,SINGLE;\n"
                "RLNRC:CELL=%1,CELLR=%2,CAND=BOTH,CS=NO,KHYST=6,KOFFSETP=0 ,LHYST=8,LOFFSETP=0,TRHYST=4,TROFFSETP=0,AWOFFSET= 10 ,BQOFFSET= 10 ;\n"
                "RLMFC:CELL=%1,MBCCHNO=%3,MRNIC;\n\n";
        return t.arg(cellA, cellB, BCCH_B);
    }

    QString ericssonDefaultHandover(const QString &cellA, const QString &cellB,
                                    const QString &BCCH_A, const QString &BCCH_B) {
        static QString t = "RLNRI:CELL=%1,%2;\n"
                           "RLNRC:CELL=%1,CELLR= %2,CAND=BOTH,CS=%3,KHYST=6,KOFFSETP=0 ,LHYST=8,LOFFSETP=0,TRHYST=4,TROFFSETP=0,AWOFFSET= 10 ,BQOFFSET= 10;\n"
                           "RLMfC:CELL=%1,MBCCHNO=%4,MRNIC;\n"
                           "RLMFC:CELL=%2,MBCCHNO=%5,MRNIC;\n\n";
        const QString CS = handovers::helpers::getRBSFromCellId(cellA) == handovers::helpers::getRBSFromCellId(cellB) ? "YES" : "NO";
        return t.arg(cellA, cellB, CS, BCCH_B, BCCH_A);
    }
}

QString Handovers2G2G::make(const QStringList &rows) noexcept
{
    QString ericssonDefaultHandoverBSC03 = "ERICSSON DEFAULT HANDOVERS(BSC03)\n" + QString(20, '=') + '\n';
    QString ericssonExternalCellsBSC03 =  "ERICSSON EXTERNAL CELLS(TO BSC03)\n" + QString(20, '=') + '\n';
    QString ericssonExternalHandoversBSC03 =  "ERICSSON EXTERNAL HANDOVERS(TO BSC03)\n" + QString(20, '=') + '\n';

    QString ericssonDefaultHandoverBSC04 = "ERICSSON DEFAULT HANDOVERS(BSC04)\n" + QString(20, '=') + '\n';
    QString ericssonExternalCellsBSC04 =  "ERICSSON EXTERNAL CELLS(TO BSC04)\n" + QString(20, '=') + '\n';
    QString ericssonExternalHandoversBSC04 =  "ERICSSON EXTERNAL HANDOVERS(TO BSC04)\n" + QString(20, '=') + '\n';

    QString ericssonDefaultHandoverBSC05 = "ERICSSON DEFAULT HANDOVERS(BSC05)\n" + QString(20, '=') + '\n';
    QString ericssonExternalCellsBSC05 =  "ERICSSON EXTERNAL CELLS(TO BSC05)\n" + QString(20, '=') + '\n';
    QString ericssonExternalHandoversBSC05 = "ERICSSON EXTERNAL HANDOVERS(TO BSC05)\n" + QString(20, '=') + '\n';

    QString huaweiDefaultHandovers = "HUAWEI DEFAULT HANDOVERS\n" + QString(20, '=') + '\n';
    QString huaweiExternalHandovers = "HUAWEI EXTERNAL HANDOVERS\n" + QString(20, '=') + '\n';
    QString huaweiExternalCells =  "HUAWEI EXTERNAL CELLS\n" + QString(20, '=') + '\n';

    QString errors = "ERRORS WHILE EXECUTING\n" + QString(20, '=') + '\n';

    QVector<QString> extCellsBSC04;
    QVector<QString> extCellsBSC05;
    QVector<QString> extCellsBSC06;


    for (int i = 1; i < rows.size(); ++i) {
        const QString& row = rows.at(i);
        const QStringList elements = row.split(csv_delimeter);

        const QMap colRoles = columnRoles();

        const QString &controllerA = elements.at(colRoles[ColumnRole::Controller]);
        const QString &controllerB = elements.at(colRoles[ColumnRole::Controller] + neighbourShift());

        if (controllerA == controllerB) {
            if (controllerA == "BSC04") {
            // Ericsson default handovers
                ericssonDefaultHandoverBSC04 += ericssonDefaultHandover(elements.at(colRoles[ColumnRole::Cell]),
                                                  elements.at(colRoles[ColumnRole::Cell] + neighbourShift()),
                                                  elements.at(colRoles[ColumnRole::BCCH]),
                                                  elements.at(colRoles[ColumnRole::BCCH]) + neighbourShift());
            } else if (controllerA == "BSC03") {
                ericssonDefaultHandoverBSC03 += ericssonDefaultHandover(elements.at(colRoles[ColumnRole::Cell]),
                                                  elements.at(colRoles[ColumnRole::Cell] + neighbourShift()),
                                                  elements.at(colRoles[ColumnRole::BCCH]),
                                                  elements.at(colRoles[ColumnRole::BCCH]) + neighbourShift());
            } else if (controllerA == "BSC05") {
                ericssonDefaultHandoverBSC05 += ericssonDefaultHandover(elements.at(colRoles[ColumnRole::Cell]),
                                                  elements.at(colRoles[ColumnRole::Cell] + neighbourShift()),
                                                  elements.at(colRoles[ColumnRole::BCCH]),
                                                  elements.at(colRoles[ColumnRole::BCCH]) + neighbourShift());
            }
        } else {
            // controller are not same
            if (controllerA == "BSC06" && controllerB == "BSC04") {

            } else if (controllerA == "BSC04" && controllerB == "BSC06") {

            } else if (controllerA == "BSC06" && controllerB == "BSC05") {

            } else if (controllerA == "BSC05" && controllerB == "BSC06") {

            } else if (controllerA == "BSC05" && controllerB == "BSC04") {

            } else if (controllerA == "BSC04" && controllerB == "BSC05") {

            } else {

            }
        }
    }

    return ericssonDefaultHandoverBSC03 +
           ericssonExternalCellsBSC03 +
           ericssonExternalHandoversBSC03 +
           ericssonDefaultHandoverBSC04 +
           ericssonExternalCellsBSC04 +
           ericssonExternalHandoversBSC04 +
           ericssonDefaultHandoverBSC05 +
           ericssonExternalCellsBSC05 +
           ericssonExternalHandoversBSC05 +
           huaweiDefaultHandovers +
           huaweiExternalCells +
           huaweiExternalHandovers +
           errors;
}

QMap<BaseHandovers::ColumnRole, std::size_t> Handovers2G2G::columnRoles() const noexcept
{
    return {
        {ColumnRole::Cell, 0},
        {ColumnRole::BCCH, 1},
        {ColumnRole::BSIC, 2},
        {ColumnRole::LAC, 3},
        {ColumnRole::Controller, 4}
    };
}

size_t Handovers2G2G::neighbourShift() const noexcept
{
    return columnRoles().size();
}


