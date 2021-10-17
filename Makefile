linux:
	go run ./make.go -v linux

windows:
	go run ./make.go -v windows

sync:
	go run ./make.go -v prepareMagicFile ~/projects/file/
