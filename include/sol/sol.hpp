#pragma once
#include <cstring>
#include <mutex>
#include <string>
#include <string_view>
#include <stdexcept>
#include <utility>


#if defined(__unix__)
#  include <dlfcn.h>
#elif defined(_WIN32)
#  include <Windows.h>
#endif


namespace sk {
namespace detail {
struct tagLibrary;
struct tagSymbol;
} // namespace sk::detail


using library_t = detail::tagLibrary*;
using symbol_t = detail::tagSymbol*;


/// @brief An exception thrown when the @c Loader is unable to perform one of
///        its operations for loading shared objects, their symbols, or
///        unloading the shared object.
struct SOLException : public std::runtime_error {
    explicit SOLException(const std::string& msg)
        : std::runtime_error{ msg }
    { }
};



/// @brief   Provides the functions for loading a shared object.
/// @details This is a thread safe class, locks on all three operations to make
///          sure behaviour is consistent.
class SOLoader final {
    using mutex_t = std::mutex;
    using string_t = std::string;
    using strview_t = std::string_view;

    template<typename T>
    using lock_t = std::lock_guard<T>;


    static string_t
    getError() noexcept {
    #if defined(__unix__)
        return string_t{ ::dlerror() };
    #elif defined(_WIN32)
        LPSTR messageBuffer = nullptr;
        DWORD lastErrorCode = ::GetLastError();
        DWORD bytesInMessage = ::FormatMessageA(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM     |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            lastErrorCode,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            messageBuffer,
            0,
            nullptr
        );

        auto result = string_t{ messageBuffer };
        ::LocalFree(messageBuffer);
        return result;
    #endif /* platform */
    }


    inline static mutex_t
    k_loader_ops_mutex;


public:
    /// @brief   Attempts to load the library at the given path.
    /// @param   file A valid path to the library to open for loading.
    /// @returns An opaque handle to the loaded library.
    /// @throws  SOLException if the library could not be opened for loading.
    static library_t
    load(strview_t file) {
    #if defined(__unix__)
    #  define solload(f) ::dlopen(f, RTLD_LAZY)
    #elif defined(_WIN32)
    #  define solload(f) ::LoadLibraryA(f)
    #endif /* platform */
        const lock_t<mutex_t>
        lock(k_loader_ops_mutex);

        void* value;
        if ((value = solload(file.data())) == nullptr)
            throw SOLException(getError());

        return static_cast<library_t>(value);
    #undef solload
    }
    
    
    /// @brief   Attempts to load a symbol from the library provided.
    /// @param   lib The library to load the symbol from.
    /// @param   sym The name of the symbol being loaded.
    /// @returns An opaque handle to the loaded symbol in the library.
    /// @throws  SOLException if a symbol could not be fetched from the
    ///          provided library.
    static symbol_t
    symbol(library_t lib, strview_t sym) {
    #if defined(__unix__)
    #  define solsymbol(l, s) ::dlsym(l, s)
    #elif defined(_WIN32)
    #  define solsymbol(l, s) reinterpret_cast<void*>(::GetProcAddress(reinterpret_cast<HMODULE>(l), s))
    #endif /* platform */
        const lock_t<mutex_t>
        lock(k_loader_ops_mutex);

        void* value;
        if ((value = solsymbol(lib, sym.data())) == nullptr)
            throw SOLException(getError());

        return static_cast<symbol_t>(value);
    #undef solsymbol
    }


    /// @brief   Attempts to load a symbol from the library provided.
    /// @tparam  FnPtrT The type to cast the result to after loading it.
    /// @param   lib The library to load the symbol from.
    /// @param   sym The name of the symbol to load from the library.
    /// @returns The casted result of calling @c symbol() with the provided
    ///          arguments.
    /// @throws  SOLException if a symbol could not be fetched from the
    ///          provided library.
    template<typename CastedT>
    static CastedT
    symbol(library_t lib, strview_t sym) {
        return reinterpret_cast<CastedT>(symbol(lib, sym));
    }


    /// @brief  Unloads the library provided.
    /// @param  lib The library to unload.
    /// @throws SOLException if the library was not unloaded properly.
    static void
    unload(library_t lib) {
    #if defined(__unix__)
    #  define solunload(l) (::dlclose(l) == 0)
    #elif defined(_WIN32)
    #  define solunload(l) (::FreeLibrary(reinterpret_cast<HMODULE>(l)) > 0)
    #endif /* platform */
        const lock_t<mutex_t>
        lock(k_loader_ops_mutex);

        if (solunload(lib) == false)
            throw SOLException(getError());
    #undef solunload
    }
};


} // namespace sk::sol