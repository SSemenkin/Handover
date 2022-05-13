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
    VendorLTE m_VendorLTE {VendorLTE::Invalid};
};

#endif // HANDOVERSXG4G_H
