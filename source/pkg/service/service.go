package service

import (
	"context"
	"github.com/1612898/zpkvservice/pkg/service/api/proto"

	"github.com/1612898/zpkvservice/pkg/utils/serverUtils"
)

type ServiceImpl struct {
	ServiceUtils serverUtils.IServiceUtils
}

func (server *ServiceImpl) Connect(ctx context.Context, req *service.ConnectionRequest) (*service.MessageResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}
	if serverUtils.IsValidType(req.GetType()) {
		server.ServiceUtils.UpdateMap(id, req.GetType())
		res := &service.Status{Code: 1, Error: ""}
		return &service.MessageResponse{Status: res}, nil
	} else {
		res := &service.Status{Code: 0, Error: "Invalid Store Type"}
		return &service.MessageResponse{Status: res}, nil
	}
}

func (server *ServiceImpl) CloseConnection(ctx context.Context, req *service.CloseConnectionRequest) (*service.MessageResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}
	_, ok := server.ServiceUtils.GetSessionStoreType(id)
	if !ok {
		res := &service.Status{Code: 0, Error: "Haven't connected to any storage yet. Check your connection again."}
		return &service.MessageResponse{Status: res}, nil
	}
	server.ServiceUtils.DelSessionStoreType(id)
	res := &service.Status{Code: 1, Error: ""}
	return &service.MessageResponse{Status: res}, nil
}

func (server *ServiceImpl) Get(ctx context.Context, req *service.GetRequest) (*service.GetResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}

	store, err := server.ServiceUtils.GetStore(id)
	if err != nil {
		status := service.Status{Error: err.Error(), Code: 0}
		res := service.GetResponse{Status: &status, Value: ""}
		return &res, nil
	}

	key := req.GetKey()
	value, err := store.Get(key)
	if err != nil {
		status := service.Status{Error: err.Error(), Code: 404}
		res := service.GetResponse{Status: &status, Value: value}
		return &res, nil
	}
	status := service.Status{Error: "", Code: 1}
	var res = service.GetResponse{Status: &status, Value: value}
	return &res, nil
}

func (server *ServiceImpl) Set(ctx context.Context, req *service.SetRequest) (*service.MessageResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}
	store, err := server.ServiceUtils.GetStore(id)
	if err != nil {
		res := &service.Status{Code: 0, Error: err.Error()}
		return &service.MessageResponse{Status: res}, nil
	}

	key := req.GetKey()
	value := req.GetValue()

	err = store.Set(key, value)
	if err!=nil{
		res := &service.Status{Code: 0, Error: err.Error()}
		return &service.MessageResponse{Status: res}, nil
	}
	res := &service.Status{Code: 1, Error: ""}
	return &service.MessageResponse{Status: res}, nil
}

func (server *ServiceImpl) Remove(ctx context.Context, req *service.RemoveRequest) (*service.RemoveResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}
	store, err := server.ServiceUtils.GetStore(id)
	if err != nil {
		status := service.Status{Error: err.Error(), Code: 0}
		res := service.RemoveResponse{Status: &status, Check: false}
		return &res, nil
	}

	key := req.GetKey()
	var res service.RemoveResponse
	res.Check = store.Remove(key)
	res.Status = &service.Status{Code: 1, Error: ""}
	return &res, nil
}

func (server *ServiceImpl) Exist(ctx context.Context, req *service.ExistRequest) (*service.ExistResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}
	store, err := server.ServiceUtils.GetStore(id)
	if err != nil {
		status := service.Status{Error: err.Error(), Code: 0}
		res := service.ExistResponse{Status: &status, Check: false}
		return &res, nil
	}

	key := req.GetKey()

	existed := store.Exist(key)

	status := service.Status{Error: "", Code: 1}
	res := service.ExistResponse{Check: existed, Status: &status}
	return &res, nil
}
