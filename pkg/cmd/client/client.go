package client

import (
	"bufio"
	"fmt"
	"os"
	"strconv"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/viper"
	"github.com/zalopay-oss/GodBee/configs"
	service "github.com/zalopay-oss/GodBee/pkg/service/api/proto"
	utils "github.com/zalopay-oss/GodBee/pkg/utils/clientUtils"
	"google.golang.org/grpc"
)

func RunClient() error {
	config := &configs.ServiceConfig{}
	err := configs.LoadConfig()
	if err != nil {
		return err
	}
	if err = viper.Unmarshal(config); err != nil {
		return err
	}

	port := strconv.Itoa(config.GRPCPort)
	host := config.GRPCHost
	//
	log.Info("Connect to GodBee port " + port + " ...")
	conn, err := grpc.Dial(host+":"+port, grpc.WithInsecure())
	if err != nil {
		log.Info("xx")
		return err
	}
	defer conn.Close()
	client := service.NewKeyValueStoreServiceClient(conn)
	_ = client

	for true {
		fmt.Print("GodBee > ")
		reader := bufio.NewReader(os.Stdin)
		text, _ := reader.ReadString('\n')
		if !utils.ExecuteCommand(client, text) {
			break
		}
	}
	return nil
}
