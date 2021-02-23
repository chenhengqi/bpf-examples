package main

import (
	"fmt"
	"os"
	"time"
)

//go:noinline
func greet(s string) {
	fmt.Println(s)
}

func main() {
	for {
		greet(os.Args[1])
		time.Sleep(time.Second)
	}
}
