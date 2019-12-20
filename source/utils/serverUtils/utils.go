package serverUtils

import (
	"context"
	"errors"
	"strings"
	"sync"

	"github.com/1612898/zpkvservice/repository"
	service "github.com/1612898/zpkvservice/service/api/proto"
	"google.golang.org/grpc/peer"
)

func IsValidType(value service.StoreType) bool {
	switch value {
	case service.StoreType_BTreeDisk, service.StoreType_BPlusTreeDisk:
		return true
	}
	return false
}

type IServiceUtils interface {
	GetConnID(context.Context) (string, error)
	GetStore(id string) (repository.GStore, error)
	UpdateMap(id string, getType service.StoreType)
	GetSessionStoreType(id string) (service.StoreType, bool)
	DelSessionStoreType(id string)
}

type Map struct {
	sync.Mutex
	m map[string]service.StoreType
}

type ServiceUtils struct {
	sessionMap *Map
}

func NewServiceUtils() *ServiceUtils {
	res := ServiceUtils{}
	res.sessionMap = &Map{m: map[string]service.StoreType{}}
	return &res
}

func (ServiceUtils) GetConnID(ctx context.Context) (string, error) {
	p, ok := peer.FromContext(ctx)
	if !ok {
		return "", errors.New("can't get context")
	}

	s := strings.Split(p.Addr.String(), ":")

	return s[len(s)-1], nil
}

func InitStore() {
	repository.Init()
}

func RemoveStore() {
	repository.Free()
}

func (s ServiceUtils) GetStore(id string) (repository.GStore, error) {
	var err error
	s.sessionMap.Lock()
	sType, ok := s.sessionMap.m[id]
	s.sessionMap.Unlock()
	if !ok {
		err := errors.New("haven't connected to any storage yet, check your connection again")
		return nil, err
	}
	var store repository.GStore

	switch sType {
	case service.StoreType_BPlusTreeDisk:
		store, err = repository.GetInstanceBPlus()
	case service.StoreType_BTreeDisk:
		store, err = repository.GetInstanceB()
	default:
		err = errors.New("store type not found")
	}
	if err != nil {
		return nil, err
	}
	return store, nil
}

func (s ServiceUtils) UpdateMap(id string, storeType service.StoreType) {
	s.sessionMap.Lock()
	s.sessionMap.m[id] = storeType
	s.sessionMap.Unlock()
}

func (s ServiceUtils) GetSessionStoreType(id string) (service.StoreType, bool) {
	s.sessionMap.Lock()
	res, ok := s.sessionMap.m[id]
	s.sessionMap.Unlock()
	return res, ok
}

func (s ServiceUtils) DelSessionStoreType(id string) {
	s.sessionMap.Lock()
	delete(s.sessionMap.m, id)
	s.sessionMap.Unlock()
}
