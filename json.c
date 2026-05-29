#include <stdio.h>
#include <file_lib.h>
#include <fcntl.h>
#include <json.h>
#include <string_lib.h>
#include <string_sequence.h> // for SeqStringFromString()
#include <regex.h>
#include <fcntl.h> // O_RDONLY

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

int query(JsonElement *element, Seq *queryParts, int queryIndex)
{
  // recursive function to pop off querySequence items until either found and print the path and value or find nothing
  fprintf(stderr, "query(element, queryParts, queryIndex: %d)\n", queryIndex);
  fprintJson(stderr, element);

  int queryLength = SeqLength(queryParts);
  fprintf(stderr, "queryLength is %d\n", queryLength);


  if (queryIndex+1 > queryLength) {
    return 0;
  }

  char *queryPart = SeqAt(queryParts, queryIndex);
  fprintf(stderr, "queryPart[%d] is %s\n", queryIndex, queryPart);

  if (StringEqual(queryPart, "*")) {
    JsonElementType jet = JsonGetElementType(element);
    if (jet != JSON_ELEMENT_TYPE_CONTAINER) {
      fprintf(stderr, "Failed to find container for * query at level %d\n", queryIndex);
      return 1;
    }

    JsonIterator iter = JsonIteratorInit(element);
    JsonContainerType jct = JsonGetContainerType(element);
    JsonElement *next;

    if (jct == JSON_CONTAINER_TYPE_OBJECT) {
      fprintf(stderr, "iterating over object\n");
      const char *key;
      while (key = JsonIteratorNextKey(&iter)) {
        JsonElement *next = JsonObjectGet(element, key);
        if (query(next, queryParts, queryIndex+1) != 0) {
          return 0;
        }
      }
    }
    if (jct == JSON_CONTAINER_TYPE_ARRAY) {
      fprintf(stderr, "iteration over array\n");
      while (next = JsonIteratorNextValue(&iter)) {
        printf("\n"); // new row, new line :p empty line to indicate next row, obtuse but still visually makes sense
        if (query(next, queryParts, queryIndex+1) != 0) {
          return 0;
        }
      }
    }
  } else {
    fprintf(stderr, "looking for key with queryPart %s\n", queryPart);
    const char *key = queryPart; // unless we find a subscript notation

    Seq *captures;
    captures = StringMatchCaptures("^(.*)[\[\(]([0-9]*)[])]$", queryPart, false);

    if (captures != NULL) {
      key = BufferData(SeqAt(captures, 1));
      fprintf(stderr, "found %ld captures\n", SeqLength(captures));
      for (int i=0; i<SeqLength(captures); i++) {
        fprintf(stderr, "capture[%d] is %s\n", i, BufferData(SeqAt(captures, i)));
      }
    }

    fprintf(stderr, "key is %s\n", key);
    JsonElement *value;
    value = JsonObjectGet(element, key);
    if (value == NULL ) {
      fprintf(stderr, "No object for key %s\n", key);
      return 0;
    }
    if (captures == NULL) {
      fprintf(stderr, "No captures\n");
      if (queryIndex+1 < queryLength) {
        return query(value, queryParts, queryIndex+1);
      } else {
        printJsonWithKey(key, value);
      }
    } else {
      int64_t index;
      if (StringToInt64(BufferData(SeqAt(captures, 2)), &index)) {
        fprintf(stderr, "Unable to parse number portion of key %s\n", queryPart);
        exit(1);
      }
      JsonElement *item = JsonArrayGetAsObject(value, index);
      if (queryIndex+1 < queryLength) {
        return query(item, queryParts, queryIndex+1);
      } else {
        printJsonWithKey(key, item);
      }
    }
  }
  return 0;
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
    fprintf(stderr, "json parse failed\n");
    return 2;
  }
  WriterClose(w);

  if (StringStartsWith("check", argv[1])) {
    return 0; // parse above was OK
  } else if (StringStartsWith("pretty", argv[1]) ||
             StringStartsWith("print", argv[1])) {
    prettyPrintJson(json);
    return 0;
  }

  fprintf(stderr, "argi is %d\n", argi);
  if (argi > argc)
  {
    fprintf(stderr, "Expected a query argument\n");
    usage(argv);
    return 1;
  }

  char *queryString = argv[argi];
  fprintf(stderr, "queryString is %s\n", queryString);

  Seq *queryParts = SeqStringFromString(queryString, '/');
  query(json, queryParts, 0);
}
