package main

import (
	"io"
	"log"
	"net"
)

func serve(c net.Conn) {
	defer c.Close()

	log.Printf("client connected: %s\n", c.RemoteAddr().String())
	io.Copy(c, c)
	log.Printf("client closed: %s\n", c.RemoteAddr().String())
}

func main() {
	lis, err := net.Listen("tcp", ":12160")
	if err != nil {
		panic(err)
	}

	for {
		conn, err := lis.Accept()
		if err != nil {
			panic(err)
		}
		go serve(conn)
	}
}
