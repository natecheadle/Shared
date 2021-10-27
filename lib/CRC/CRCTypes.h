#pragma once

#include "CRCHelper.hpp"

namespace Shared
{
    typedef CRCHelper<unsigned short, 0x1021, 0x0000, 0x0000, false> XModem16BitCRC;
    typedef CRCHelper<unsigned short, 0x3D65, 0x0000, 0xFFFF, true> DNP16BitCRC;
}
