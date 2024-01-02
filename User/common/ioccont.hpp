/*!
 *  @file       ioccont.hpp
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

namespace Common::Ioc {
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
}

class IOC {
public:
    /*!
     * @brief IoC instance accessor.
     * @return The singleton instance of the IoC container.
     */
    static
    IOC &
    Get();

    template<class T>
    using TYPE_FACTORY = std::function<std::shared_ptr<T>()>;

    template<class T>
    using TYPE_FACTORY_PARAMETERIZED = std::function<std::shared_ptr<T>(typename T::IOC_PAYLOAD IocParams)>;

    /*!
     * @brief Registers a factory function for creating instances of non-parameterized type T.
     * @param InstanceFactory The factory function for type T.
     */
    template<Common::Ioc::NOT_PARAMETERIZED T>
    void
    RegisterFactory(
        TYPE_FACTORY<T> InstanceFactory
    )
    {
        IocEntryMap_[typeid(T)] = {
            IOC_STATE::InstanceFactory,
            std::move(InstanceFactory)
        };
    }

    /*!
     * @brief Registers a factory function for creating instances of parameterized type T.
     * @param InstanceFactory The factory function for type T.
     */
    template<Common::Ioc::PARAMETERIZED T>
    void
    RegisterFactory(
        TYPE_FACTORY_PARAMETERIZED<T> InstanceFactory
    )
    {
        IocEntryMap_[typeid(T)] = {
            IOC_STATE::InstanceFactory,
            std::move(InstanceFactory)
        };
    }

    /*!
     * @brief Registers a factory function for creating a singleton instance of type T.
     * @param SingletonFactory The factory function for type T.
     */
    template<class T>
    void
    RegisterSingleton(
        TYPE_FACTORY<T> SingletonFactory
    )
    {
        IocEntryMap_[typeid(T)] = {
            IOC_STATE::SingletonFactory,
            std::move(SingletonFactory)
        };
    }

    /*!
     * @brief Resolves and returns an instance of non-parameterized type T.
     * @return A shared pointer to the resolved instance.
     */
    template<Common::Ioc::NOT_PARAMETERIZED T>
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
    template<Common::Ioc::PARAMETERIZED T>
    std::shared_ptr<T>
    Resolve(
        typename T::IOC_PAYLOAD &&IocParams = {}
    )
    {
        return ResolveInternal<T, TYPE_FACTORY_PARAMETERIZED<T>>(std::forward<typename T::IocParams>(IocParams));
    }

private:
    /*!
     * @brief Enumeration representing the state of the IoC entry.
     */
    enum class IOC_STATE {
        /* Factory used to resolve separate instances. */
        InstanceFactory,

        /* Factory used to resolve global shared singletons in a lazy manner. */
        SingletonFactory,

        /* Global shared singleton instance. */
        SingletonInstance
    };

    /*!
     * @brief Internal class representing an entry in the IoC container.
     */
    class IOC_ENTRY {
    public:
        IOC_STATE State;
        std::any Content;
    };

    template<class T, class G, class... P>
    std::shared_ptr<T>
    ResolveInternal(
        P... Payload
    )
    {
        const auto iterator = IocEntryMap_.find(typeid(T));

        if (iterator == IocEntryMap_.end()) {
            throw std::runtime_error{
                std::format("Failed to find a factory for \"{}\" in the factory map",
                            typeid(T).name())
            };
        }

        auto &entry = iterator->second;

        try {
            std::shared_ptr<T> ptr;

            switch (entry.State) {

            case IOC_STATE::InstanceFactory:
                ptr = std::any_cast<G>(entry)(std::forward<P>(Payload)...);
                break;

            case IOC_STATE::SingletonFactory:
                ptr = std::any_cast<G>(entry)(std::forward<P>(Payload)...);
                entry.Content = ptr;
                entry.State = IOC_STATE::SingletonInstance;
                break;

            case IOC_STATE::SingletonInstance:
                ptr = std::any_cast<std::shared_ptr<T>>(entry.Content);
                break;
            }

            return ptr;

        } catch (const std::bad_any_cast &) {
            throw std::runtime_error{
                std::format("Failed to find a factory or instance for \"{}\" mapped from \"{}\"",
                            typeid(G).name(),
                            entry.Content.type().name())
            };
        }
    }

    std::unordered_map<std::type_index, IOC_ENTRY> IocEntryMap_;
};
