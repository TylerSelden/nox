#!/bin/bash

set -m
echo " - STARTING -"
echo " (remember to run ./utils/gdb.sh)"

./utils/websockify/run --cert /etc/letsencrypt/live/benti.dev-0003/cert.pem --key /etc/letsencrypt/live/benti.dev-0003/privkey.pem 9000 localhost:5900 >/dev/null 2>&1 &
websockify_pid=$!

qemu-system-i386 -kernel ./output/os.bin -vnc :0 -no-reboot -no-shutdown -s -S -monitor stdio 2>/dev/null

cleanup() {
	kill $websockify_pid
	wait $websockify_pid
	echo " - STOPPED -"
}

trap cleanup SIGINT
