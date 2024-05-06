#!/bin/bash

# Start a new process group for this script
set -m

# Start the first process in the background
./utils/websockify/run --cert /etc/letsencrypt/live/benti.dev-0003/cert.pem --key /etc/letsencrypt/live/benti.dev-0003/privkey.pem 9000 localhost:5900 &
websockify_pid=$!

# Start the second process in the background
qemu-system-i386 -drive format=raw,if=floppy,file=./build/os.bin -vnc :0 &
#qemu-system-i386 -fda ./build/os.bin -vnc :0 &
qemu_pid=$!

# Function to clean up when script receives SIGINT
clean_up() {
    # Send SIGTERM to all jobs in the current process group
    kill -TERM $websockify_pid $qemu_pid
    wait $websockify_pid $qemu_pid 2> /dev/null
}

# Trap SIGINT to call the clean_up function
trap clean_up SIGINT

# Wait for the processes to finish
wait

