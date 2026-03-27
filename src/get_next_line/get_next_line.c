#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "get_next_line.h"

static char *append_buffer(char *buffer, const char *chunk, ssize_t chunk_len) {
  const size_t old_len = buffer ? strlen(buffer) : 0;

  // ReSharper disable once CppDFAMemoryLeak
  char *new_buffer = realloc(buffer, old_len + chunk_len + 1);
  if (!new_buffer) {
    free(buffer);
    return NULL;
  }

  memcpy(new_buffer + old_len, chunk, chunk_len);
  new_buffer[old_len + chunk_len] = '\0';

  return new_buffer;
}

char *get_next_line(const int fd) {
  static char *buffer = NULL;
  char read_buf[BUFFER_SIZE + 1];
  char *line;
  char *newline_pos;

  if (fd < 0 || BUFFER_SIZE <= 0)
    return NULL;

  while (!buffer || !(newline_pos = strchr(buffer, '\n'))) {
    const ssize_t bytes_read = read(fd, read_buf, BUFFER_SIZE);
    if (bytes_read == -1) {
      free(buffer);
      buffer = NULL;
      return NULL;
    }
    if (bytes_read == 0)
      break;

    // ReSharper disable once CppDFAMemoryLeak
    buffer = append_buffer(buffer, read_buf, bytes_read);
    if (!buffer)
      return NULL;
  }

  if (!buffer)
    return NULL;

  newline_pos = strchr(buffer, '\n');
  if (newline_pos) {
    const size_t line_len = newline_pos - buffer + 1;
    line = strndup(buffer, line_len);

    const size_t remain_len = strlen(newline_pos + 1);
    if (remain_len > 0)
      memmove(buffer, newline_pos + 1, remain_len + 1);
    else {
      free(buffer);
      buffer = NULL;
    }
  } else {
    line = strdup(buffer);
    free(buffer);
    buffer = NULL;
  }

  return line;
}