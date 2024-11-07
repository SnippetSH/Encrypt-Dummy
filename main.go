package main

/*
#cgo CXXFLAGS: -I./lib
#cgo LDFLAGS: -L. -lentry
#include "entry.h"
*/
import "C"

func main() {
	C.startApp()
}
