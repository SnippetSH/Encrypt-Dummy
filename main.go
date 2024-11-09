package main

/*
#cgo CXXFLAGS: -I./lib
#cgo LDFLAGS: -L. -lc_entry
#cgo windows LDFLAGS: -Wl,-subsystem,windows
#include "lib/header/c_entry.h"
*/
import "C"
import "runtime"

func init() {
	runtime.LockOSThread()
}

func main() {
	C.startApp()
}
