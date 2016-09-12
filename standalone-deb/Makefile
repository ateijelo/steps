PKGS = libgeographic17 libicu57 libjpeg62-turbo libpcre16-3 libpng16-16 \
       libqt5core5a libqt5dbus5 libqt5gui5 libqt5network5 libqt5sql5 libqt5sql5-sqlite \
       libqt5widgets5 libqt5xml5 libstdc++6 libx11-xcb1 libxcb1 libxcb-glx0 \
       libxcb-icccm4 libxcb-image0 libxcb-keysyms1 libxcb-randr0 libxcb-render0 \
       libxcb-render-util0 libxcb-shape0 libxcb-shm0 libxcb-sync1 libxcb-util0 \
       libxcb-xfixes0 libxcb-xinerama0 libxcb-xkb1 libxi6 libxkbcommon0 \
       libxkbcommon-x11-0 libxrender1

deb: getdeps buildbinary
	$(MAKE) builddeb


skipdeps: buildbinary
	$(MAKE) builddeb

getdeps:
	mkdir -p steps/usr-local-steps
	mkdir -p pkgs/
	for PKG in $(PKGS);	do \
		sh -c "cd pkgs; apt-get download $${PKG};" \
		echo extracting $${PKG}; \
		dpkg -x pkgs/$${PKG}_* steps/usr-local-steps/; \
	done

buildbinary:
	mkdir -p build
	cd build; qmake ../../; cd ..
	$(MAKE) -C build

builddeb:
	rm -rf steps/usr-local-steps/usr/share/doc/*
	install -D -m 0755 build/steps steps/usr-local-steps/usr/bin/steps
	strip steps/usr-local-steps/usr/bin/steps
	cd steps; dpkg-buildpackage -b -us -uc

.PHONY: clean

clean:
	rm -rf pkgs
	rm -rf steps/usr-local-steps
	rm -f steps_*.deb
	rm -f steps_*.changes
	rm -rf build
	rm -rf steps/debian/steps
	rm -rf steps/debian/.debhelper
