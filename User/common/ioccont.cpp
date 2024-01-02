/*!
 *  @file       ioccont.cpp
 *  @brief      IoC container.
 */

#include "ioccont.hpp"

namespace Common::Ioc {
IOC &
IOC::Get()
{
    static IOC container;
    return container;
}

}
