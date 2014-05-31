import spacenav, atexit

try:
	# open the connection
	spacenav.open()
	# register the close function if no exception was raised
	atexit.register(spacenav.close)
except spacenav.ConnectionError:
	# give some user advice if the connection failed 
	print("No connection to the SpaceNav driver. Is spacenavd running?")
	sys.exit(-1)

# reset exit condition
stop = False

# loop over space navigator events
while not stop:
	# wait for next event
	event = spacenav.wait()

	# if event signals the release of the first button
	if type(event) is spacenav.ButtonEvent \
		and event.button == 0 and event.pressed == 0:
		# set exit condition
		stop = True

	# print event
	print(str(event))
