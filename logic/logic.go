package main

import (
	"enad/logic/app"
)

import "C"

func EncryptGO(inputFile string, key []byte) error {
	return app.EncryptFile(inputFile, key)
}

func DecryptGO(inputFile string, key []byte) error {
	return app.DecryptFile(inputFile, key)
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
