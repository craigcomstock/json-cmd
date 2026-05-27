set -ex
make json
./json q '*/Names[0)' <podman.ps.json 2>/dev/null
exit 0
./json q '*/ID' <docker.ps.json 2>/dev/null
./json q '*/Id' <podman.ps.json 2>/dev/null
./json q '*/Names' <podman.ps.json 2>/dev/null
exit 0
./json check <object.json
./json ch object.json
./json c array.json
./json pretty array.json
./json print <array.json
./json p object.json
