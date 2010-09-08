#! /usr/bin/env python
# -*- coding: UTF-8 -*-
#################################################################
#
# ProActive Parallel Suite(TM): The Java(TM) library for
#    Parallel, Distributed, Multi-Core Computing for
#     Enterprise Grids & Clouds
#
# Copyright (C) 1997-2010 ActiveEon 
# Contact: proactive@ow2.org or contact@activeeon.com
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; version 3 of
# the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this library; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
# USA
#
# If needed, contact us to obtain a release under GPL Version 2 
# or a different license than the GPL.
#
#  Initial developer(s):               The ActiveEon Team
#  Contributor(s):
#
#################################################################
# $$ACTIVEEON_INITIAL_DEV$$
#################################################################

import time
import logging

logger = logging.getLogger("agent.ctr")


def mainloop(eventGenerator, speedup=1, bias=0):
    '''
    The main loop of the agent.
    '''

    gen = eventGenerator.getActions()
    
    event     = None
    old_event = None
    
    while True:
        try:
            old_event = event
            event = gen.next()
            
            if old_event is not None:
                old_event.cancel()
            
            logger.debug("Controller is scheduling an event -> type:%s start:%s duration:%s" % (event.type, event.epoch_date, event.duration)) 
            event.schedule()
            
            # FIXME: sleep shouldn't be used. It should be event based ! 
            sleep_time = max(0, int((event.epoch_date + event.duration) - time.time()))
            logger.debug("Controller will sleep for %s seconds" % sleep_time)
            time.sleep(sleep_time)
            
        except StopIteration:
            logger.critical("Controller failed to get the next event for the event generator.")
            break
        except (KeyboardInterrupt, SystemExit):
            # Terminate all the processes then exit
            logger.info("Daemon exiting...")
            if old_event is not None:
                old_event.cancel()
            if event is not None:
                event.cancel()
                return