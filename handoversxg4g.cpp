#include "handoversxg4g.h"

void HandoversXG4G::setVendorLTE(VendorLTE vendorLTE)
{
    m_VendorLTE = vendorLTE;
}

VendorLTE HandoversXG4G::vendorLTE() const
{
    return m_VendorLTE;
}
