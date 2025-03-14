#!/bin/bash

set -m

su $USER -c "./utils/websockify/run --cert /etc/letsencrypt/live/benti.dev-0003/cert.pem --key /etc/letsencrypt/live/benti.dev-0003/privkey.pem 9000 localhost:5900" /dev/null 2>&1 &
websockify_pid=$!

su $USER -c "qemu-system-i386 -m 128M -kernel ./output/os.bin -vnc :0" /dev/null 2>&1 &
#su $USER -c "qemu-system-i386 ./output/os.iso -vnc :0" /dev/null 2>&1 &
qemu_pid=$!

cleanup() {
	pkill -9 python3
	pkill -9 qemu-system-i38
	wait $websockify_pid $qemu_pid 2>/dev/null
}

trap cleanup SIGINT
trap cleanup SIGKILL

wait
