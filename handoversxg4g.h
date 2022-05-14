#ifndef HANDOVERSXG4G_H
#define HANDOVERSXG4G_H

#include "handovers.h"

class HandoversXG4G : public BaseHandovers
{
public:
    HandoversXG4G() = default;

    void setVendorLTE(VendorLTE vendorLTE);
    VendorLTE vendorLTE() const;

protected:
    [[nodiscard]] virtual bool loadTemplatesLTE() const;
    [[nodiscard]] virtual bool loadEricssonTemplatesLTE() const = 0;
    [[nodiscard]] virtual bool loadHuaweiTemplatesLTE() const = 0;

protected:
    VendorLTE m_vendorLTE;

};

#endif // HANDOVERSXG4G_H
