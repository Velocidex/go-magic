//+build mage

package main

import (
	"bufio"
	"fmt"
	"io/ioutil"
	"os"
	"path/filepath"
	"runtime"
	"strings"

	"github.com/magefile/mage/mg"
	"github.com/magefile/mage/sh"
)

const (
	output_file = "./magic_files/magic_rules.mgc"

	// apt-get install gcc-mingw-w64-x86-64
	mingw_xcompiler = "x86_64-w64-mingw32-gcc"

	// apt-get install gcc-mingw-w64
	mingw_xcompiler_32 = "i686-w64-mingw32-gcc"
)

func fileb0x(asset string) error {
	err := sh.Run("fileb0x", asset)
	if err != nil {
		err = sh.Run(mg.GoCmd(), "install", "github.com/Velocidex/fileb0x@d54f4040016051dd9657ce04d0ae6f31eab99bc6")
		if err != nil {
			return err
		}

		err = sh.Run("fileb0x", asset)
	}

	return err
}

type Builder struct {
	goos      string
	arch      string
	extension string
}

func (self *Builder) Name() string {
	return "magic_file" + self.extension
}

func (self *Builder) Env() map[string]string {
	env := make(map[string]string)

	env["GOOS"] = self.goos
	env["GOARCH"] = self.arch
	env["CGO_ENABLED"] = "1"

	// If we are cross compiling, set the right compiler.
	if (runtime.GOOS == "linux" || runtime.GOOS == "darwin") &&
		self.goos == "windows" {

		if self.arch == "amd64" {
			env["CC"] = mingw_xcompiler
		} else {
			env["CC"] = mingw_xcompiler_32
		}
	}

	return env
}

func (self Builder) Run() error {
	if err := os.Mkdir("output", 0700); err != nil && !os.IsExist(err) {
		return fmt.Errorf("failed to create output: %v", err)
	}

	args := []string{
		"build",
		"-o", filepath.Join("output", self.Name()),
	}
	args = append(args, "./bin/")

	return sh.RunWith(self.Env(), mg.GoCmd(), args...)
}

func Linux() error {
	return Builder{
		goos: "linux",
		arch: "amd64",
	}.Run()
}

func PrepareMagicFile(file_src_tree string) error {
	fmt.Printf("Will compile magic files from file source tree %v\n", file_src_tree)

	out_fd, err := os.OpenFile(
		output_file, os.O_WRONLY|os.O_CREATE|os.O_TRUNC, 0600)
	if err != nil {
		return err
	}

	globs, err := filepath.Glob(file_src_tree + "/magic/Magdir/*")
	if err != nil {
		return err
	}

	for _, f := range globs {
		fmt.Printf("Including %s\n", f)

		fd, err := os.Open(f)
		if err != nil {
			return err
		}

		// Read lines from fd and remove empty line or comments
		scanner := bufio.NewScanner(fd)
		for scanner.Scan() {
			// line := strings.TrimSpace(scanner.Text())
			line := scanner.Text()
			if len(line) > 0 && line[0] == '#' {
				continue
			}
			_, err = out_fd.Write([]byte(line + "\n"))
			if err != nil {
				return err
			}
		}
		fd.Close()
	}

	err = fileb0x("magic_files/b0x.yaml")
	if err != nil {
		return err
	}

	return replace_string_in_file(
		"magic_files/ab0x.go", "func init()", "func Init()")

}

func replace_string_in_file(filename string, old string, new string) error {
	read, err := ioutil.ReadFile(filename)
	if err != nil {
		return err
	}
	newContents := strings.Replace(string(read), old, new, -1)
	return ioutil.WriteFile(filename, []byte(newContents), 0)
}
