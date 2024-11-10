package app

import (
	"bytes"
	"crypto/aes"
	"crypto/cipher"
	"crypto/rand"
	"fmt"
	"io"
	"os"
)

func EncryptStream(key []byte, w io.Writer) (io.Writer, []byte, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, nil, err
	}

	iv := make([]byte, aes.BlockSize)
	if _, err := io.ReadFull(rand.Reader, iv); err != nil {
		return nil, nil, err
	}

	stream := cipher.NewCFBEncrypter(block, iv)
	return cipher.StreamWriter{
		S: stream,
		W: w,
	}, iv, nil
}

func EncryptFile(inputFile string, key []byte) error {
	data, err := os.ReadFile(inputFile)
	if err != nil {
		return err
	}

	var encryptData []byte
	encryptWriter := bytes.NewBuffer(encryptData)
	encryptStream, iv, err := EncryptStream(key, encryptWriter)
	if err != nil {
		return err
	}

	if _, err := encryptStream.Write(data); err != nil {
		return err
	}

	finalData := append(iv, encryptWriter.Bytes()...)
	if err := os.WriteFile(inputFile, finalData, 0644); err != nil {
		return err
	}

	fmt.Println("Encryption complete")
	return nil
}
