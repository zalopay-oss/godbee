package main

import (
	"os"

	log "github.com/sirupsen/logrus"
	"github.com/zalopay-oss/GodBee/pkg/cmd/server"
)

func main() {
	if err := server.RunServer(); err != nil {
		log.Fatal("run server err: ", err)
		os.Exit(1)
	}
}
