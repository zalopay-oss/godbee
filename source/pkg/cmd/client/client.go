package client

import (
	"bufio"
	"fmt"
	"github.com/1612898/zpkvservice/configs"
	service "github.com/1612898/zpkvservice/pkg/service/api/proto"
	utils "github.com/1612898/zpkvservice/pkg/utils/clientUtils"
	log "github.com/sirupsen/logrus"
	"github.com/spf13/viper"
	"google.golang.org/grpc"
	"os"
	"strconv"
)

func RunClient() error{
	config := &configs.ServiceConfig{}
	err := configs.LoadConfig()
	if err!=nil {
		return err
	}
	if err = viper.Unmarshal(config); err != nil {
		return err
	}

	port := strconv.Itoa(config.GRPCPort)
	host := config.GRPCHost
	//
	log.Info("Connect to KVZP service port " + port + " ...")
	conn, err := grpc.Dial(host+":"+port, grpc.WithInsecure())
	if err != nil {
		log.Info("xx")
		return err
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
	return nil
}
