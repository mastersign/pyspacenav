PySpaceNav API
==============

Package: `spacenav`

## Constants

### `spacenav.EVENT_TYPE_MOTION`
The value of attribute `type` on a motion event object.
  
### `spacenav.EVENT_TYPE_BUTTON`
The value of attribute `type` on a button event object.

## Functions

### `spacenav.open()`  
*Establishes a connection to the SpaceNav driver.*

Raises a `spacenav.ConnectionError` if the connection fails.  
Does nothing if a connection already exists.

### `spacenav.close()`  
*Destroyes the connection to the SpaceNav driver.*

Does nothing if no connection exists.

### `spacenav.poll()`  
*Returns the next event if there is any, or* `None` *if no event is queued.*

Raises a `spacenav.ConnectionError` if no connection to the SpaceNav driver exists.

### `spacenav.wait()`  
*Returns the next event. Blocks if no event is queued until a new event arrives.*

Raises a `spacenav.ConnectionError` if no connection to the SpaceNav driver exists.

### `spacenav.remove_events(event_type)`  
*Removes all events of the given type from the event queue.*

`event_type` can be one of the event type constants.  
Raises a `spacenav.ConnectionError` if no connection to the SpaceNav driver exists.

## Attributes

### `spacenav.is_connected`
*The state of the connection to the SpaceNav driver.*

`True` if a connection was established; otherwise `False`.

## Classes

### `spacenav.ConnectionError`
*The exception class of the PySpaceNav extension.*

### `spacenav.ButtonEvent`
*Objects of this class are representing a button event.*

**Attributes**

* `type` the event type, is allways `spacenav.EVENT_TYPE_BUTTON`
* `button` the number of the button
* `pressed` 1 for the press event, 0 for the release event

### `spacenav.MotionEvent`
*Objects of this class are representing a motion event.*

**Attributes**

* `type` the event type, is allways `spacenav.EVENT_TYPE_MOTION`
* `period` the number of milliseconds since the last event
* `x`, `y`, `z` the amount of lateral force on the three axis
* `rx`, `ry`, `rz` the amount of rotational force on the three axis
