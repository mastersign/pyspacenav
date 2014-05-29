#include <Python.h>
#include <structmember.h>
#include <spnav.h>

static PyObject *ConnectionError;

static int open;

static void set_state(PyObject *m, int state)
{
	PyObject *md;
	md = PyModule_GetDict(m);

	open = state;
	if (open)
	{
		PyDict_SetItemString(md, "is_connected", Py_True);
	}
	else
	{
		PyDict_SetItemString(md, "is_connected", Py_False);
	}
}

static PyObject * spacenav_open(PyObject *self, PyObject *args)
{
	if (open) Py_RETURN_NONE;
	if (spnav_open()==-1) {
	  	PyErr_SetString(ConnectionError, "Connection to spacenavd failed.");
		return NULL;
	}
	set_state(self, -1);
	Py_RETURN_NONE;
}

static PyObject * spacenav_close(PyObject *self, PyObject *args)
{
	if (open)
	{
		spnav_close();
		set_state(self, 0);
	}
	Py_RETURN_NONE;
}

typedef struct {
	PyObject_HEAD
	int type;
	int button;
	int pressed;
} ButtonEvent;

static int ButtonEvent_init(ButtonEvent *self, PyObject *args, PyObject *kwds)
{
	int button, pressed;

	if (!PyArg_ParseTuple(args, "ii", &button, &pressed))
		return -1;

	self->type = SPNAV_EVENT_BUTTON;
	self->button = button;
	self->pressed = pressed;

	return 0;
}

static PyObject * ButtonEvent_str(ButtonEvent *self)
{
	return PyUnicode_FromFormat(
		"ButtonEvent no \%d pressed \%d",
		self->button, self->pressed);
}

static PyMemberDef ButtonEvent_members[] = {
	{ "type", T_INT, offsetof(ButtonEvent, type), 0, "The event type" },
	{ "button", T_INT, offsetof(ButtonEvent, button), 0, "The button number" },
	{ "pressed", T_INT, offsetof(ButtonEvent, pressed), 0, "The button state: 1 for pressed, 0 for released" },
	{ NULL }
};

static PyTypeObject ButtonEventType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"spacenav.ButtonEvent",       /* tp_name */
	sizeof(ButtonEvent),          /* tp_basicsize */
};

typedef struct {
	PyObject_HEAD
	int type;
	unsigned int period;
	int x, y, z;
	int rx, ry, rz;
} MotionEvent;

static int MotionEvent_init(MotionEvent *self, PyObject *args, PyObject *kwds)
{
	unsigned int period;
	int x, y, z, rx, ry, rz;

	if (!PyArg_ParseTuple(args, "Iiiiiii", &period, &x, &y, &z, &rx, &ry, &rz))
		return -1;

	self->type = SPNAV_EVENT_MOTION;
	self->period = period;
	self->x = x;
	self->y = y;
	self->z = z;
	self->rx = rx;
	self->ry = ry;
	self->rz = rz;

	return 0;
}

static PyObject * MotionEvent_str(MotionEvent *self)
{
	return PyUnicode_FromFormat(
		"MotionEvent \%d ms lateral (\%d, \%d, \%d) rotational (\%d, \%d, \%d)",
		self->period, self->x, self->y, self->z, self->rx, self->ry, self->rz);
}

static PyMemberDef MotionEvent_members[] = {
	{ "type", T_INT, offsetof(MotionEvent, type), 0, "The event type" },
	{ "period", T_UINT, offsetof(MotionEvent, period), 0, "The period of time since the last event in milliseconds" },
	{ "x", T_INT, offsetof(MotionEvent, x), 0, "lateral force on the X axis" },
	{ "y", T_INT, offsetof(MotionEvent, y), 0, "lateral force on the Y axis" },
	{ "z", T_INT, offsetof(MotionEvent, z), 0, "lateral force on the Z axis" },
	{ "rx", T_INT, offsetof(MotionEvent, rx), 0, "rotary force around the X axis" },
	{ "ry", T_INT, offsetof(MotionEvent, ry), 0, "rotary force around the Y axis" },
	{ "rz", T_INT, offsetof(MotionEvent, rz), 0, "rotary force around the Z axis" },
	{ NULL }
};

static PyTypeObject MotionEventType = {
	PyVarObject_HEAD_INIT(NULL, 0)
	"spacenav.MotionEvent",       /* tp_name */
	sizeof(MotionEvent),          /* tp_basicsize */
};


