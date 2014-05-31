import atexit
import spacenav as sn

# make shure, connection is closed at exit of python interpreter
atexit.register(sn.close)

# open connection to spacenavd
sn.open()

# reset exit condition
stop = False

# loop over space navigator events
while not stop:
	# wait for next event
	event = sn.wait()

	# if event signals the release of the first button
	if type(event) is sn.ButtonEvent \
		and event.button == 0 and event.pressed == 0:
		# set exit condition
		stop = True

	# print event
	print(str(event))
