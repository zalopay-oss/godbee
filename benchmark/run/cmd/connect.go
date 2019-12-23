package cmd

import (
	boomer "github.com/myzhan/boomer"
	"github.com/spf13/cobra"
	"github.com/zalopay-oss/GodBee/benchmark"
)

var connectBoomerCmd = &cobra.Command{
	Use:   "connect",
	Short: "Test connect boomer",
	Run: func(cmd *cobra.Command, args []string) {
		boomerClient := benchmark.GetBoomerClient()
		tasks := make([]*boomer.Task, 0)
		_ = boomerClient.LoadTask(&tasks, benchmark.CONNECT, 1)
		boomerClient.RunTask(tasks)
	},
}

func init() {
	rootCmd.AddCommand(connectBoomerCmd)
}
