/*!
 *  @file       ioc.cpp
 *  @brief      IoC container.
 */

#include "ioc.hpp"

namespace Common::Ioc {

IOC &
GetIoc()
{
    static IOC iocContainer;
    return iocContainer;
}

SINGLETONS &
GetSingletons()
{
    static SINGLETONS singletonsContainer;
    return singletonsContainer;
}

}
