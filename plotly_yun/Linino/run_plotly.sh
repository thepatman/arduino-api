#!/bin/sh
kill -9 $(pgrep -f "python /root/run.py")
(python /root/run.py)&
