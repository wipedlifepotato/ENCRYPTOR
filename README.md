# MEncryptor 
Prototype for encrypt/decrypt system in Haiku OS

## License
WTF

## Usage
Compiling: ```make```
Encrypt File: ```./MEncryptor --key "hello" --iv "123" -f input.txt```
After your encrypted file will be in input.txt.enc
For decrypt file: ```./MEncryptor --key "hello" --iv "123" -f input.txt.enc```

Also, after all manipulation you maybe want to shred file, you can do it with -s option

## GUI

<img width="1168" height="889" alt="image" src="https://github.com/user-attachments/assets/109976e1-0aab-4dcd-bf4d-c1ee552a9414" />


for a gui launch app with --gui option. This is experemental and can be fully rewrited in future.

## Alternative
```
<uu2> openssl enc -chacha20 -salt -pbkdf2 -iter 100000 -in file -out file.enc
<uu2> openssl enc -d -chacha20 -pbkdf2 -iter 100000 -in file.enc -out file.dec
<uu2> Важно: параметры KDF (-pbkdf2, -iter, -salt) должны совпадать при шифровании и расшифровке, иначе расшифровка не удастся.
```
