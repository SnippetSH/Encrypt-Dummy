package main

/*
#cgo CXXFLAGS: -I./lib
#cgo LDFLAGS: -L. -lentry
#include "lib/header/entry.h"
*/
import "C"

func main() {
	C.startApp()
}
