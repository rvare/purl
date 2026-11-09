# Purl

Purl is a preprocessor that allows you to use special IDs within your text files instead of the actual URLs.
The idea is instead of inserting long URLs in your files (whether they're source files or Markdown files), you can use a special shorthand to keep things neat and tidy in your files.

## Use

### Reference File

To use this program, you need a `ref` file (short for reference).
In this file, you have what is essentially a key-value pair scheme.
In the file you write:

```text
%ID {
www.myurl.wow
}
```

Here, the `%` indicates a new ID and the `{` signifies the start of a URL.
Always end the URL with a closing brace `}`.
White-space will not effect the reference file.
You could write the above like this

```text
%ID{www.myurl.wow}
```

but that's hard to read.

### Using

When using the program, it assumes that the `ref` file is in the same directory where the program is executing.
To use the program, type of the following:

The simplest way to use purl is to give it a file that has your macros as input, and it'll process it and print to `stdout`.

```bash
purl /path/to/input/file
```

This also assumes that your `ref` file is in the same directory where you invoked it.

You can specify things explicitly using flags:

- `-i` specifies your input file.
- `-o` specifies where you want the output to go.
	- If left out, assume `stdout`.
- `-r` specify where your `ref` file is.
	- If left out, assume `ref` is in the current directory where `purl` is invoked.

Below is an example of the complete syntax to use with the flags:

```bash
purl -i=/path/to/input/file -o=/path/to/output -r=/path/to/ref/file
```

You can specify these in any order you wish.

## Building

Purl only relies on the Standard C Library.
No other library is used.

To build Purl, simply run `make`.
The `makefile` assumes you're using GCC.
If you want to use a different compiler, change the `CC` variable to whatever compiler you're using.
