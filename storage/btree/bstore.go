package btree

/*
#include "KeyValueStoreGC.h"
#include <stdlib.h>
#cgo CXXFLAGS: -g -std=c++17 -w -pthread
*/
import (
	"C"
)
import (
	"errors"
	"sync"
	"unsafe"
)

type BTreeStore struct {
	tree C.KeyValueStoreDiskGC
}

var instance *BTreeStore
var once sync.Once


func GetInstance() *BTreeStore {
	once.Do(func() {
		instance = &BTreeStore{C.BTreeInit()}
	})
	return instance
}

func Free()  {
	if instance!=nil{
		instance.Free()
	}
}

func (b BTreeStore) Free() {
	C.BTreeFree(b.tree)
	b.tree = nil
}

func (b BTreeStore) Get(k string) (string, error) {
	key := C.CString(k)
	res := C.BTreeGet(b.tree, key)
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

func (b BTreeStore) Set(k string, v string) {
	key := C.CString(k)
	value := C.CString(v)
	C.BTreeSet(b.tree, key, value)
	C.free(unsafe.Pointer(key))
	C.free(unsafe.Pointer(value))
}

func (b BTreeStore) Remove(k string) bool {
	key := C.CString(k)
	res := C.BTreeRemove(b.tree, key)
	C.free(unsafe.Pointer(key))
	return int(res) != 0
}

func (b BTreeStore) Exist(k string) bool {
	key := C.CString(k)
	res := C.BTreeExist(b.tree, key)
	C.free(unsafe.Pointer(key))
	return int(res) != 0
}

