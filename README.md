# SOL

a.k.a. Shared Object Loader. This is a header only library that I whipped up to load and unload shared object files on a target platform. It's about what you'd expect it to be, just it's thread safe. I found I do this more often than I care to, and I wanted to have a consolidated way of doing it. This doesn't account for any nuances between platforms at this moment. I will handle those as they come up, for example: the platform macros aren't very robust and account for the bare minimum (`__unix__` and `_WIN32`), and both `dlopen` and `LoadLibraryA` have options to customize their usage which this project doesn't utilize yet. 

## Usage

See the [test file](./tests/sol_test.cpp) for a complete example. The gist is, `sk::SOLoader::load()` will load the library and hand back an opaque `sk::library_t` which you can use in both `sk::SOLoader::symbol()` and `sk::SOLoader::unload()`. `sk::SOLoader::symbol()` will provide an opaque `sk::symbol_t` similar to the load operation, but there is a template function with the same name for directly reinterpreting the result type to your desired type.

## Upcoming

I plan to add build support for my own build tool [CPak](https://github.com/SoraKatadzuma/cpak) once I'm comfortable with it. I'm currently working on some major changes for it, and this library will eventually be included in it. It'll faciliate the foundation of plugins within the build tool so developers using it can customize it to do things that it doesn't currently do or may never support.
