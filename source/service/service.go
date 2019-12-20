package service

import (
	"context"

	api "github.com/1612898/zpkvservice/service/api/proto"
	"github.com/1612898/zpkvservice/utils/serverUtils"
)

type ServiceImpl struct {
	ServiceUtils serverUtils.IServiceUtils
}

func (server *ServiceImpl) Connect(ctx context.Context, req *api.ConnectionRequest) (*api.MessageResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}
	if serverUtils.IsValidType(req.GetType()) {
		server.ServiceUtils.UpdateMap(id, req.GetType())
		res := &api.Status{Code: 1, Error: ""}
		return &api.MessageResponse{Status: res}, nil
	} else {
		res := &api.Status{Code: 0, Error: "Invalid Store Type"}
		return &api.MessageResponse{Status: res}, nil
	}
}

func (server *ServiceImpl) CloseConnection(ctx context.Context, req *api.CloseConnectionRequest) (*api.MessageResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}
	_, ok := server.ServiceUtils.GetSessionStoreType(id)
	if !ok {
		res := &api.Status{Code: 0, Error: "Haven't connected to any storage yet. Check your connection again."}
		return &api.MessageResponse{Status: res}, nil
	}
	server.ServiceUtils.DelSessionStoreType(id)
	res := &api.Status{Code: 1, Error: ""}
	return &api.MessageResponse{Status: res}, nil
}

func (server *ServiceImpl) Get(ctx context.Context, req *api.GetRequest) (*api.GetResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}

	store, err := server.ServiceUtils.GetStore(id)
	if err != nil {
		status := api.Status{Error: err.Error(), Code: 0}
		res := api.GetResponse{Status: &status, Value: ""}
		return &res, nil
	}

	key := req.GetKey()
	value, err := store.Get(key)
	if err != nil {
		status := api.Status{Error: err.Error(), Code: 404}
		res := api.GetResponse{Status: &status, Value: value}
		return &res, nil
	}
	status := api.Status{Error: "", Code: 1}
	var res = api.GetResponse{Status: &status, Value: value}
	return &res, nil
}

func (server *ServiceImpl) Set(ctx context.Context, req *api.SetRequest) (*api.MessageResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}
	store, err := server.ServiceUtils.GetStore(id)
	if err != nil {
		res := &api.Status{Code: 0, Error: err.Error()}
		return &api.MessageResponse{Status: res}, nil
	}

	key := req.GetKey()
	value := req.GetValue()

	err = store.Set(key, value)
	if err!=nil{
		res := &api.Status{Code: 0, Error: err.Error()}
		return &api.MessageResponse{Status: res}, nil
	}
	res := &api.Status{Code: 1, Error: ""}
	return &api.MessageResponse{Status: res}, nil
}

func (server *ServiceImpl) Remove(ctx context.Context, req *api.RemoveRequest) (*api.RemoveResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}
	store, err := server.ServiceUtils.GetStore(id)
	if err != nil {
		status := api.Status{Error: err.Error(), Code: 0}
		res := api.RemoveResponse{Status: &status, Check: false}
		return &res, nil
	}

	key := req.GetKey()
	var res api.RemoveResponse
	res.Check = store.Remove(key)
	res.Status = &api.Status{Code: 1, Error: ""}
	return &res, nil
}

func (server *ServiceImpl) Exist(ctx context.Context, req *api.ExistRequest) (*api.ExistResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}
	store, err := server.ServiceUtils.GetStore(id)
	if err != nil {
		status := api.Status{Error: err.Error(), Code: 0}
		res := api.ExistResponse{Status: &status, Check: false}
		return &res, nil
	}

	key := req.GetKey()

	existed := store.Exist(key)

	status := api.Status{Error: "", Code: 1}
	res := api.ExistResponse{Check: existed, Status: &status}
	return &res, nil
}
