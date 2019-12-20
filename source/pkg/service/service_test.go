package service

import (
	"context"
	"errors"
	"github.com/1612898/zpkvservice/pkg/service/api/proto"
	"github.com/1612898/zpkvservice/pkg/utils/serverUtils"
	"github.com/1612898/zpkvservice/repository"
	"github.com/stretchr/testify/mock"
	"testing"
)

type serviceUtilsMock struct {
	mock.Mock
	serviceUtil serverUtils.ServiceUtils
}

func newServiceUtilsMock() *serviceUtilsMock {
	return &serviceUtilsMock{serviceUtil: *serverUtils.NewServiceUtils()}
}

type gStoreMock struct {
	mock.Mock
}

func (gS gStoreMock) Free() {
}
func (gS gStoreMock) Get(k string) (string, error) {
	args := gS.Called(k)
	return args.String(0),args.Error(1)
}
func (gS gStoreMock) Set(k string, v string) error {
	return nil
}
func (gS gStoreMock) Remove(k string) bool {
	args := gS.Called(k)
	return args.Bool(0)
}
func (gS gStoreMock) Exist(k string) bool {
	args := gS.Called(k)
	return args.Bool(0)
}


func (s serviceUtilsMock)GetConnID(ctx context.Context) (string, error) {
	args := s.Called(ctx)
	return args.String(0),args.Error(1)
}
func (s serviceUtilsMock) GetStore(id string) (repository.GStore,error){
	_,ok := s.serviceUtil.GetSessionStoreType(id)
	if !ok{
		return nil,errors.New("Invalid type")
	}
	args := s.Called(id)
	k := args.Get(0).(*gStoreMock)
	return k,args.Error(1)
}
func (s serviceUtilsMock) UpdateMap(id string, storeType service.StoreType) {
	s.serviceUtil.UpdateMap(id,storeType)
}
func (s serviceUtilsMock) GetSessionStoreType(id string) (service.StoreType,bool) {
	return	s.serviceUtil.GetSessionStoreType(id)
}

func (s serviceUtilsMock) DelSessionStoreType(id string){
	s.serviceUtil.DelSessionStoreType(id)
}

