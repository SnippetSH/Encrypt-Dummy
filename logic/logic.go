package main

import (
	"enad/logic/app"
	"syscall"
	"unsafe"
)

import "C"

func CharToWcharGo(str *byte) *uint16 {
	if str == nil {
		return nil
	}

	goStr := ""
	for i := 0; ; i++ {
		val := *(*byte)(unsafe.Pointer(uintptr(unsafe.Pointer(str)) + uintptr(i)*unsafe.Sizeof(*str)))
		if val == 0 {
			break
		}
		goStr += string(val)
	}

	utf16Str := syscall.StringToUTF16(goStr)

	size := len(utf16Str) * int(unsafe.Sizeof(utf16Str[0]))
	utf16Ptr := (*uint16)(C.malloc(C.size_t(size)))
	if utf16Ptr == nil {
		return nil
	}

	// UTF-16 문자열을 할당된 메모리로 복사
	for i, val := range utf16Str {
		*(*uint16)(unsafe.Pointer(uintptr(unsafe.Pointer(utf16Ptr)) + uintptr(i)*unsafe.Sizeof(val))) = val
	}
	return utf16Ptr
}

func EncryptGO(inputFile string, key []byte) error {
	return app.EncryptFile(inputFile, key)
}

func DecryptGO(inputFile string, key []byte) error {
	return app.DecryptFile(inputFile, key)
}

func WcharToCharGo(wchar *uint16) string {
	return app.WcharToChar(wchar)
}

//export CharToWchar
func CharToWchar(str *C.char) *C.wchar_t {
	wchar := CharToWcharGo((*byte)(unsafe.Pointer(str)))
	return (*C.wchar_t)(unsafe.Pointer(wchar))
}

//export WcharToChar
func WcharToChar(wchar *C.wchar_t) *C.char {
	utf8Str := WcharToCharGo((*uint16)(unsafe.Pointer(wchar)))
	return C.CString(utf8Str)
}

//export Encrypt
func Encrypt(inputFile *C.char, key *C.char) *C.char {
	e := EncryptGO(C.GoString(inputFile), []byte(C.GoString(key)))
	if e != nil {
		return C.CString(e.Error())
	}
	return nil
}

//export Decrypt
func Decrypt(inputFile *C.char, key *C.char) *C.char {
	e := DecryptGO(C.GoString(inputFile), []byte(C.GoString(key)))
	if e != nil {
		return C.CString(e.Error())
	}
	return nil
}

func main() {}
