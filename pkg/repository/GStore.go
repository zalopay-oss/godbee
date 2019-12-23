package repository

import (
	log "github.com/sirupsen/logrus"
	"github.com/zalopay-oss/GodBee/pkg/storage/bplustree"
	"github.com/zalopay-oss/GodBee/pkg/storage/btree"
)

type GStore interface {
	Set(string, string) error
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
	log.Info("Prepared B+storage")
	btree.GetInstance()
	log.Info("Prepared B-storage")
}