func TestZPKVServiceImpl_Connect(t *testing.T) {
	ctx := context.Background()
	mockService := newServiceUtilsMock()
	(mockService).On("GetConnID",ctx).Return("123",nil)
	var server = &ServiceImpl{ServiceUtils: *mockService}

	req := service.ConnectionRequest{Type: service.StoreType_BPlusTreeDisk}
	res,err := server.Connect(ctx,&req)
	if err!=nil{
		t.Error("Connect failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode()!=1 {
		t.Error("Connect failed, got internal Error ",res.GetStatus().GetError())
	}

	req = service.ConnectionRequest{Type: service.StoreType_BTreeDisk}
	res,err = server.Connect(ctx,&req)
	if err!=nil{
		t.Error("Connect failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode()!=1 {
		t.Error("Connect failed, got internal Error ",res.GetStatus().GetError())
	}

	req = service.ConnectionRequest{Type: 4}
	res,err = server.Connect(ctx,&req)
	if err!=nil{
		t.Error("Connect failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("Connect failed, should get Error Store type not Available " + res.GetStatus().GetError())
	}

	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("",errors.New("error"))
	server = &ServiceImpl{ServiceUtils: *mockService}
	req = service.ConnectionRequest{}
	res,err = server.Connect(ctx,&req)
	if err==nil{
		t.Error("CloseConnectionfailed, Should got Error")
	}
}

func TestZPKVServiceImpl_CloseConnection(t *testing.T) {
	ctx := context.Background()
	mockService := newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("123",nil)
	var server = &ServiceImpl{ServiceUtils: mockService}
	req := service.CloseConnectionRequest{}

	/*
	Case Haven't connect to any storage
	 */
	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("123",nil)
	res,err := server.CloseConnection(ctx,&req)
	if err!=nil{
		t.Error("CloseConnectionfailed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("CloseConnectionfailed, should get Error Store type not Available ")
	}

	/*
		Case Connect to B-Storage
	*/
	reqC := service.ConnectionRequest{Type: service.StoreType_BTreeDisk}
	_,_ = server.Connect(ctx,&reqC)
	req = service.CloseConnectionRequest{}
	res,err = server.CloseConnection(ctx,&req)
	if err!=nil{
		t.Error("CloseConnectionfailed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("CloseConnectionfailed, got internal Error " + res.GetStatus().GetError())
	}


	/*
		Case Connect to B+Storage
	*/
	reqC = service.ConnectionRequest{Type: service.StoreType_BTreeDisk}
	_,_ = server.Connect(ctx,&reqC)
	req = service.CloseConnectionRequest{}
	res,err = server.CloseConnection(ctx,&req)
	if err!=nil{
		t.Error("CloseConnectionfailed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("CloseConnectionfailed, got internal Error " + res.GetStatus().GetError())
	}

	/*
		Case Can't get conn ID
	*/
	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("",errors.New("error"))
	server = &ServiceImpl{ServiceUtils: mockService}
	req = service.CloseConnectionRequest{}
	res,err = server.CloseConnection(ctx,&req)
	if err==nil{
		t.Error("CloseConnectionfailed, Should got Error")
	}
}

func TestZPKVServiceImpl_Set(t *testing.T) {
	ctx := context.Background()
	mockService := newServiceUtilsMock()
	gStore := &gStoreMock{}

	mockService.On("GetStore",mock.Anything).Return(gStore,nil)
	mockService.On("GetConnID",mock.Anything).Return("1223",nil)
	var server = &ServiceImpl{ServiceUtils: mockService}
	req := service.SetRequest{Key: "abc",Value:"abd"}

	/*
		Case Haven't connect to any storage
	*/
	res,err := server.Set(ctx,&req)
	if err!=nil{
		t.Error("Set failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("Set failed, should get Error Store type not Available ")
	}
	/*
		Case Connect to B-Storage
	*/
	reqC := service.ConnectionRequest{Type: service.StoreType_BTreeDisk}
	_,_ = server.Connect(ctx,&reqC)
	res,err = server.Set(ctx,&req)
	if err!=nil{
		t.Error("Set failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("Set failed, got internal Error " + res.GetStatus().GetError())
	}

	/*
		Case Connect to B+Storage
	*/
	reqC = service.ConnectionRequest{Type: service.StoreType_BPlusTreeDisk}
	_,_ = server.Connect(ctx,&reqC)
	res,err = server.Set(ctx,&req)
	if err!=nil{
		t.Error("Set failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("Set failed, got internal Error " + res.GetStatus().GetError())
	}

	/*
		Case Can't get conn ID
	*/
	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("",errors.New("error"))
	server = &ServiceImpl{ServiceUtils: mockService}
	req = service.SetRequest{Key: "abc",Value:"abd"}
	res,err = server.Set(ctx,&req)
	if err==nil{
		t.Error("Set failed, Should got Error")
	}
}

func TestZPKVServiceImpl_Get(t *testing.T) {
	ctx := context.Background()
	mockService := newServiceUtilsMock()
	gStore := &gStoreMock{}
	gStore.On("Get","getNULL").Return("",errors.New("Invalid Key"))
	gStore.On("Get","abc").Return("abd",nil)

	mockService.On("GetStore",mock.Anything).Return(gStore,nil)
	mockService.On("GetConnID",mock.Anything).Return("1223",nil)
	var server = &ServiceImpl{ServiceUtils: mockService}
	req := service.GetRequest{Key: "getNULL"}

	/*
		Case Haven't connect to any storage
	*/
	res,err := server.Get(ctx,&req)
	if err!=nil{
		t.Error("Get failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("Get failed, should get Error Store type not Available " + res.GetStatus().GetError())
	}

	/*
		Case NOT FOUND Connect to B+Storage
	*/
	reqC := service.ConnectionRequest{Type: service.StoreType_BPlusTreeDisk}
	_,_ = server.Connect(ctx,&reqC)
	res,err = server.Get(ctx,&req)
	if err!=nil{
		t.Error("Get failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("Get failed, should got internal Error 404 ")
	}

	/*
		Case NOT FOUND Connect to B-Storage
	*/
	reqC = service.ConnectionRequest{Type: service.StoreType_BTreeDisk}
	_,_ = server.Connect(ctx,&reqC)
	res,err = server.Get(ctx,&req)
	if err!=nil{
		t.Error("Get failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("Get failed, should got internal Error 404 ")
	}

	/*
		Normal case
	*/
	req.Key = "abc"
	res,err = server.Get(ctx,&req)
	if err!=nil{
		t.Error("Get failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("Get failed, got internal Error "+res.GetStatus().GetError())
	}

	/*
		Case Can't get conn ID
	*/
	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("",errors.New("error"))
	server = &ServiceImpl{ServiceUtils: mockService}
	req = service.GetRequest{Key: "abc"}
	res,err = server.Get(ctx,&req)
	if err==nil{
		t.Error("Get failed, Should got Error")
	}

}

func TestZPKVServiceImpl_Remove(t *testing.T) {
	ctx := context.Background()
	mockService := newServiceUtilsMock()
	gStore := &gStoreMock{}
	gStore.On("Remove","removedNULL").Return(false)
	gStore.On("Remove","abc").Return(true)

	mockService.On("GetStore",mock.Anything).Return(gStore,nil)
	mockService.On("GetConnID",mock.Anything).Return("1223",nil)
	var server = &ServiceImpl{ServiceUtils: mockService}

	req := service.RemoveRequest{Key: "removedNULL"}

	res,err := server.Remove(ctx,&req)
	if err!=nil{
		t.Error("Remove failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("Remove failed, should get Error Store type not Available " + res.GetStatus().GetError())
	}

	reqC := service.ConnectionRequest{Type: service.StoreType_BPlusTreeDisk}
	_,_ = server.Connect(ctx,&reqC)
	res,err = server.Remove(ctx,&req)
	if err!=nil{
		t.Error("Remove failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("Remove failed, got internal error: "+res.GetStatus().GetError())
	} else if res.GetCheck(){
		t.Error("Remove failed, Nothing to remove")
	}

	reqC = service.ConnectionRequest{Type: service.StoreType_BTreeDisk}
	_,_ = server.Connect(ctx,&reqC)
	res,err = server.Remove(ctx,&req)
	if err!=nil{
		t.Error("Remove failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("Remove failed, got internal error: "+res.GetStatus().GetError())
	} else if res.GetCheck(){
		t.Error("Remove failed, Nothing to remove")
	}

	req.Key="abc"
	res,err = server.Remove(ctx,&req)
	if err!=nil{
		t.Error("Remove failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("Remove failed, got internal Error "+res.GetStatus().GetError())
	} else if !res.GetCheck() {
		t.Error("Remove failed, Can't remove")
	}

	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("",errors.New("error"))
	server = &ServiceImpl{ServiceUtils: mockService}
	req = service.RemoveRequest{Key: "abc"}
	res,err = server.Remove(ctx,&req)
	if err==nil{
		t.Error("Remove failed, Should got Error")
	}

}

func TestZPKVServiceImpl_Exist(t *testing.T) {
	ctx := context.Background()

	mockService := newServiceUtilsMock()
	gStore := &gStoreMock{}
	gStore.On("Exist","existNULL").Return(false)
	gStore.On("Exist","abc").Return(true)

	mockService.On("GetStore",mock.Anything).Return(gStore,nil)
	mockService.On("GetConnID",mock.Anything).Return("1223",nil)
	var server = &ServiceImpl{ServiceUtils: mockService}

	req := service.ExistRequest{Key: "existNULL"}

	res,err := server.Exist(ctx,&req)
	if err!=nil{
		t.Error("Exist failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("Exist failed, should get Error Store type not Available " + res.GetStatus().GetError())
	}

	reqC := service.ConnectionRequest{Type: service.StoreType_BPlusTreeDisk}
	_,_ = server.Connect(ctx,&reqC)
	res,err = server.Exist(ctx,&req)
	if err!=nil{
		t.Error("Exist failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("Exist failed, got internal error: "+res.GetStatus().GetError())
	} else if res.GetCheck(){
		t.Error("Exist failed, Nothing to remove")
	}


	reqC = service.ConnectionRequest{Type: service.StoreType_BTreeDisk}
	_,_ = server.Connect(ctx,&reqC)
	res,err = server.Exist(ctx,&req)
	if err!=nil{
		t.Error("Exist failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("Exist failed, got internal error: "+res.GetStatus().GetError())
	} else if res.GetCheck(){
		t.Error("Exist failed, Nothing to remove")
	}

	req.Key="abc"
	res,err = server.Exist(ctx,&req)
	if err!=nil{
		t.Error("Exist failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("Exist failed, got internal Error "+res.GetStatus().GetError())
	} else if !res.GetCheck() {
		t.Error("Exist failed, Can't remove")
	}


	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("",errors.New("error"))
	server = &ServiceImpl{ServiceUtils: mockService}
	req = service.ExistRequest{Key: "abc"}
	res,err = server.Exist(ctx,&req)
	if err==nil{
		t.Error("Exist failed, Should got Error")
	}

}
