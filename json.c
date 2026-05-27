#include <stdio.h>
#include <file_lib.h>
#include <json.h>

int main(int argc, char *argv[])
{

  int fd = 0; // default to stdin
  if (argc > 1 && PathExists(argv[1])) {
    fd = safe_open(argv[1], O_RDONLY);
    if (fd == -1)
    {
      fprintf(stderr, "Failed to open file at %s\n", argv[1]);
      perror("safe_open");
      return 1;
    }
  }

  Writer *w = FileReadFromFd(fd, SIZE_MAX, NULL);
  if (!w)
  {
    fprintf(stderr, "Unable to read json content");
    return 1;
  }

  JsonElement *json = NULL;
  const char *data = StringWriterData(w);
  if (JsonParse(&data, &json) != JSON_PARSE_OK)
  {
    WriterClose(w);
    fprintf(stderr, "json parse failed");
    return 2;
  }
  WriterClose(w);

  return 0;
}