static PyObject * translate_spnav_event(spnav_event *sev)
{
	if (sev->type == SPNAV_EVENT_MOTION)
	{
		MotionEvent *me;
		me = (MotionEvent *)PyType_GenericNew(&MotionEventType, NULL, NULL);
		me->type = sev->type;
		me->period = sev->motion.period;
		me->x = sev->motion.x;
		me->y = sev->motion.y;
		me->z = sev->motion.z;
		me->rx = sev->motion.rx;
		me->ry = sev->motion.ry;
		me->rz = sev->motion.rz;
		return (PyObject *)me;
	}
	else if (sev->type == SPNAV_EVENT_BUTTON)
	{
		ButtonEvent *be;
		be = (ButtonEvent *)PyType_GenericNew(&ButtonEventType, NULL, NULL);
		be->type = sev->type;
		be->button = sev->button.bnum;
		be->pressed = sev->button.press;
		return (PyObject *)be;
	}
	else
	{
		Py_RETURN_NONE;
	}
}

static PyObject * spacenav_poll(PyObject *self, PyObject *args)
{
	spnav_event sev;

	if (!open)
	{
		PyErr_SetString(ConnectionError, "No open connection to spacenavd. Call spacenav.open() first.");
		return NULL;
	}
	if (spnav_poll_event(&sev))
	{
		return translate_spnav_event(&sev);
	}
	else
	{
		Py_RETURN_NONE;
	}
}

static PyObject * spacenav_wait(PyObject *self, PyObject *args)
{
	spnav_event sev;

	if (!open)
	{
		PyErr_SetString(ConnectionError, "No open connection to spacenavd. Call spacenav.open() first.");
		return NULL;
	}
	if (spnav_wait_event(&sev))
	{
		return translate_spnav_event(&sev);
	}
	else
	{
		spnav_close();
		set_state(self, 0);
		Py_RETURN_NONE;
	}
}

static PyObject * spacenav_remove_events(PyObject *self, PyObject *args)
{
	int type;
	if (!PyArg_ParseTuple(args, "i", &type))
	{
		return NULL;
	}
	if (!open)
	{
		PyErr_SetString(ConnectionError, "No open connection to spacenavd. Call spacenav.open() first.");
		return NULL;
	}
	return PyLong_FromLong(spnav_remove_events(type));
}

static PyMethodDef SpaceNavMethods[] = {
	{ "open", spacenav_open, METH_NOARGS, "Open connection to spacenavd." },
	{ "close", spacenav_close, METH_NOARGS, "Close connection to spacenavd." },
	{ "poll", spacenav_poll, METH_NOARGS, "Poll for next event." },
	{ "wait", spacenav_wait, METH_NOARGS, "Wait for next event." },
	{ "remove_events", spacenav_remove_events, METH_VARARGS, "Removes buffered events from the queue." },
	{ NULL, NULL, 0, NULL }
};

static struct PyModuleDef spacenavmodule = {
	PyModuleDef_HEAD_INIT,
	"spacenav", /* name of module */
	NULL,       /* module documentation, my be NULL */
	-1,         /* size of per-interpreter state of the module,
	               or -1 of the module keeps state in global variables. */
	SpaceNavMethods
};

PyMODINIT_FUNC PyInit_spacenav(void)
{
	PyObject *m;

	ButtonEventType.tp_flags = Py_TPFLAGS_DEFAULT;
	ButtonEventType.tp_doc = "A SpaceNav button event.";
	ButtonEventType.tp_new = PyType_GenericNew;
	ButtonEventType.tp_init = (initproc)ButtonEvent_init;
	ButtonEventType.tp_members = ButtonEvent_members;
	ButtonEventType.tp_str = (reprfunc)ButtonEvent_str;
	if (PyType_Ready(&ButtonEventType) < 0)
		return NULL;

	MotionEventType.tp_flags = Py_TPFLAGS_DEFAULT;
	MotionEventType.tp_doc = "A SpaceNav motion event.";
	MotionEventType.tp_new = PyType_GenericNew;
	MotionEventType.tp_init = (initproc)MotionEvent_init;
	MotionEventType.tp_members = MotionEvent_members;
	MotionEventType.tp_str = (reprfunc)MotionEvent_str;
	if (PyType_Ready(&MotionEventType) < 0)
		return NULL;

	m = PyModule_Create(&spacenavmodule);

	if (m == NULL)
		return NULL;

	set_state(m, 0);

	Py_INCREF(&ButtonEventType);
	PyModule_AddObject(m, "ButtonEvent", (PyObject *)&ButtonEventType);
	Py_INCREF(&MotionEventType);
	PyModule_AddObject(m, "MotionEvent", (PyObject *)&MotionEventType);

	ConnectionError = PyErr_NewException("spacenav.ConnectionError", NULL, NULL);
	Py_INCREF(ConnectionError);
	PyModule_AddObject(m, "ConnectionError", ConnectionError);

	PyModule_AddIntConstant(m, "EVENT_TYPE_MOTION", SPNAV_EVENT_MOTION);
	PyModule_AddIntConstant(m, "EVENT_TYPE_BUTTON", SPNAV_EVENT_BUTTON);
	PyModule_AddIntConstant(m, "EVENT_TYPE_ANY", SPNAV_EVENT_ANY);

	return m;
}
