#!/bin/bash
killall server
nohup ./server 9000 > ./serverlog.log 2>&1 &
