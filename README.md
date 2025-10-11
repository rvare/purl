# Purl

Purl is a preprocessor that allows you to use special IDs within your text files instead of the actual URLs.
The idea is, instead of inserting long URLs in your files (whether they're source files or MarkDown files), you can use a special shorthand to keep things neat and tidy in your files instead of insert long URLs.

## Use

### Reference File

To use this program, you need a `ref` file (short for reference).
In this file, you have what is essnetially a key-value pair scheme.
In the file you write:

```text
%ID {
www.myurl.wow
}
```

Here, the `%` indicates a new ID and the `{` signifies the start of a URL.
Always end the URL with a closing brace `}`.
Whitespace will not effect the reference file.
You could write the above like this

```text
%ID{www.myurl.wow}
```

but that's hard to read.

### Using

When using the program, it assumes that the `ref` file is in the same directory where the program is executing.
To use the program, type of the following:

```bash
./a.exe <directory for input file> <directory for output file>
```

