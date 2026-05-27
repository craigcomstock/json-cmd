(cd libntech && git clean -fXd)
(cd tests/unit/cmocka && git clean -fXd)

# Check for libntech submodule and give nice advice on how to fix:
if [ ! -f libntech/libutils/sequence.h ] ; then
    echo "Error: libntech/libutils/sequence.h is missing"
    echo "       You probably forgot to use the --recursive option when cloning"
    echo "       To fix it now, run:"
    echo "       git submodule init && git submodule update"
    exit 1
fi

autoreconf -i ./
./configure $@
