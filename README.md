# Sysdiff

A quick way to enumerate folders in a new file system; usefull when pentesting to find interesting stuff.

## Showcase

```bash
left@lefts-big:/projects/sv/Sysdiff$ ./Sysdiff -h
  ./Sysdiff {OPTIONS}

    Sysdiff

  OPTIONS:

      -h, --help                        help
      -d[maxDepth]                      Max Depth
      -s, --save                        If it should save to sdiff.bin :. it
                                        will not print to std::cout
      -p[startPath], --path=[startPath] Start Path
      -e[evadedPaths],
      --evaded=[evadedPaths]            Evaded paths comma separated
```

The output is json you can find viewer for differences in my other repo