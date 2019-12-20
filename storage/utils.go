package storage

import (
	"bufio"
	"encoding/csv"
	"fmt"
	"io"
	"os"
	"strconv"
	"strings"
)

func splitLine(s string) ([]string, error) {
	r := csv.NewReader(strings.NewReader(s))
	r.Comma = ' ' // space
	return r.Read()
}

func SplitConfig(filename string) ([]int, error) {
	f, err := os.Open(filename)
	if err != nil {
		fmt.Println("error opening file ", err)
		os.Exit(1)
		return nil, err
	}
	defer f.Close()
	r := bufio.NewReader(f)
	var res []int
	keySize, valueSize := 0,0
	for {
		value, err := r.ReadString(10) // 0x0A separator = newline
		if err == io.EOF {
			// do something here
			break
		} else if err != nil {
			return nil,err // if you return error
		}
		values, _ := splitLine(value)
		n := len(values)
		if n>=3 {
			cType,cValue := values[0],values[2]
			if cType == "BUFFER_SIZE" || cType == "KEY_SIZE" {
				tmp, err := strconv.Atoi(cValue)
				if err!=nil{
					return nil,err
				}
				keySize = tmp
			}
			if cType == "VALUE_SIZE" {
				tmp, err := strconv.Atoi(cValue)
				if err!=nil{
					return nil,err
				}
				valueSize = tmp
			}
		}
	}
	res = append(res, keySize)
	res = append(res,valueSize)
	return res,nil
}