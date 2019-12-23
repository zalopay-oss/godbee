package main

import (
	"os"

	log "github.com/sirupsen/logrus"
	"github.com/zalopay-oss/GodBee/pkg/cmd/client"
)

func main() {
	if err := client.RunClient(); err != nil {
		log.Fatal("run client err: ", err)
		os.Exit(1)
	}
}
