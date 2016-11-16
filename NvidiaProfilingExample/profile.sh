#!/bin/bash
export COMPUTE_PROFILE=1
export COMPUTE_PROFILE_CSV=1
pfDir=./NvidiaProfilingExample/profilingFolder/
export COMPUTE_PROFILE_CONFIG=${pfDir}pf.cfg
export COMPUTE_PROFILE_LOG=${pfDir}pflog.csv
./SimpleCL

