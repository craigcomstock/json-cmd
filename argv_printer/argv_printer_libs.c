#include <sequence.h> // Seq
#include <stdio.h>    // printf()

int main(int argc, const char *const *argv)
{
    Seq *seq = SeqFromArgv(argc, argv);
    size_t length = SeqLength(seq);
    assert(length == argc);
    for (int i = 0; i < length; ++i)
    {
        printf("argv[%zu]: '%s'\n", length, (const char *) SeqAt(seq, i));
    }
    return 0;
}
