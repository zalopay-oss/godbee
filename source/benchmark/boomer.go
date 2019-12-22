package benchmark

import (
	"github.com/1612898/zpkvservice/pkg/service/api/proto"
	boomer "github.com/myzhan/boomer"
	log "github.com/sirupsen/logrus"
	"math/rand"
	"strconv"
)

const CONNECT = "connect"
const SET = "set"
const GET = "get"
const REMOVE = "remove"
const EXIST = "exist"
const CLOSE = "close"

var managerClient *ManagerClient

type BoomerClient struct {
	managerClient *ManagerClient
	client *Client
}

var boomerClient *BoomerClient

func LoadManagerClient(manager *ManagerClient) {
	boomerClient = &BoomerClient{}
	boomerClient.managerClient = manager
	boomerClient.client = boomerClient.managerClient.NewClient()
}

func GetBoomerClient() *BoomerClient {
	return boomerClient
}

func (boomerClient *BoomerClient) Connect() {
	//client := managerClient.NewClient()
	client := boomerClient.client
	start := boomer.Now()
	storeType := service.StoreType_BPlusTreeDisk
	messageResponse, err := client.Connect(service.ConnectionRequest{Type: storeType})
	elapsed := boomer.Now() - start

	client.Close()

	if err != nil {
		log.Error("Connect error: ", err)
		boomer.RecordFailure("tcp", "Connect error", elapsed, err.Error())
	} else {
		boomer.RecordSuccess("tcp", "Connect", elapsed, int64(messageResponse.XXX_Size()))
	}
}

func (boomerClient *BoomerClient) Set() {
	// client := managerClient.NewClient()
	client := boomerClient.client
	start := boomer.Now()
	kv := strconv.Itoa(rand.Int())
	messageResponse, err := client.Set(service.SetRequest{Key: kv,Value:kv})
	elapsed := boomer.Now() - start

	client.Close()

	if err != nil {
		log.Error("Set error: ", err)
		boomer.RecordFailure("tcp", "Set error", elapsed, err.Error())
	} else {
		boomer.RecordSuccess("tcp", "Set", elapsed, int64(messageResponse.XXX_Size()))
	}
}

func (boomerClient *BoomerClient) Get() {
	// client := managerClient.NewClient()
	client := boomerClient.client
	start := boomer.Now()
	kv := strconv.Itoa(rand.Int())
	kvResponse, err := client.Get(service.GetRequest{Key: kv})
	elapsed := boomer.Now() - start
	client.Close()
	if err != nil {
		log.Error("Get error: ", err)
		boomer.RecordFailure("tcp", "Get error", elapsed, err.Error())
	} else {
		boomer.RecordSuccess("tcp", "Get", elapsed, int64(kvResponse.XXX_Size()))
	}
}

func (boomerClient *BoomerClient) Remove() {
	// client := managerClient.NewClient()
	client := boomerClient.client
	start := boomer.Now()
	kv := strconv.Itoa(rand.Int())
	kvResponse, err := client.Remove(service.RemoveRequest{Key: kv})
	elapsed := boomer.Now() - start

	client.Close()

	if err != nil {
		log.Error("Remove error: ", err)
		boomer.RecordFailure("tcp", "Remove error", elapsed, err.Error())
	} else {
		boomer.RecordSuccess("tcp", "Remove", elapsed, int64(kvResponse.XXX_Size()))
	}
}

func (boomerClient *BoomerClient) Exist() {
	// client := managerClient.NewClient()
	client := boomerClient.client
	start := boomer.Now()
	kv := strconv.Itoa(rand.Int())
	kvResponse, err := client.Exist(service.ExistRequest{Key: kv})
	elapsed := boomer.Now() - start

	client.Close()

	if err != nil {
		log.Error("Exist error: ", err)
		boomer.RecordFailure("tcp", "Exist error", elapsed, err.Error())
	} else {
		boomer.RecordSuccess("tcp", "Exist", elapsed, int64(kvResponse.XXX_Size()))
	}
}

func (boomerClient *BoomerClient) Close() {
	client := managerClient.NewClient()
	start := boomer.Now()
	kvResponse, err := client.Disconnect(service.CloseConnectionRequest{})
	elapsed := boomer.Now() - start

	client.Close()

	if err != nil {
		log.Error("Exist error: ", err)
		boomer.RecordFailure("tcp", "Disconnect error", elapsed, err.Error())
	} else {
		boomer.RecordSuccess("tcp", "Disconnect", elapsed, int64(kvResponse.XXX_Size()))
	}
}



func (boomerClient *BoomerClient) LoadTask(taskList *[]*boomer.Task, nameTask string, weight int) (error) {
	taskPing := boomerClient.createTask(nameTask, weight)
	*taskList = append(*taskList, taskPing)

	return nil
}

func (boomerClient *BoomerClient) getFuncTask(nameTask string) func() {
	switch nameTask {
	case CONNECT:
		return boomerClient.Connect
	case GET:
		return boomerClient.Get
	case SET:
		return boomerClient.Set
	case EXIST:
		return boomerClient.Exist
	case REMOVE:
		return boomerClient.Remove
	case CLOSE:
		return boomerClient.Close
	}

	return nil
}

func (boomerClient *BoomerClient) createTask(nameTask string, weight int) *boomer.Task {
	return &boomer.Task{
		Name:   nameTask,
		Weight: weight,
		Fn:     boomerClient.getFuncTask(nameTask),
	}
}

func (boomerClient *BoomerClient) RunTask(tasks []*boomer.Task) {
	boomer.Events.Subscribe("boomer:hatch", func(workers int, hatchRate float64) {
		log.Info("The master asks me to spawn ", workers, " goroutines with a hatch rate of", int(hatchRate), "per second.")
	})

	boomer.Events.Subscribe("boomer:quit", func() {
		log.Info("Boomer is quitting now.")
		boomerClient.managerClient.ClosePool()
	})
	boomer.Run(tasks...)
}