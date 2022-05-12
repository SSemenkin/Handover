#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QHash>
#include <QMap>
#include <QVector>
#include <QDebug>

#define csv_delimeter ';'

enum class HandoverType{
    INVALID = -1,
    GSM_TO_GSM,
    GSM_TO_UMTS,
    GSM_TO_LTE,
    UMTS_TO_GSM,
    UMTS_TO_UMTS,
    UMTS_TO_LTE,
    LTE_TO_GSM,
    LTE_TO_UMTS
};

enum class VendorLTE {
    Invalid = -1,
    Huawei,
    Ericsson
};

inline QDebug operator << (QDebug debug, HandoverType type) {
    QString s;
    switch(type)
    {
    case HandoverType::INVALID:
        s = "HandoverType::INVALID";
        break;
    case HandoverType::GSM_TO_GSM:
        s = "HandoverType::GSM_TO_GSM";
        break;
    case HandoverType::GSM_TO_LTE:
        s = "HandoverType::GSM_TO_LTE";
        break;
    case HandoverType::GSM_TO_UMTS:
        s = "HandoverType::GSM_TO_UMTS";
        break;
    case HandoverType::UMTS_TO_GSM:
        s = "HandoverType::UMTS_TO_GSM";
        break;
    case HandoverType::UMTS_TO_LTE:
        s = "HandoverType::UMTS_TO_LTE";
        break;
    case HandoverType::UMTS_TO_UMTS:
        s = "HandoverType::UMTS_TO_UMTS";
        break;
    case HandoverType::LTE_TO_UMTS:
        s = "HandoverType::LTE_TO_UMTS";
        break;
    case HandoverType::LTE_TO_GSM:
        s = "HandoverType::LTE_TO_GSM";
        break;
    default:
        s = "HandoverType::Black magic";
        break;
    }

    return debug << s;
}

#endif // GLOBAL_H
