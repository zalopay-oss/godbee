package cmd

import (
	boomer "github.com/myzhan/boomer"
	"github.com/spf13/cobra"
	"github.com/zalopay-oss/GodBee/benchmark"
)

var closeBoomerCmd = &cobra.Command{
	Use:   "close",
	Short: "Test close boomer",
	Run: func(cmd *cobra.Command, args []string) {
		boomerClient := benchmark.GetBoomerClient()
		boomerClient.Connect()
		tasks := make([]*boomer.Task, 0)
		_ = boomerClient.LoadTask(&tasks, benchmark.CLOSE, 1)
		boomerClient.RunTask(tasks)
	},
}

func init() {
	rootCmd.AddCommand(closeBoomerCmd)
}
