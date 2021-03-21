# os_scheduling_algorithms
Implements simple operating system scheduling algorithms in the C programming language

## Description

Simulates a process queue where the integer next to the process name in the process file (ex. ProcessA 10) represents the amount of CPU time (in seconds) that the process needs to complete

## Getting Started

### Dependencies

* To be run on a x86-64 Linux machine

### Executing program

* User must create a process list file
* Each entry in the file will represent a process which will be placed in the queue
* Each entry should follow the format: [process_name time_remaining]
* The process file must reside in the location where the fopen() function checks
* The user can/should adjust this path in the source file so that it works
* The executable can then be executed as: ./[whatever you named it]

## Authors

* Stefan Silverio
* stefansilverio@gmail.com
* https://twitter.com/stefansilverio
