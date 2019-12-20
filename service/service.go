package service

import (
	"context"

	api "github.com/1612898/zpkvservice/service/api/proto"
	"github.com/1612898/zpkvservice/utils/serverUtils"
)

type ZPKVServiceImpl struct {
	ServiceUtils serverUtils.IServiceUtils
}

func (server *ZPKVServiceImpl) ConnectZPKV(ctx context.Context, req *api.ConnectionRequest) (*api.MessageResponse, error) {
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

func (server *ZPKVServiceImpl) CloseConnectionZPKV(ctx context.Context, req *api.CloseConnectionRequest) (*api.MessageResponse, error) {
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

func (server *ZPKVServiceImpl) GetKV(ctx context.Context, req *api.GetKVRequest) (*api.GetKVResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}

	store, err := server.ServiceUtils.GetStore(id)
	if err != nil {
		status := api.Status{Error: err.Error(), Code: 0}
		res := api.GetKVResponse{Status: &status, Value: ""}
		return &res, nil
	}

	key := req.GetKey()
	value, err := store.Get(key)
	if err != nil {
		status := api.Status{Error: err.Error(), Code: 404}
		res := api.GetKVResponse{Status: &status, Value: value}
		return &res, nil
	}
	status := api.Status{Error: "", Code: 1}
	var res = api.GetKVResponse{Status: &status, Value: value}
	return &res, nil
}

func (server *ZPKVServiceImpl) SetKV(ctx context.Context, req *api.SetKVRequest) (*api.MessageResponse, error) {
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

func (server *ZPKVServiceImpl) RemoveKV(ctx context.Context, req *api.RemoveKVRequest) (*api.RemoveKVResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}
	store, err := server.ServiceUtils.GetStore(id)
	if err != nil {
		status := api.Status{Error: err.Error(), Code: 0}
		res := api.RemoveKVResponse{Status: &status, Check: false}
		return &res, nil
	}

	key := req.GetKey()
	var res api.RemoveKVResponse
	res.Check = store.Remove(key)
	res.Status = &api.Status{Code: 1, Error: ""}
	return &res, nil
}

func (server *ZPKVServiceImpl) ExistKV(ctx context.Context, req *api.ExistKVRequest) (*api.ExistKVResponse, error) {
	id, err := server.ServiceUtils.GetConnID(ctx)
	if err != nil {
		return nil, err
	}
	store, err := server.ServiceUtils.GetStore(id)
	if err != nil {
		status := api.Status{Error: err.Error(), Code: 0}
		res := api.ExistKVResponse{Status: &status, Check: false}
		return &res, nil
	}

	key := req.GetKey()

	existed := store.Exist(key)

	status := api.Status{Error: "", Code: 1}
	res := api.ExistKVResponse{Check: existed, Status: &status}
	return &res, nil
}
