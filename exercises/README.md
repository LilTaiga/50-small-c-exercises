# Exercise index

Work in order. Read the instructions, experiment in the matching `.c` file,
then run `make test N=<number>`.

## 1 — Basic logic and character handling

| #   | Function     | Instructions                   |
| --- | ------------ | ------------------------------ |
| 1   | `my_isdigit` | [Is it a digit?](01.md)        |
| 2   | `my_isalpha` | [Is it a letter?](02.md)       |
| 3   | `my_isalnum` | [Letters or digits](03.md)     |
| 4   | `my_isspace` | [Whitespace](04.md)            |
| 5   | `my_toupper` | [Uppercase](05.md)             |
| 6   | `my_tolower` | [Lowercase](06.md)             |
| 7   | `my_abs`     | [Absolute value](07.md)        |
| 8   | `clamp_int`  | [Clamp an integer](08.md)      |
| 9   | `swap_int`   | [Swap through pointers](09.md) |
| 10  | `array_sum`  | [Sum an array](10.md)          |

## 2 — Strings and pointer traversal

| #   | Function         | Instructions                   |
| --- | ---------------- | ------------------------------ |
| 11  | `my_strlen`      | [String length](11.md)         |
| 12  | `my_strcpy`      | [Copy a string](12.md)         |
| 13  | `my_strcmp`      | [Compare strings](13.md)       |
| 14  | `my_strchr`      | [First character match](14.md) |
| 15  | `my_strrchr`     | [Last character match](15.md)  |
| 16  | `my_strcat`      | [Append a string](16.md)       |
| 17  | `my_strncmp`     | [Bounded comparison](17.md)    |
| 18  | `my_strncpy`     | [Copy and pad](18.md)          |
| 19  | `my_strncat`     | [Bounded append](19.md)        |
| 20  | `reverse_string` | [Reverse in place](20.md)      |

## 3 — Raw memory and searching

| #   | Function     | Instructions                     |
| --- | ------------ | -------------------------------- |
| 21  | `my_memset`  | [Fill memory](21.md)             |
| 22  | `my_memcpy`  | [Copy memory](22.md)             |
| 23  | `my_memmove` | [Move overlapping memory](23.md) |
| 24  | `my_memcmp`  | [Compare memory](24.md)          |
| 25  | `my_memchr`  | [Search memory](25.md)           |
| 26  | `my_strstr`  | [Find a substring](26.md)        |
| 27  | `my_strspn`  | [Accepted prefix](27.md)         |
| 28  | `my_strcspn` | [Rejected prefix](28.md)         |
| 29  | `my_strpbrk` | [Match any character](29.md)     |
| 30  | `next_token` | [Stateful tokenization](30.md)   |

## 4 — Conversion and dynamic allocation

| #   | Function          | Instructions                       |
| --- | ----------------- | ---------------------------------- |
| 31  | `my_atoi`         | [Decimal prefix conversion](31.md) |
| 32  | `parse_int`       | [Checked integer parsing](32.md)   |
| 33  | `int_to_string`   | [Integer to decimal string](33.md) |
| 34  | `parse_uint_base` | [Parse a selected base](34.md)     |
| 35  | `my_strdup`       | [Duplicate a string](35.md)        |
| 36  | `my_calloc`       | [Allocate zeroed memory](36.md)    |
| 37  | `array_clone`     | [Clone an integer array](37.md)    |
| 38  | `string_slice`    | [Allocate a slice](38.md)          |
| 39  | `string_join`     | [Join strings](39.md)              |
| 40  | `string_split`    | [Split with ownership](40.md)      |

## 5 — Small systems-building blocks

| #   | Function         | Instructions                        |
| --- | ---------------- | ----------------------------------- |
| 41  | `bit_count`      | [Count set bits](41.md)             |
| 42  | `read_u32_le`    | [Decode little-endian bytes](42.md) |
| 43  | `write_u32_le`   | [Encode little-endian bytes](43.md) |
| 44  | `array_sort`     | [Insertion sort](44.md)             |
| 45  | `my_bsearch`     | [Generic binary search](45.md)      |
| 46  | `vector_push`    | [Growable integer vector](46.md)    |
| 47  | `buffer_append`  | [Growable byte buffer](47.md)       |
| 48  | `list_push_back` | [Append a linked-list node](48.md)  |
| 49  | `list_remove_if` | [Remove matching list nodes](49.md) |
| 50  | `read_line`      | [Read an owned line](50.md)         |
