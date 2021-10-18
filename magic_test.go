package magic

import (
	"fmt"
	"os/exec"
	"runtime"
	"testing"

	"github.com/alecthomas/assert"
	"github.com/stretchr/testify/suite"
)

type MagicTestSuite struct {
	suite.Suite
	binary string
}

func (self *MagicTestSuite) SetupTest() {
	self.binary = "./output/magic_file"
	if runtime.GOOS == "windows" {
		self.binary += ".exe"
	}
}

func (self *MagicTestSuite) TestCollector() {
	cmdline := []string{
		"file", self.binary,
	}
	cmd := exec.Command(self.binary, cmdline...)
	out, err := cmd.CombinedOutput()
	assert.NoError(self.T(), err)
	fmt.Printf("Magic is %v\n", string(out))
	assert.Contains(self.T(), string(out), "executable")
}

func TestMagic(t *testing.T) {
	suite.Run(t, &MagicTestSuite{})
}
