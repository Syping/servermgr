## Server Manager
Syping Gaming Team Server Manager

#### FAQ
 - Q: How to connect to localhost without running server? A: type SM_LOCAL as hostname
 - Q: How to setup a Server Manager Server? A: Download http://www.syping.de/download/ServerManagerServer.txz for Linux and read README.txt
 - Q: My 64bit Build of the Server Manager Server don't work, what should i do? A: Compile the Server with a 32bit Qt5
 - Q: The Linux Server doesn't work what can i do? A: Check if all dependencies installed with ldd ServerManagerServer and ldd platforms/libqxcb.so
 - Q: How to run the Linux Server without DISPLAY? A: ./ServerManagerServer.sh -platform minimal

#### Build Server Manager

Make sure you have Qt5.2 or newer installed (pls dont build with Qt4, only if you can't build with Qt5!)

	apt-get install git qtbase5-dev checkinstall
	git clone https://github.com/Syping/servermgr.git
	cd servermgr
	mkdir build
	cd build
	SM_INSTALL_PATH=/usr ../configure
	make
	checkinstall --pkgname=servermgr --pkgversion=$(cat "sm_version")
	
#### Configure Server Manager with the Server

    SM_INSTALL_PATH=/usr ../configure --with-server

Server only

    SM_INSTALL_PATH=/usr ../configure --server-only
	
#### Download Server Manager Binary

https://github.com/Syping/servermgr/releases

#### Download Server Manager Daily Builds (Windows)

Daily-Builds: https://www.syping.de/servermgr_daily/
