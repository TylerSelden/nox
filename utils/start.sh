#!/bin/bash

set -m

./utils/websockify/run --cert /etc/letsencrypt/live/benti.dev-0003/cert.pem --key /etc/letsencrypt/live/benti.dev-0003/privkey.pem 9000 localhost:5900 &
websockify_pid=$!

qemu-system-i386 -kernel ./output/os.bin -vnc :0
qemu_pid=$!

cleanup() {
  kill -TERM $websockify_pid $qemu_pid
  wait $websockify_pid $qemu_pid 2>/dev/null
}

trap cleanup SIGINT
trap cleanup SIGKILL

wait