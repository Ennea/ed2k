Create ed2k hashes of anything you feed into stdin. Run `make` to compile. Usage:
```
cat hello_world.txt | ./ed2k
```

Will write both the number of bytes read and the ed2k hash to stdout, like this:
```
12 97668ab2f29d0115bd0d1161b9bec520
```


MD4 implementation from https://openwall.info/wiki/people/solar/software/public-domain-source-code/md4.
