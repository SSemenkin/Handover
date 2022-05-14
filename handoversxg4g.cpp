#include "handoversxg4g.h"

void HandoversXG4G::setVendorLTE(VendorLTE vendorLTE)
{
    m_vendorLTE = vendorLTE;
}

VendorLTE HandoversXG4G::vendorLTE() const
{
    return m_vendorLTE;
}

bool HandoversXG4G::loadTemplatesLTE() const
{
    return m_vendorLTE == VendorLTE::Ericsson ? loadEricssonTemplatesLTE() : loadHuaweiTemplatesLTE();
}
