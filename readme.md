#encryptout
Encryptout is a simple encryption tool that uses [GPGME](https://www.gnupg.org/%28es%29/related_software/gpgme/index.html) library that allows you to manage your password with GPG keys.
It's safe to keep the encrypted file on the cloud and sync between your devices.

##Usage

```
usage: ./encryptout --cipherFile=string --select-key=int [options] ...
options:
  -c, --cipherFile    Cipher database (string)
  -a, --add           Add a string as record (string [=])
  -d, --dump          Dump record
  -l, --list-keys     List private keys
  -s, --select-key    Select a key in key array (int)
  -?, --help          print this message
```

##Build from Source

To compile this sofeware from source, you need CMake and GPGME library. 

```
mkdir build
cd build
cmake ..
make
```

