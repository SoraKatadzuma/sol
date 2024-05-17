# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2024-05-17

### Added

- `sk::SOLoader` class
- `load(string_view)` for loading a shared object.
- `symbol(library_t, string_view)` for loading a symbol from the shared object.
- `unload(library_t)` for unloading a shared object.
- Tests to verify code works 👍
