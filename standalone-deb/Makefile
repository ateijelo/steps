app:
	@echo This is the Makefile for the Steps debian standalone package
	@echo This default action does nothing and is in the Makefile
	@echo to please debhelper

install:
	mkdir -p debian/steps/usr/local/
	rsync -av usr-local-steps/ debian/steps/usr/local/steps/
	install -D -m 0755 wrapper.sh debian/steps/usr/bin/steps
	install -D -m 0644 steps.desktop debian/steps/usr/share/applications/steps.desktop
	install -D -m 0644 steps.png debian/steps/usr/share/pixmaps/steps.png
	install -D -m 0644 com.ateijelo.steps.xml debian/steps/usr/share/mime/packages/com.ateijelo.steps.xml
