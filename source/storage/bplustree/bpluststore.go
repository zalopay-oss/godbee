package bplustree

/*
#include "GBPlusTreeStore.h"
#include <stdlib.h>
#cgo CXXFLAGS: -g -std=c++17 -w -pthread
*/
import (
	"C"
)
import (
	"errors"
	"github.com/1612898/zpkvservice/storage"
	"sync"
	"unsafe"
)

type GBPlusStore struct {
	tree C.GBPlusTreeStore
}

var instance *GBPlusStore
var once sync.Once
var keySize, valueSize = 35,1024
var configFile = "./configs/.configBPlus"

func GetInstance() *GBPlusStore {
	once.Do(func() {
		instance = &GBPlusStore{}
		instance.tree = C.GBPlusInit()
		tmp,err := storage.SplitConfig(configFile)
		if err==nil{
			keySize = tmp[0]
			valueSize = tmp[1]
		}
	})
	return instance
}

func Free() {
	if instance != nil {
		instance.Free()
	}
}

func (instance GBPlusStore) Free() {
	C.GBPlusFree(instance.tree)
	instance.tree = nil
}

func (instance GBPlusStore) Get(k string) (string, error) {
	key := C.CString(k)
	res := C.GBPlusGet(instance.tree, key)
	strRes := ""
	var err error = nil
	if res == nil {
		err = errors.New("key not exist")
	} else {
		strRes = C.GoString(res)
	}
	C.free(unsafe.Pointer(res))
	C.free(unsafe.Pointer(key))
	return strRes, err
}

func (instance GBPlusStore) Set(k string, v string) error {
	if len(k) > keySize || len(v)>keySize {
		return errors.New("key or value is oversize")
	}
	key := C.CString(k)
	value := C.CString(v)
	C.GBPlusSet(instance.tree, key, value)
	C.free(unsafe.Pointer(key))
	C.free(unsafe.Pointer(value))
	return nil
}

func (instance GBPlusStore) Remove(k string) bool {
	key := C.CString(k)
	res := C.GBPlusRemove(instance.tree, key)
	C.free(unsafe.Pointer(key))
	return res != 0
}

func (instance GBPlusStore) Exist(k string) bool {
	key := C.CString(k)
	res := C.GBPlusExist(instance.tree, key)
	C.free(unsafe.Pointer(key))
	return res != 0
}
