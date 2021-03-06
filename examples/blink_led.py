#!/usr/bin/env python
"""Basic blinking led example.

The led on A20-OLinuXino-LIME  blinks with rate of 1Hz like "heartbeat".

https://www.olimex.com/wiki/A20-OLinuXino-LIME
"""

import os
import sys

if not os.getegid() == 0:
    sys.exit('Script must be run as root')


from time import sleep
from pySUNXI import gpio

__author__ = "Stefan Mavrodiev"
__copyright__ = "Copyright 2014, Olimex LTD"
__credits__ = ["Stefan Mavrodiev"]
__license__ = "GPL"
__version__ = "2.0"
__maintainer__ = __author__
__email__ = "support@olimex.com"


led = gpio.PH(2)

gpio.init()
gpio.setcfg(led, gpio.OUTPUT)

try:
    print ("Press CTRL+C to exit")
    while True:
        gpio.output(led, 1)
        sleep(0.1)
        gpio.output(led, 0)
        sleep(0.1)

        gpio.output(led, 1)
        sleep(0.1)
        gpio.output(led, 0)
        sleep(0.1)

        sleep(0.6)
except KeyboardInterrupt:
    print ("Goodbye.")
