#!/bin/bash

echo Building

g++ myLed.cpp -o myLed.cgi -lcgicc -pthread

echo Build Complete
