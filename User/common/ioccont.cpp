/*!
 *  @file       ioccont.cpp
 *  @brief      IoC container.
 */

#include "ioccont.hpp"

IOC_CONTAINER &
IOC_CONTAINER::Get()
{
    static IOC_CONTAINER container;
    return container;
}
