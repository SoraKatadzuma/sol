#include <cassert>
#include <iostream>
#include "sol/sol.hpp"


int main() {
#if defined(__unix__)
    constexpr std::string_view
    k_library_name = "libc.so.6";
#elif defined(_WIN32)
    constexpr std::string_view
    k_library_name = "msvcrt.dll";
#endif

    constexpr std::string_view
    k_symbol_name = "printf";

    try {
        using PFN_printf = int(*)(const char*, ...);

        auto lib = sk::SOLoader::load(k_library_name);
        assert(lib != nullptr);   

        auto sym = sk::SOLoader::symbol<PFN_printf>(lib, k_symbol_name);
        assert(sym != nullptr);

        (*sym)("Testing the %s call!", "printf");
        sk::SOLoader::unload(lib);
    } catch (const std::runtime_error& anyerr) {
        std::cout << anyerr.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
