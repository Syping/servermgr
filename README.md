## Server Manager
Syping Gaming Team Server Manager

#### FAQ
 - Q: How to connect to localhost without running server? A: type SM_LOCAL as hostname
 - Q: How to setup a Server Manager Server? A: Download http://www.syping.de/download/ServerManagerServer.txz for Linux and read README.txt
 - Q: It is planned to make the server open source? A: Yes, if the server 64bit compatible without problems
 - Q: The Linux Server doesn't work what can i do? A: Check if all dependencies installed with ldd ServerManagerServer and ldd platforms/libqxcb.so
 - Q: The Windows Version doesn't work because MSVC* what can i do? A: Install MSVC 2008 SP1 Redist and Install MSVC 2010 SP1 Redist

#### Build Server Manager

Make sure you have Qt5.2 or newer installed (pls dont build with Qt4, only if you can't build with Qt5!)

	apt-get install qtbase5-dev
	git clone https://github.com/Syping/servermgr.git
	cd servermgr
	mkdir build
	cd build
	export SM_INSTALL_PATH=/usr
	/usr/lib/x86_64-linux-gnu/qt5/bin/qmake ../ServerManager.pro
	make
	checkinstall --pkgname=servermgr --pkgversion=1.1.0
