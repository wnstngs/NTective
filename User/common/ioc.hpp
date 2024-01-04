/*!
 *  @file       ioc.hpp
 *  @brief      IoC container.
 *  @details    The implementation of an IoC container that supports registration
 *              and resolution of factories for creating instances of various types,
 *              both non-parameterized and parameterized and the registration and
 *              resolution of global shared singleton objects.
 */

#pragma once

#include <any>
#include <format>
#include <functional>
#include <memory>
#include <typeindex>

#include "assert.hpp"
#include "strutil.hpp"

namespace Common::Ioc {

class FACTORY_NOT_FOUND_EXCEPTION : public Util::BUF_EXCEPTION {
public:
    using BUF_EXCEPTION::BUF_EXCEPTION;
};

/*!
 * @brief Concept to identify types with payload.
 */
template<class T>
concept PARAMETERIZED = requires() {
    {
        typename T::IOC_PAYLOAD{}
    };
};

/*!
 * @brief Concept to identify types without payload.
 */
template<class T>
concept NOT_PARAMETERIZED = !PARAMETERIZED<T>;

template<class T>
using TYPE_FACTORY = std::function<std::shared_ptr<T>()>;

template<class T>
using TYPE_FACTORY_PARAMETERIZED = std::function<std::shared_ptr<T>(typename T::IOC_PAYLOAD IocParams)>;

/*!
 * @brief IoC container.
 */
class IOC {
public:
    /*!
     * @brief Registers a factory function for creating instances of non-parameterized type T.
     * @param InstanceFactory The factory function for type T.
     */
    template<NOT_PARAMETERIZED T>
    void
    RegisterFactory(
        TYPE_FACTORY<T> InstanceFactory
    )
    {
        IocContainerMap_[typeid(T)] = InstanceFactory;
    }

    /*!
     * @brief Registers a factory function for creating instances of parameterized type T.
     * @param InstanceFactory The factory function for type T.
     */
    template<PARAMETERIZED T>
    void
    RegisterFactory(
        TYPE_FACTORY_PARAMETERIZED<T> InstanceFactory
    )
    {
        IocContainerMap_[typeid(T)] = InstanceFactory;
    }

    /*!
     * @brief Resolves and returns an instance of non-parameterized type T.
     * @return A shared pointer to the resolved instance.
     */
    template<NOT_PARAMETERIZED T>
    std::shared_ptr<T>
    Resolve()
    {
        return ResolveInternal<T, TYPE_FACTORY<T>>();
    }

    /*!
     * @brief Resolves and returns an instance of parameterized type T.
     * @param IocParams The parameters for creating the instance of type T.
     * @return A shared pointer to the resolved instance.
     */
    template<PARAMETERIZED T>
    std::shared_ptr<T>
    Resolve(
        typename T::IOC_PAYLOAD &&IocParams = {}
    )
    {
        return ResolveInternal<T, TYPE_FACTORY_PARAMETERIZED<T>>(std::forward<typename T::IOC_PAYLOAD>(IocParams));
    }

private:
    template<class T, class G, class... P>
    std::shared_ptr<T>
    ResolveInternal(
        P&&... Payload
    )
    {
        const auto iterator = IocContainerMap_.find(typeid(T));
        if (iterator == IocContainerMap_.end()) {
            throw FACTORY_NOT_FOUND_EXCEPTION{
                std::format("Failed to find factory for \"{}\" in the factory map",
                            typeid(T).name())
            };
        }

        const auto &entry = iterator->second;

        try {
            return std::any_cast<G>(entry)(std::forward<P>(Payload)...);
        } catch (const std::bad_any_cast &) {
            NTECTIVE_CHECK_FAIL.Message(std::format(L"Could not resolve factory for type \"{}\" mapped from \"{}\"",
                                                    Util::StringToWstring(typeid(G).name()),
                                                    Util::StringToWstring(entry.type().name())))
                               .Throw();
        }
    }

    std::unordered_map<std::type_index, std::any> IocContainerMap_;
};

/*!
 * @brief IoC instance accessor.
 * @return The singleton instance of the IoC container.
 */
IOC &
GetIoc();

/*!
 * @brief Singleton container.
 */
class SINGLETONS {
public:
    /*!
     * @brief Registers a factory function for creating a singleton instance of type T.
     * @param SingletonFactory The factory function for type T.
     */
    template<class T>
    void
    RegisterFactory(
        TYPE_FACTORY<T> SingletonFactory
    )
    {
        SingletonContainerMap_[typeid(T)] = SingletonFactory;
    }

    /*!
     * @brief Registers a factory function that essentially passes through the resolution
     * to the IoC container for the specified type T.
     */
    template<class T>
    void
    RegisterDelegateFactory()
    {
        RegisterFactory<T>([] {
            return GetIoc().Resolve<T>();
        });
    }

    template<class T>
    std::shared_ptr<T>
    Resolve()
    {
        const auto iterator = SingletonContainerMap_.find(typeid(T));

        if (iterator == SingletonContainerMap_.end()) {
            throw FACTORY_NOT_FOUND_EXCEPTION{
                std::format("Could not find entry for type \"{}\" in singleton container",
                            typeid(T).name())
            };
        }

        auto &entry = iterator->second;

        try {
            if (auto existingInstance = std::any_cast<std::shared_ptr<T>>(&entry)) {
                return *existingInstance;
            }

            auto instance = std::any_cast<TYPE_FACTORY<T>>(entry)();
            entry = instance;

            return instance;

        } catch (const std::bad_any_cast &) {
            NTECTIVE_CHECK_FAIL.Message(std::format(L"Could not resolve singleton for type \"{}\" mapped from \"{}\"",
                                                    Util::StringToWstring(entry.type().name()),
                                                    Util::StringToWstring(typeid(TYPE_FACTORY<T>).name())))
                               .Throw();
        }
    }

private:
    std::unordered_map<std::type_index, std::any> SingletonContainerMap_;
};

SINGLETONS &
GetSingletons();

}
