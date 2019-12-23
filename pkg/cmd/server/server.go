package server

import (
	"net"
	"os"
	"os/signal"
	"strconv"
	"syscall"

	log "github.com/sirupsen/logrus"
	"github.com/spf13/viper"
	"github.com/zalopay-oss/GodBee/configs"
	"github.com/zalopay-oss/GodBee/pkg/service"
	api "github.com/zalopay-oss/GodBee/pkg/service/api/proto"
	"github.com/zalopay-oss/GodBee/pkg/utils/serverUtils"
	"google.golang.org/grpc"
)

func RunServer() error {
	path := "./data"
	if _, err := os.Stat(path); os.IsNotExist(err) {
		os.Mkdir(path, 0777)
	}

	config := &configs.ServiceConfig{}

	_ = configs.LoadConfig()

	if err := viper.Unmarshal(config); err != nil {
		log.Fatal("Load config: ", err)
	}
	serverUtils.InitStore()

	c := make(chan os.Signal)
	signal.Notify(c, os.Interrupt, syscall.SIGTERM)
	go func() {
		<-c
		log.Info("Shutting down server...")
		serverUtils.RemoveStore()
		os.Exit(1)
	}()

	grpcServer := grpc.NewServer()
	port := strconv.Itoa(config.GRPCPort)
	server := service.ServiceImpl{ServiceUtils: serverUtils.NewServiceUtils()}
	api.RegisterKeyValueStoreServiceServer(grpcServer, &server)
	lis, err := net.Listen("tcp", ":"+port)
	if err != nil {
		//log.Fatal(err)
		return err
	}
	log.Info("Start Godbee service port " + port + " ...")

	return grpcServer.Serve(lis)
}
