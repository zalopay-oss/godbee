package main

import (
	log "github.com/sirupsen/logrus"
	"github.com/zalopay-oss/GodBee/benchmark/run/cmd"
	"github.com/zalopay-oss/GodBee/configs"

	"github.com/zalopay-oss/GodBee/benchmark"

	"github.com/spf13/viper"
)

func run() {
	//load config
	config := &configs.ServiceConfig{}
	if err := configs.LoadConfig(); err != nil {
		log.Fatal("Load config: ", err)
	}

	if err := viper.Unmarshal(config); err != nil {
		log.Fatal("Load config: ", err)
	}

	managerClient := benchmark.NewManagerClient(config)
	benchmark.LoadManagerClient(managerClient)

	// _ = boomerClient.LoadTask(&tasks,benchmark.CONNECT, 1)
	// _ = boomerClient.LoadTask(&tasks,benchmark.GET, 1)
	// _ = boomerClient.LoadTask(&tasks,benchmark.REMOVE, 1)
	// _ = boomerClient.LoadTask(&tasks,benchmark.EXIST, 1)
	// _ = boomerClient.LoadTask(&tasks,benchmark.CLOSE, 1)

	cmd.Execute()
}

func main() {
	run()
}
