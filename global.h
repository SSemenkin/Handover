#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>
#include <QHash>
#include <QMap>
#include <QVector>
#include <QDebug>

enum class HandoverType{
    GSM_TO_GSM,
    GSM_TO_UMTS,
    GSM_TO_LTE,
    UMTS_TO_GSM,
    UMTS_TO_LTE,
    LTE_TO_GSM,
    LTE_TO_UMTS
};

enum class VendorLTE {
    Huawei,
    Ericsson
};

#endif // GLOBAL_H
