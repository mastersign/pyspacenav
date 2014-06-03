# SpaceNavigator on Debian
This guide is useful for setting up a the spacenav userland driver `spacenavd` and 
the spnav C library `libspnav` on a debian system.
If you are using another linux distribution some adjustments might be necessary.

The spacenav userland driver and the spnav library are a requirement to build and install
the SpaceNavigator Python C extension `pyspacenav`.

## Install dependencies (optional)

	sudo apt-get update

For build support

	sudo apt-get install gcc make

For `spacenavd` and `libspnav` with X11 support

	sudo apt-get install libx11-dev -y

For OpenGL example "cube" (not working on Raspbian because of missing GLX support):

	sudo apt-get install libgl1-mesa-dev -y
	sudo apt-get install freeglut3-dev -y

## Prepare working directory and download sources

	mkdir ~/spacenav
	cd ~/spacenav

### Download latest releases

	wget -O spacenavd-0.5.tar.gz http://sourceforge.net/projects/spacenav/files/spacenav%20daemon/spacenavd%200.5/spacenavd-0.5.tar.gz/download
	tar -xf spacenavd-0.5.tar.gz
	ln -s spacenavd-0.5 spacenavd

The version 0.5 has a known bug: the period value of any motion event is flawed. A solution is provided [here](http://sourceforge.net/p/spacenav/bugs/4/)

	wget -O libspnav-0.2.2.tar.gz http://sourceforge.net/projects/spacenav/files/spacenav%20library%20%28SDK%29/libspnav%200.2.2/libspnav-0.2.2.tar.gz/download
	tar -xf libspnav-0.2.2.tar.gz
	ln -s libspnav-0.2.2 libspnav

### Alternativly download last revision from trunk

Replace `177` by the current revision of [HEAD](http://sourceforge.net/p/spacenav/code/HEAD/tree/trunk/)

	wget http://sourceforge.net/code-snapshots/svn/s/sp/spacenav/code/spacenav-code-177-trunk.zip
	unzip spacenav-code-177-trunk.zip
	ln -s spacenav-code-177-trunk/spacenavd spacenavd
	ln -s spacenav-code-177-trunk/libspnav libspnav

## Compile and install driver and library
### SpaceNav Driver

Deactivating X11 support if not needed

	sed -i 's/X11=yes/X11=no/' ./spacenavd/configure

Build and install

	pushd ./spacenavd
	./configure && make && sudo make install
	popd

Install custom configuration for `spacenavd`

	pushd ./spacenavd
	sed 's/#led = on/led = on/' doc/example-spnavrc > doc/my-spnavrc
	sudo cp doc/my-spnavrc /etc/spnavrc
	popd

Install init script (with custom `setup_init.2`). 
The `setup_init.2` is found besides this file and needs to be copied
into the `spacenavd` directory. Afterwards it can be run with root previleges.

	sudo ./setup_init.2

### SpaceNav library

Deactivate X11 support if not needed

	sed -i 's/X11=yes/X11=no/' libspnav/configure

Build and install

	pushd ./libspnav
	./configure && make && sudo make install
	popd

## Compile examples
Simple example

	pushd ./libspnav/examples/simple
	make
	./simple_af_unix
	popd

OpenGL example (not working on Raspbian because of missing GLX support)

	pushd ./libspnav/examples/cube
	make
	./cube
	popd
