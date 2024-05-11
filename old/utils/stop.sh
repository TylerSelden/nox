#!/bin/bash

kill -9 $(jobs -p)

pkill -9 python3

pkill -9 qemu-system-i38
