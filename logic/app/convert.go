package app

import (
	"syscall"
	"unsafe"
)

func WcharToChar(wchar *uint16) string {
	if wchar == nil {
		return ""
	}

	u16Slice := make([]uint16, 0)
	for i := 0; ; i++ {
		val := *(*uint16)(unsafe.Pointer(uintptr(unsafe.Pointer(wchar)) + uintptr(i)*unsafe.Sizeof(*wchar)))
		if val == 0 {
			break
		}
		u16Slice = append(u16Slice, val)
	}

	utf16Str := syscall.UTF16ToString(u16Slice)

	return utf16Str
}
