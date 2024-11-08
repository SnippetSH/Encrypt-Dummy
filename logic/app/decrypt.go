package app

import (
	"crypto/aes"
	"crypto/cipher"
	"fmt"
	"io"
	"os"
	"path/filepath"
)

func DecryptStream(key, iv []byte, r io.Reader) (io.Reader, error) {
	block, err := aes.NewCipher(key)
	if err != nil {
		return nil, err
	}

	stream := cipher.NewCFBDecrypter(block, iv)
	return cipher.StreamReader{
		S: stream,
		R: r,
	}, nil
}

func DecryptFile(inputFile string, key []byte) error {
	// 원본 파일 열기
	file, err := os.Open(inputFile)
	if err != nil {
		return err
	}
	defer file.Close()

	iv := make([]byte, aes.BlockSize)
	if _, err := io.ReadFull(file, iv); err != nil {
		return err
	}

	// 복호화 스트림 생성
	decryptReader, err := DecryptStream(key, iv, file)
	if err != nil {
		return err
	}

	// 임시 파일 생성
	tempFile, err := os.CreateTemp(filepath.Dir(inputFile), "temp_*")
	if err != nil {
		return err
	}
	defer os.Remove(tempFile.Name()) // 함수 종료 시 임시 파일 삭제

	// 복호화된 내용을 임시 파일에 쓰기
	if _, err := io.Copy(tempFile, decryptReader); err != nil {
		return err
	}

	// 임시 파일 닫기
	if err := tempFile.Close(); err != nil {
		return err
	}

	// 원본 파일 닫기
	if err := file.Close(); err != nil {
		return err
	}

	// 원본 파일 삭제
	if err := os.Remove(inputFile); err != nil {
		return err
	}

	// 임시 파일의 이름을 원본 파일 이름으로 변경
	if err := os.Rename(tempFile.Name(), inputFile); err != nil {
		return err
	}

	fmt.Println("Decryption complete")
	return nil
}
