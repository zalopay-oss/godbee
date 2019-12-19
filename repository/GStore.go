package repository

import (
	"github.com/1612898/zpkvservice/storage/bplustree"
	"github.com/1612898/zpkvservice/storage/btree"
	log "github.com/sirupsen/logrus"
)

type GStore interface {
	Set(string, string)
	Get(string) (string, error)
	Remove(string) bool
	Exist(string) bool
	Free()
}

func GetInstanceBPlus() (GStore, error) {
	return bplustree.GetInstance(), nil
}

func GetInstanceB() (GStore, error) {
	return btree.GetInstance(), nil
}


func Free() {
	btree.Free()
	bplustree.Free()
}

func Init() {
	bplustree.GetInstance()
	log.Info("B+ tree")
	btree.GetInstance()
	log.Info("B tree")
}


