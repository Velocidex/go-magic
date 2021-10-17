# go-magic #

## Go bindings for libmagic ##

There are number of existing bindings for libmagic, for example
`github.com/vimeo/go-magic`.

This binding aims to be self contained, without requiring libmagic to
be installed. This allows us to build this dependency for all
architectures even Windows.

### Installing ###

```
go get https://github.com/Velocidex/go-magic
```

This package contains libmagic source code already so you do not need
to have it installed on the system.

### How to use it from Go code.

The first step is to obtain a handle to the library.

```go
handle := magic.NewMagicHandle(magic.MAGIC_NONE)
defer handle.Close()

// Load built in magic files
magic_files.LoadDefaultMagic(handle)
classification := handle.File("foobar.jpeg")
```

This binding bundles the built in magic files. You can load them into
memory by calling `magic_files.LoadDefaultMagic(handle)`.

The packages magic files are the default ones provided by the file
package.

## Reference

* File source code is here https://github.com/file/file
