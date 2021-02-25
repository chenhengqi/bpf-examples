package main

import (
	"fmt"
	"os"
	"time"
)

//go:noinline
func foobar() (int, string, int) {
	var x [10]int
	foo(x)
	return 0xbeef, os.Args[1], 0xfaac
}

//go:noinline
func foo(x [10]int) {
	var y [100]int
	bar(y)
}

//go:noinline
func bar(y [100]int) {
	var z [1000]int
	buz(z)
}

//go:noinline
func buz(z [1000]int) {

}

func main() {
	for {
		a, b, c := foobar()
		fmt.Printf("%x %s %x\n", a, b, c)
		time.Sleep(time.Second)
	}
}
