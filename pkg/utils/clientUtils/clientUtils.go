package clientUtils

import (
	"context"
	"encoding/csv"
	"errors"
	"fmt"
	"strings"

	service2 "github.com/zalopay-oss/GodBee/pkg/service/api/proto"
)

func isExitCommand(command string) bool {
	return command == "exit" || command == "quit" || command == "q"
}

func splitCommand(s string) ([]string, error) {
	r := csv.NewReader(strings.NewReader(s))
	r.Comma = ' ' // space
	return r.Read()
}

func ExecuteCommand(cli service2.KeyValueStoreServiceClient, command string) bool {

	var commands []string
	commands, _ = splitCommand(command)
	n := len(commands)

	if n == 0 {
		fmt.Println("(error) ERR unknown command.")
		return true
	}

	if isExitCommand(commands[0]) {
		fmt.Println("Bye bye!!! Beeeeeee~")
		return false
	}

	if n > 0 {
		cmdType := strings.ToLower(commands[0])
		switch cmdType {
		case "connect":
			if n != 2 {
				fmt.Println("(error) ERR wrong number of arguments for 'connect' command")
			} else {
				storeType, err := toStoreType(commands[1])
				if err != nil {
					fmt.Println("(error) " + err.Error())
					break
				}

				req := service2.ConnectionRequest{Type: service2.StoreType(storeType)}

				res, err := cli.Connect(context.Background(), &req)
				if err != nil {
					fmt.Println(err)
				} else {
					if 1 == res.GetStatus().GetCode() {
						fmt.Println("OK")
					} else {
						fmt.Println("ERROR: " + res.GetStatus().GetError())
					}
				}
			}
		case "close":
			if n != 1 {
				fmt.Println("(error) ERR wrong number of arguments for 'close' command")
			} else {
				req := service2.DisconnectRequest{}
				res, err := cli.Disconnect(context.Background(), &req)
				if err != nil {
					fmt.Println(err)
				} else {
					if 1 == res.GetStatus().GetCode() {
						fmt.Println("OK")
					} else {
						fmt.Println("ERROR: " + res.GetStatus().GetError())
					}
				}
			}
		case "set":
			if n != 3 {
				fmt.Println("(error) ERR wrong number of arguments for 'connect' command")
			} else {
				req := service2.SetRequest{Key: commands[1], Value: commands[2]}
				res, err := cli.Set(context.Background(), &req)
				if err != nil {
					fmt.Println(err)
				} else {
					if 1 == res.GetStatus().GetCode() {
						fmt.Println("OK")
					} else {
						fmt.Println("ERROR: " + res.GetStatus().GetError())
					}
				}
			}
		case "get":
			if n != 2 {
				fmt.Println("(error) ERR wrong number of arguments for 'get' command")
			} else {
				req := service2.GetRequest{Key: commands[1]}
				res, err := cli.Get(context.Background(), &req)
				if err != nil {
					//fmt.Println("\""+res.GetValue()+"\"")
					fmt.Println("ERROR: " + err.Error())
				} else {
					if res.GetStatus().GetCode() == 404 {
						fmt.Println("(nil)")
					} else if res.GetStatus().GetCode() != 1 {
						fmt.Println(res.GetStatus().GetError())
					} else {
						fmt.Println("\"" + res.GetValue() + "\"")
					}
				}
			}
		case "exist":
			if n != 2 {
				fmt.Println("(error) ERR wrong number of arguments for 'exist' command")
			} else {
				req := service2.ExistRequest{Key: commands[1]}
				res, err := cli.Exist(context.Background(), &req)
				if err != nil {
					fmt.Println(err)
				} else if res.Status.Code != 1 {
					fmt.Println("ERROR: " + res.Status.Error)
				} else {
					if !res.GetCheck() {
						fmt.Println("FALSE")
					} else {
						fmt.Println("TRUE")
					}
				}
			}
		case "remove", "del":
			if n != 2 {
				fmt.Println("(error) ERR wrong number of arguments for 'remove' command")
			} else {
				req := service2.RemoveRequest{Key: commands[1]}
				res, err := cli.Remove(context.Background(), &req)
				if err != nil {
					fmt.Println(err)
				} else if res.Status.Code != 1 {
					fmt.Println("ERROR: " + res.Status.Error)
				} else {
					if !res.GetCheck() {
						fmt.Println("Can not delete.")
					} else {
						fmt.Println("OK")
					}
				}
			}
		default:
			fmt.Println("(error) ERR unknown command.")
		}
	}
	return true
}

func toStoreType(input string) (int, error) {
	input = strings.ToLower(input)
	switch input {
	case "bplus", "1", "bpluststore":
		return 1, nil
	case "b", "0", "bstore":
		return 0, nil
	}
	return 0, errors.New("invalid store")
}
