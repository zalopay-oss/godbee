package main

import (
	"bufio"
	"fmt"
	"github.com/1612898/zpkvservice/configs"
	service "github.com/1612898/zpkvservice/service/api/proto"
	utils "github.com/1612898/zpkvservice/utils/clientUtils"
	log "github.com/sirupsen/logrus"
	"github.com/spf13/viper"
	"google.golang.org/grpc"
	"os"
	"strconv"
)

func main() {
	config := &configs.ServiceConfig{}
	configs.LoadConfig()
	if err := viper.Unmarshal(config); err != nil {
		log.Fatal("Load config: ", err)
	}

	port := strconv.Itoa(config.GRPCPort)
	host := config.GRPCHost
	//
	log.Info("Connect to KVZP service port " + port + " ...")
	conn, err := grpc.Dial(host+":"+port, grpc.WithInsecure())
	if err != nil {
		log.Fatal(err)
	}
	defer conn.Close()
	client := service.NewKeyValueStoreServiceClient(conn)
	_=client

	for true {
		fmt.Print("KVZP > ")
		reader := bufio.NewReader(os.Stdin)
		text, _ := reader.ReadString('\n')
		if !utils.ExecuteCommand(client, text) {
			break
		}
	}

}