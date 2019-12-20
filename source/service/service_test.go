package service

import (
	"context"
	"errors"
	"github.com/1612898/zpkvservice/repository"
	"github.com/1612898/zpkvservice/service/api/proto"
	"github.com/1612898/zpkvservice/utils/serverUtils"
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

func TestZPKVServiceImpl_ConnectZPKV(t *testing.T) {
	ctx := context.Background()
	mockService := newServiceUtilsMock()
	(mockService).On("GetConnID",ctx).Return("123",nil)
	var server = &ZPKVServiceImpl{ServiceUtils:*mockService}

	req := service.ConnectionRequest{Type:service.StoreType_BPlusTreeDisk}
	res,err := server.ConnectZPKV(ctx,&req)
	if err!=nil{
		t.Error("ConnectZPKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode()!=1 {
		t.Error("ConnectZPKV failed, got internal Error ",res.GetStatus().GetError())
	}

	req = service.ConnectionRequest{Type:service.StoreType_BTreeDisk}
	res,err = server.ConnectZPKV(ctx,&req)
	if err!=nil{
		t.Error("ConnectZPKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode()!=1 {
		t.Error("ConnectZPKV failed, got internal Error ",res.GetStatus().GetError())
	}

	req = service.ConnectionRequest{Type:4}
	res,err = server.ConnectZPKV(ctx,&req)
	if err!=nil{
		t.Error("ConnectZPKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("ConnectZPKV failed, should get Error Store type not Available " + res.GetStatus().GetError())
	}

	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("",errors.New("error"))
	server = &ZPKVServiceImpl{ServiceUtils:*mockService}
	req = service.ConnectionRequest{}
	res,err = server.ConnectZPKV(ctx,&req)
	if err==nil{
		t.Error("CloseConnectZPKV failed, Should got Error")
	}
}

func TestZPKVServiceImpl_CloseConnectionZPKV(t *testing.T) {
	ctx := context.Background()
	mockService := newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("123",nil)
	var server = &ZPKVServiceImpl{ServiceUtils:mockService}
	req := service.CloseConnectionRequest{}

	/*
	Case Haven't connect to any storage
	 */
	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("123",nil)
	res,err := server.CloseConnectionZPKV(ctx,&req)
	if err!=nil{
		t.Error("CloseConnectZPKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("CloseConnectZPKV failed, should get Error Store type not Available ")
	}

	/*
		Case Connect to B-Storage
	*/
	reqC := service.ConnectionRequest{Type:service.StoreType_BTreeDisk}
	_,_ = server.ConnectZPKV(ctx,&reqC)
	req = service.CloseConnectionRequest{}
	res,err = server.CloseConnectionZPKV(ctx,&req)
	if err!=nil{
		t.Error("CloseConnectZPKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("CloseConnectZPKV failed, got internal Error " + res.GetStatus().GetError())
	}


	/*
		Case Connect to B+Storage
	*/
	reqC = service.ConnectionRequest{Type:service.StoreType_BTreeDisk}
	_,_ = server.ConnectZPKV(ctx,&reqC)
	req = service.CloseConnectionRequest{}
	res,err = server.CloseConnectionZPKV(ctx,&req)
	if err!=nil{
		t.Error("CloseConnectZPKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("CloseConnectZPKV failed, got internal Error " + res.GetStatus().GetError())
	}

	/*
		Case Can't get conn ID
	*/
	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("",errors.New("error"))
	server = &ZPKVServiceImpl{ServiceUtils:mockService}
	req = service.CloseConnectionRequest{}
	res,err = server.CloseConnectionZPKV(ctx,&req)
	if err==nil{
		t.Error("CloseConnectZPKV failed, Should got Error")
	}
}

func TestZPKVServiceImpl_SetKV(t *testing.T) {
	ctx := context.Background()
	mockService := newServiceUtilsMock()
	gStore := &gStoreMock{}

	mockService.On("GetStore",mock.Anything).Return(gStore,nil)
	mockService.On("GetConnID",mock.Anything).Return("1223",nil)
	var server = &ZPKVServiceImpl{ServiceUtils:mockService}
	req := service.SetKVRequest{Key:"abc",Value:"abd"}

	/*
		Case Haven't connect to any storage
	*/
	res,err := server.SetKV(ctx,&req)
	if err!=nil{
		t.Error("SetKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("SetKV failed, should get Error Store type not Available ")
	}
	/*
		Case Connect to B-Storage
	*/
	reqC := service.ConnectionRequest{Type:service.StoreType_BTreeDisk}
	_,_ = server.ConnectZPKV(ctx,&reqC)
	res,err = server.SetKV(ctx,&req)
	if err!=nil{
		t.Error("SetKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("SetKV failed, got internal Error " + res.GetStatus().GetError())
	}

	/*
		Case Connect to B+Storage
	*/
	reqC = service.ConnectionRequest{Type:service.StoreType_BPlusTreeDisk}
	_,_ = server.ConnectZPKV(ctx,&reqC)
	res,err = server.SetKV(ctx,&req)
	if err!=nil{
		t.Error("SetKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("SetKV failed, got internal Error " + res.GetStatus().GetError())
	}

	/*
		Case Can't get conn ID
	*/
	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("",errors.New("error"))
	server = &ZPKVServiceImpl{ServiceUtils:mockService}
	req = service.SetKVRequest{Key:"abc",Value:"abd"}
	res,err = server.SetKV(ctx,&req)
	if err==nil{
		t.Error("SetKV failed, Should got Error")
	}
}

func TestZPKVServiceImpl_GetKV(t *testing.T) {
	ctx := context.Background()
	mockService := newServiceUtilsMock()
	gStore := &gStoreMock{}
	gStore.On("Get","getNULL").Return("",errors.New("Invalid Key"))
	gStore.On("Get","abc").Return("abd",nil)

	mockService.On("GetStore",mock.Anything).Return(gStore,nil)
	mockService.On("GetConnID",mock.Anything).Return("1223",nil)
	var server = &ZPKVServiceImpl{ServiceUtils:mockService}
	req := service.GetKVRequest{Key:"getNULL"}

	/*
		Case Haven't connect to any storage
	*/
	res,err := server.GetKV(ctx,&req)
	if err!=nil{
		t.Error("GetKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("GetKV failed, should get Error Store type not Available " + res.GetStatus().GetError())
	}

	/*
		Case NOT FOUND Connect to B+Storage
	*/
	reqC := service.ConnectionRequest{Type:service.StoreType_BPlusTreeDisk}
	_,_ = server.ConnectZPKV(ctx,&reqC)
	res,err = server.GetKV(ctx,&req)
	if err!=nil{
		t.Error("GetKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("GetKV failed, should got internal Error 404 ")
	}

	/*
		Case NOT FOUND Connect to B-Storage
	*/
	reqC = service.ConnectionRequest{Type:service.StoreType_BTreeDisk}
	_,_ = server.ConnectZPKV(ctx,&reqC)
	res,err = server.GetKV(ctx,&req)
	if err!=nil{
		t.Error("GetKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("GetKV failed, should got internal Error 404 ")
	}

	/*
		Normal case
	*/
	req.Key = "abc"
	res,err = server.GetKV(ctx,&req)
	if err!=nil{
		t.Error("GetKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("GetKV failed, got internal Error "+res.GetStatus().GetError())
	}

	/*
		Case Can't get conn ID
	*/
	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("",errors.New("error"))
	server = &ZPKVServiceImpl{ServiceUtils:mockService}
	req = service.GetKVRequest{Key:"abc"}
	res,err = server.GetKV(ctx,&req)
	if err==nil{
		t.Error("GetKV failed, Should got Error")
	}

}

func TestZPKVServiceImpl_RemoveKV(t *testing.T) {
	ctx := context.Background()
	mockService := newServiceUtilsMock()
	gStore := &gStoreMock{}
	gStore.On("Remove","removedNULL").Return(false)
	gStore.On("Remove","abc").Return(true)

	mockService.On("GetStore",mock.Anything).Return(gStore,nil)
	mockService.On("GetConnID",mock.Anything).Return("1223",nil)
	var server = &ZPKVServiceImpl{ServiceUtils:mockService}

	req := service.RemoveKVRequest{Key:"removedNULL"}

	res,err := server.RemoveKV(ctx,&req)
	if err!=nil{
		t.Error("RemoveKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("RemoveKV failed, should get Error Store type not Available " + res.GetStatus().GetError())
	}

	reqC := service.ConnectionRequest{Type:service.StoreType_BPlusTreeDisk}
	_,_ = server.ConnectZPKV(ctx,&reqC)
	res,err = server.RemoveKV(ctx,&req)
	if err!=nil{
		t.Error("RemoveKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("RemoveKV failed, got internal error: "+res.GetStatus().GetError())
	} else if res.GetCheck(){
		t.Error("RemoveKV failed, Nothing to remove")
	}

	reqC = service.ConnectionRequest{Type:service.StoreType_BTreeDisk}
	_,_ = server.ConnectZPKV(ctx,&reqC)
	res,err = server.RemoveKV(ctx,&req)
	if err!=nil{
		t.Error("RemoveKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("RemoveKV failed, got internal error: "+res.GetStatus().GetError())
	} else if res.GetCheck(){
		t.Error("RemoveKV failed, Nothing to remove")
	}

	req.Key="abc"
	res,err = server.RemoveKV(ctx,&req)
	if err!=nil{
		t.Error("RemoveKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("RemoveKV failed, got internal Error "+res.GetStatus().GetError())
	} else if !res.GetCheck() {
		t.Error("RemoveKV failed, Can't remove")
	}

	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("",errors.New("error"))
	server = &ZPKVServiceImpl{ServiceUtils:mockService}
	req = service.RemoveKVRequest{Key:"abc"}
	res,err = server.RemoveKV(ctx,&req)
	if err==nil{
		t.Error("RemoveKV failed, Should got Error")
	}

}

func TestZPKVServiceImpl_ExistKV(t *testing.T) {
	ctx := context.Background()

	mockService := newServiceUtilsMock()
	gStore := &gStoreMock{}
	gStore.On("Exist","existNULL").Return(false)
	gStore.On("Exist","abc").Return(true)

	mockService.On("GetStore",mock.Anything).Return(gStore,nil)
	mockService.On("GetConnID",mock.Anything).Return("1223",nil)
	var server = &ZPKVServiceImpl{ServiceUtils:mockService}

	req := service.ExistKVRequest{Key:"existNULL"}

	res,err := server.ExistKV(ctx,&req)
	if err!=nil{
		t.Error("ExistKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() == 1{
		t.Error("ExistKV failed, should get Error Store type not Available " + res.GetStatus().GetError())
	}

	reqC := service.ConnectionRequest{Type:service.StoreType_BPlusTreeDisk}
	_,_ = server.ConnectZPKV(ctx,&reqC)
	res,err = server.ExistKV(ctx,&req)
	if err!=nil{
		t.Error("ExistKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("ExistKV failed, got internal error: "+res.GetStatus().GetError())
	} else if res.GetCheck(){
		t.Error("ExistKV failed, Nothing to remove")
	}


	reqC = service.ConnectionRequest{Type:service.StoreType_BTreeDisk}
	_,_ = server.ConnectZPKV(ctx,&reqC)
	res,err = server.ExistKV(ctx,&req)
	if err!=nil{
		t.Error("ExistKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("ExistKV failed, got internal error: "+res.GetStatus().GetError())
	} else if res.GetCheck(){
		t.Error("ExistKV failed, Nothing to remove")
	}

	req.Key="abc"
	res,err = server.ExistKV(ctx,&req)
	if err!=nil{
		t.Error("ExistKV failed, got Error ",err.Error())
	} else if res.GetStatus().GetCode() != 1{
		t.Error("ExistKV failed, got internal Error "+res.GetStatus().GetError())
	} else if !res.GetCheck() {
		t.Error("ExistKV failed, Can't remove")
	}


	mockService = newServiceUtilsMock()
	mockService.On("GetConnID",mock.Anything).Return("",errors.New("error"))
	server = &ZPKVServiceImpl{ServiceUtils:mockService}
	req = service.ExistKVRequest{Key:"abc"}
	res,err = server.ExistKV(ctx,&req)
	if err==nil{
		t.Error("ExistKV failed, Should got Error")
	}

}
