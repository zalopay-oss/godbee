package main

import (
	"github.com/1612898/zpkvservice/configs"
	"github.com/1612898/zpkvservice/service"
	api "github.com/1612898/zpkvservice/service/api/proto"
	"github.com/1612898/zpkvservice/utils/serverUtils"
	log "github.com/sirupsen/logrus"
	"github.com/spf13/viper"
	"google.golang.org/grpc"
	"net"
	"os"
	"os/signal"
	"strconv"
	"syscall"
)

func main() {
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
	server := service.ZPKVServiceImpl{ServiceUtils: serverUtils.NewServiceUtils()}
	api.RegisterKeyValueStoreServiceServer(grpcServer, &server)
	lis, err := net.Listen("tcp", ":"+port)
	if err != nil {
		log.Fatal(err)
	}
	log.Info("Start KVZP service port " + port + " ...")
	grpcServer.Serve(lis)

}