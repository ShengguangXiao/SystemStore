#ifndef _SYSTEM_STORE_CONSTANTS_H_
#define _SYSTEM_STORE_CONSTANTS_H_

namespace AOI
{
namespace SystemStore
{

#define SYSTEM_DB_NAME      "system.cfg"

namespace Enum
{
    enum class YesNo
    {
        UNDEFINED,
        NO,
        YES,
        MIN_ = UNDEFINED,
        MAX_ = YES,
        END_,
    };
}

}
}

#endif