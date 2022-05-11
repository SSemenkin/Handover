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
    UMTS_TO_LTE,
    LTE_TO_GSM,
    LTE_TO_UMTS
};

enum class VendorLTE {
    Invalid = -1,
    Huawei,
    Ericsson
};

#endif // GLOBAL_H
