package cmd

import (
	boomer "github.com/myzhan/boomer"
	"github.com/spf13/cobra"
	"github.com/zalopay-oss/GodBee/benchmark"
)

var removeBoomerCmd = &cobra.Command{
	Use:   "remove",
	Short: "Test remove boomer",
	Run: func(cmd *cobra.Command, args []string) {
		boomerClient := benchmark.GetBoomerClient()
		boomerClient.Connect()
		tasks := make([]*boomer.Task, 0)
		_ = boomerClient.LoadTask(&tasks, benchmark.REMOVE, 1)
		boomerClient.RunTask(tasks)
	},
}

func init() {
	rootCmd.AddCommand(removeBoomerCmd)
}
