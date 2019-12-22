package cmd

import (
	boomer "github.com/myzhan/boomer"
	"github.com/spf13/cobra"
	"github.com/zalopay-oss/GodBee/benchmark"
)

var existBoomerCmd = &cobra.Command{
	Use:   "exist",
	Short: "Test exist boomer",
	Run: func(cmd *cobra.Command, args []string) {
		boomerClient := benchmark.GetBoomerClient()
		boomerClient.Connect()
		tasks := make([]*boomer.Task, 0)
		_ = boomerClient.LoadTask(&tasks, benchmark.EXIST, 1)
		boomerClient.RunTask(tasks)
	},
}

func init() {
	rootCmd.AddCommand(existBoomerCmd)
}
