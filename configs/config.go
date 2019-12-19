package configs


import (
	"github.com/spf13/viper"
)

const configFilePath = "./configs"
const configFileName = "config"

//ServiceConfig struct
type ServiceConfig struct {
	GRPCPort int
	GRPCHost string
	PoolSize int
	TimeOut  int
}

//LoadConfig load configs
func LoadConfig() error {
	viper.SetConfigName(configFileName)
	viper.AddConfigPath(configFilePath)
	viper.SetConfigType("yaml")

	return viper.ReadInConfig()
}