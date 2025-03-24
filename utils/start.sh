#!/bin/bash

set -m
echo " - STARTING -"

./utils/websockify/run --cert /etc/letsencrypt/live/benti.dev-0003/cert.pem --key /etc/letsencrypt/live/benti.dev-0003/privkey.pem 9000 localhost:5900 >/dev/null 2>&1 &
websockify_pid=$!

qemu-system-i386 -m 128M -kernel ./output/os.bin -vnc :0 2>/dev/null &
qemu_pid=$!

cleanup() {
	kill $websockify_pid $qemu_pid
	wait $websockify_pid $qemu_pid
	echo " - STOPPED -"
}

trap cleanup SIGINT

wait
