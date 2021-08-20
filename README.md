# KeyValueStorage

[![codecov](https://codecov.io/gh/smirok/KeyValueStorage/branch/master/graph/badge.svg?token=0XSW06RMUT)](https://codecov.io/gh/smirok/KeyValueStorage)
[![release](https://github.com/smirok/KeyValueStorage/actions/workflows/build.yml/badge.svg)](https://github.com/smirok/KeyValueStorage/workflows/build.yml)

## Description
Two-level key-value store. A small store, based on hash-table, is located in RAM. The big trie-based store is located on a disk.

The key-value store periodically merges the content of a small store into a big store and updates its state, so the key-value store doesn't store outdated key-value pairs on disk.

## Installation
```bash
$ git clone --recursive https://github.com/smirok/Key-Value-Store
$ cd Key-Value-Store/
$ mkdir build
$ cd build/
$ cmake ..
$ make
```

## Usage example
```
$ build/kvs
Enter the key size to use:
$ 2
Enter the value size to use:
$ 4
$ add b4 12de
$ get b4
12de
$ clear
Store was cleared.
$ get b4
This key is not in store
$ exit
```

## Queries
- `add(Key, Value)` inserts a (key, value) pair into the storage. If the key already exists in the storage, update the corresponding value.
- `get(Key)` returns the value to which the specified key is mapped, or prints "This key is not in store" if this storage contains no mapping for the key.
- `del(Key)` removes the key and its corresponding value if the map contains the mapping for the key.
- `clear` clears the storage.
- `exit` stops the storage.

## Testing (unit and stress)
```bash
$ build/test/tests
```

## Documentation
[https://smirok.github.io/Key-Value-Store/](https://smirok.github.io/Key-Value-Store/)
