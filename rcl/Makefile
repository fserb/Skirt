DEPS = Makefile vec3.h raytracer.h objects.h
SRC = rcl.cpp
OUTPUT = rcl.mjs
ROOT = $(shell pwd)

$(OUTPUT): $(SRC) $(DEPS)
	emcc -o $(OUTPUT) \
		-Oz \
		-std=c++14 \
		-s EXTRA_EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
		-s MODULARIZE=1 \
		-s EXPORT_ALL=1 \
		-s EXPORT_ES6=1 \
		$(SRC)

clean:
	rm -f $(OUTPUT) rcl.wasm rcl.mjs
