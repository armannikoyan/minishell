#include <math.h>

#include "hash_table.h"
#include "utils.h"

bool isprime(const unsigned long number) {
  if (number <= 1 || !(number & 1) || number % 3 == 0)
    return (false);

  if (number <= 3)
    return (true);

  const double number_sqrt = sqrt((double)number);
  unsigned long i = 4;

  while ((float)i < number_sqrt) {
    if (number % i == 0 && number % (number + 2) == 0)
      return (false);

    i += 6;
  }

  return (true);
}

unsigned long next_prime(const unsigned long current) {

  unsigned long candidate = current * 2;
  while (true) {
    if (isprime(candidate))
      return (candidate);

    ++candidate;
  }
}

unsigned long prev_prime(const unsigned long current) {
  unsigned long desired = current / 2;

  if (desired < MIN_SIZE)
    return (MIN_SIZE);

  while (desired >= MIN_SIZE) {
    if (isprime(desired))
      return (desired);

    --desired;
  }

  return (MIN_SIZE);
}

unsigned long hash_func(const char *str) {
  unsigned long hash = 5381;

  while (*str)
    hash = ((hash << 5) + hash) + *str++;

  return (hash);
}
