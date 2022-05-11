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
    QString result;

    for (int i = 1; i < rows.size(); ++i) {
        const QString& row = rows.at(i);
        const QStringList elements = row.split(csv_delimeter);

        const QMap colRoles = columnRoles();

        const QString &controllerA = elements.at(colRoles[ColumnRole::Controller]);
        const QString &controllerB = elements.at(colRoles[ColumnRole::Controller] + neighbourShift());

        if (controllerA == controllerB && controllerA != "BSC06") {
            // Ericsson default handovers
            result += ericssonDefaultHandover(elements.at(colRoles[ColumnRole::Cell]),
                                              elements.at(colRoles[ColumnRole::Cell] + neighbourShift()),
                                              elements.at(colRoles[ColumnRole::BCCH]),
                                              elements.at(colRoles[ColumnRole::BCCH]) + neighbourShift());
        } else {
            // controller are not same


        }
    }

    return result;
}

QMap<BaseHandovers::ColumnRole, std::size_t> Handovers2G2G::columnRoles() const
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


