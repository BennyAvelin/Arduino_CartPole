#!/bin/sh
arduino-builder -compile -build-options-file build/build.options.json -build-path $(pwd)/.build Bluetooth.ino
