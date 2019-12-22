package main

import (
	"github.com/1612898/zpkvservice/pkg/cmd/server"
	log "github.com/sirupsen/logrus"
	"os"
)

func main() {
	if err := server.RunServer(); err != nil {
		log.Fatal("run server err: ", err)
		os.Exit(1)
	}
}