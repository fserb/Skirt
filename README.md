Skirt
=====

A modern web raytracer.

Build instructions
------------------

There are 2 build systems for **Skirt**: a native one and a WebAssembly one.

```
./buildgen.sh
```

Will generate `build/native` and `build/wasm` targets to use `ninja`. Then you
can:

```
ninja -C build/native
```

to generate `build/native/skirt` or

```
ninja -C build/wasm
```

to generate the WASM module of Skirt.


Running tests
-------------

```
build/native/skirt_tests
```

or open `wasm_tests.html` after building the wasm target.
