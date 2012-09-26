all:
	[ -d build ] || mkdir build; \
        cd build && cmake .. && $(MAKE)

bindings:
	[ -d build ] || mkdir build; \
	cd build && cmake .. -DDEFAULT_NO_BINDINGS=OFF -DENABLE_python=ON && $(MAKE)

install: all
	cd build && $(MAKE) install

clean:
	rm -Rf build

