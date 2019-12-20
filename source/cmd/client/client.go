package main

import (
	"github.com/1612898/zpkvservice/pkg/cmd/client"
	log "github.com/sirupsen/logrus"
	"os"
)

func main() {
	if err := client.RunClient(); err != nil {
		log.Fatal("run client err: ", err)
		os.Exit(1)
	}
}