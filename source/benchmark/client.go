package benchmark

import (
	"context"
	"fmt"
	"github.com/1612898/zpkvservice/configs"
	proto "github.com/1612898/zpkvservice/service/api/proto"
	"time"

	grpcpool "github.com/processout/grpc-go-pool"
	log "github.com/sirupsen/logrus"
	"google.golang.org/grpc"
)

type ManagerClient struct {
	pool   *grpcpool.Pool
	config *configs.ServiceConfig
}

type Client struct {
	client proto.KeyValueStoreServiceClient
	conn   *grpcpool.ClientConn
	ctx    context.Context
}

func NewManagerClient(config *configs.ServiceConfig) *ManagerClient {
	manager := &ManagerClient{
		config: config,
	}

	p, err := grpcpool.New(manager.NewFactoryClient, config.PoolSize, config.PoolSize, time.Duration(config.TimeOut)*time.Second)
	if err != nil {
		log.Fatal("Do not init connection pool")
	}

	manager.pool = p

	return manager
}

func (manager *ManagerClient) NewFactoryClient() (*grpc.ClientConn, error) {
	address := fmt.Sprintf("%s:%d", manager.config.GRPCHost, manager.config.GRPCPort)

	conn, err := grpc.Dial(address, grpc.WithInsecure())
	if err != nil {
		log.WithFields(log.Fields{"Error": err}).Fatal("Did not connect server")
		return nil, err
	}

	return conn, nil
}

func (manager *ManagerClient) NewClient() *Client {
	ctx := context.Background()

	conn, _ := manager.pool.Get(ctx)

	return &Client{
		client: proto.NewKeyValueStoreServiceClient(conn.ClientConn),
		conn:   conn,
		ctx:    ctx,
	}
}

func (manager *ManagerClient) ClosePool() {
	manager.pool.Close()
}

func (c *Client) getTimestamp() int64 {
	return time.Now().UnixNano() / int64(time.Millisecond)
}

func (c *Client) Connect(request proto.ConnectionRequest) (*proto.MessageResponse, error) {
	return c.client.Connect(c.ctx, &request)
}

func (c *Client) Disconnect(request proto.CloseConnectionRequest) (*proto.MessageResponse, error) {
	return c.client.CloseConnection(c.ctx, &request)
}

func (c *Client) Get(request proto.GetRequest) (*proto.GetResponse, error) {
	return c.client.Get(c.ctx, &request)
}

func (c *Client) Set(request proto.SetRequest) (*proto.MessageResponse, error) {
	return c.client.Set(c.ctx, &request)
}

func (c *Client) Remove(request proto.RemoveRequest) (*proto.RemoveResponse, error) {
	return c.client.Remove(c.ctx, &request)
}
func (c *Client) Exist(request proto.ExistRequest) (*proto.ExistResponse, error) {
	return c.client.Exist(c.ctx, &request)
}


//Close close conn
func (c *Client) Close() error {
	return c.conn.Close()
}