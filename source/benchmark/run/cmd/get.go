package cmd

import (
	"github.com/1612898/zpkvservice/benchmark"
	boomer "github.com/myzhan/boomer"
	"github.com/spf13/cobra"
)

var getBoomerCmd = &cobra.Command{
	Use:   "get",
	Short: "Test get boomer",
	Run: func(cmd *cobra.Command, args []string) {
		boomerClient := benchmark.GetBoomerClient()
		boomerClient.Connect()
		tasks := make([]*boomer.Task, 0)
		_ = boomerClient.LoadTask(&tasks,benchmark.GET, 1)
		boomerClient.RunTask(tasks)
	},
}

func init() {
	rootCmd.AddCommand(getBoomerCmd)
}
