#ifndef HANDOVERMAKER_H
#define HANDOVERMAKER_H

#include "handovers.h"

namespace HandoverMaker
{
    QString makeHandovers(HandoverType type,
                          const QString &filename,
                          VendorLTE = VendorLTE::Ericsson) noexcept;

    QStringList getRowsFromFile(const QString &filename) noexcept;
};

#endif // HANDOVERMAKER_H
