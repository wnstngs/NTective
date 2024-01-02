/*!
 *  @file       ioccont.cpp
 *  @brief      IoC container.
 */

#include "ioccont.hpp"

namespace Common::Ioc {

IOC &
GetIoc()
{
    static IOC iocContainer;
    return iocContainer;
}

}
