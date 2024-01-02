/*!
 *  @file       ioccont.cpp
 *  @brief      IoC container.
 */

#include "ioccont.hpp"

IOC &
IOC::Get()
{
    static IOC container;
    return container;
}
