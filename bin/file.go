package main

import (
	"fmt"

	"github.com/Velocidex/go-magic/magic"
	"github.com/Velocidex/go-magic/magic_files"
	"github.com/alecthomas/kingpin"
)

var (
	file_command = app.Command(
		"file", "apply magic files.")

	file_command_file_load = file_command.Flag(
		"magic", "The magic files to load",
	).String()

	file_command_file_type = file_command.Flag(
		"type", "The type of magic to get",
	).String()

	file_command_file_targets = file_command.Arg(
		"targets", "The magic files to load",
	).Required().Strings()
)

func doFile() {
	flag := magic.MAGIC_NONE
	switch *file_command_file_type {
	case "extension":
		flag = magic.MAGIC_EXTENSION
	case "mime":
		flag = magic.MAGIC_MIME
	case "":
	default:
		kingpin.Fatalf("Unsupported type (mime, extension) ")
	}

	handle := magic.NewMagicHandle(flag)
	defer handle.Close()

	// Load a special magic file or the default.
	if *file_command_file_load != "" {
		res := handle.Load(*file_command_file_load)
		fmt.Printf("Loaded %v: %v\n", *file_command_file_load, res)
	} else {
		magic_files.LoadDefaultMagic(handle)
	}

	for _, f := range *file_command_file_targets {
		fmt.Printf("%v: %v\n", f, handle.File(f))
	}
}

func init() {
	command_handlers = append(command_handlers, func(command string) bool {
		switch command {
		case file_command.FullCommand():
			doFile()
		default:
			return false
		}
		return true
	})
}
