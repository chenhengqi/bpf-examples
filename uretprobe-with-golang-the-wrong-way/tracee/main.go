package main

import (
	"fmt"
	"os"
	"time"
)

//go:noinline
func foobar(s string) (int, int) {
	fmt.Println(s)
	var arr [1024 * 16]byte
	for i, b := range []byte(s) {
		arr[i] = b
	}
	return len(s), len(arr)
}

func main() {
	for {
		foobar(os.Args[1])
		time.Sleep(time.Second)
	}
}
