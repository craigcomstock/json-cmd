set -ex
make json
./json check <object.json
./json ch object.json
./json c array.json
./json pretty array.json
./json print <array.json
./json p object.json
