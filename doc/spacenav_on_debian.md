# SpaceNavigator on Debian

## Install dependencies (optional)
For `spacenavd` and `libspnav` with X11 support

	sudo apt-get update
	sudo apt-get install libx11-dev -y

For OpenGL example "cube" (not working on Raspberry PI):

	sudo apt-get install libgl1-mesa-dev -y
	sudo apt-get install freeglut3-dev -y

## Prepare working directory and download sources

	mkdir ~/spacenav
	cd ~/spacenav

### Download latest releases

	wget -O spacenavd-0.5.tar.gz http://sourceforge.net/projects/spacenav/files/spacenav%20daemon/spacenavd%200.5/spacenavd-0.5.tar.gz/download
	tar -xf spacenavd-0.5.tar.gz
	ln -s spacnavd-0.5 spacenavd

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

Hint: Has currently a bug which prevents the spacenavd connecting with the device.

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
	sudo cp ./doc/example_spnavrc /etc/spnavrc
	sudo nano /etc/spnavrc
	# => uncomment the led line
	popd

Install init script (with custom `setup_init.2`). 
The `setup_init.2` is found besides this file and needs to be copied
into the `spacenavd` directory. Afterwards it can be run with root previleges.

	sudo ./setup_init.2

### SpaceNav library

Deactivate X11 support if not needed

	sed -i 's/X11=yes/X11=no/' ./configure

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

OpenGL example (not working on Raspberry PI)

	pushd ./libspnav/examples/cube
	make
	./cube
	popd
