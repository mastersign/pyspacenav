#!/usr/bin/python3

import atexit
import spacenav as sn

atexit.register(sn.close)
sn.open()

stop = False

while not stop:
	event = sn.wait()
	if type(event) is sn.ButtonEvent \
		and event.button == 0 \
		and event.pressed == 0:

		stop = True

	print(str(event))
