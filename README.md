# MEncryptor 
Prototype for encrypt/decrypt system in Haiku OS

## License
WTF

## Usage
Compiling: ```make```
Encrypt File: ```./MEncryptor --key "hello" --iv "123" -f input.txt```
After your encrypted file will be in input.txt.enc
For decrypt file: ```./MEncryptor --key "hello" --iv "123" -f input.txt.enc```

Also, after all manipulation you maybe want to shred file, you can do it with:```
-s option
```
