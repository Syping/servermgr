## Server Manager
SyDevTeam Server Manager

#### FAQ
 - Q: How to connect to localhost without running server? A: type SM_LOCAL as hostname
 - Q: How to run the Linux Server without DISPLAY? A: servermgrd -platform minimal

#### Build Server Manager

Make sure you have Qt 5.3 or above installed

	apt-get install git qtbase5-dev checkinstall
	git clone https://github.com/Syping/servermgr.git
	cd servermgr
	mkdir build
	cd build
	SM_INSTALL_PATH=/usr ../configure
	make
	checkinstall --pkgname=servermgr --pkgversion=$(cat "../sm_version")
	
#### Configure Server Manager with the Server

    SM_INSTALL_PATH=/usr ../configure --with-server

Server only

    SM_INSTALL_PATH=/usr ../configure --server-only
	
#### Download Server Manager Binary

Windows and Android: https://github.com/Syping/servermgr/releases
