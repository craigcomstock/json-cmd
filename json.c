#include <stdio.h>
#include <file_lib.h>
#include <json.h>
#include <string_lib.h>

void usage(char *argv[])
{
  fprintf(stderr,
"usage: %s <command> <file or args>\n"
"  note: if no file stdin will be read\n"
"\n"
"  check		validate json\n"
"  print		print json nicely formatted\n"
"  query		look for keys and values in json\n"
, argv[0]
);
}

void fprintJson(FILE *stream, JsonElement *value)
{
  Writer *writer = StringWriter();
  JsonWriteCompact(writer, value);
  char *output = StringWriterClose(writer);
  fprintf(stream, "%s\n", output);
}

void printJson(JsonElement *value)
{
  fprintJson(stdout, value);
}

void printJsonWithKey(const char *key, JsonElement *value)
{
  printf("%s\t", key);
  printJson(value);
}

void prettyPrintJson(JsonElement *value)
{
  Writer *writer = StringWriter();
  JsonWrite(writer, value, 0);
  char *output = StringWriterClose(writer);
  printf("%s\n", output);
}


int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    usage(argv);
    return 1;
  }


  int argi=2;

  int fd = 0; // default to stdin
  if (argc > 2 && PathExists(argv[argi])) {
    fd = safe_open(argv[argi], O_RDONLY);
    if (fd == -1)
    {
      fprintf(stderr, "Failed to open file at %s\n", argv[argi]);
      perror("safe_open");
      return 1;
    }
    argi++;
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

  if (StringStartsWith("check", argv[1])) {
    return 0; // parse above was OK
  } else if (StringStartsWith("pretty", argv[1]) ||
             StringStartsWith("print", argv[1])) {
    prettyPrintJson(json);
    return 0;
  } else {
    usage(argv);
    return 1;
  }
}
