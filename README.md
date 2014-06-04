PySpaceNav
==========

PySpaceNav is a C extension for Python 2.2+ and 3.x, which exposes the capability to receive input events from Connexion SpaceNavigator devices (3D mouses). PySpaceNav depends on the [SpaceNav] project. It make use of *libspnav* to communicate with the userland driver *spacenavd*. PySpaceNav is published under the [MIT license].

Currently PySpaceNav only supports Linux.

Quick Start
-----------

Make shure you have build and installed *spacenavd* and *libspnav*. You can use the included guide [SpaceNav on Debian].

For building PySpaceNav you need the development package for your python.

```shell
sudo apt-get install python3-dev
```

Run the following lines to build and install PySpaceNav:

```shell
python3 setup.py build
sudo python3 setup.py install
```

To see if everything works you can run the included [example](example.py):

```shell
python3 example.py
```

Check out the [API] of **PySpaceNav** for detailed information.

Best Practice
-------------

To make shure that the connection to the SpaceNav driver is closed properly, 
regardless of how your python script ends, you can use the *atexit* module.

```python
import spacenav, atexit

try:
	# open the connection
	spacenav.open()
	# register the close function if no exception was raised
	atexit.register(spacenav.close)
except spacenav.ConnectionError:
	# give some user advice if the connection failed
	print("No connection to the SpaceNav driver. Is spacenavd running?")

# check if the connection was established
if spacenav.is_connected:
	# do your normal business here
else:
	# do your fallback business here
```

[SpaceNav]: http://spacenav.sourceforge.net/
[SpaceNav on Debian]: doc/spacenav_on_debian.md
[MIT license]: http://opensource.org/licenses/MIT
[API]: doc/api.md

