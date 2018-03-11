#ifndef EXAMPLE_MSG_H
#define EXAMPLE_MSG_H

struct msg {
  char text[256];
};

#define GET_SIZE(msg) (strlen((msg)->text)+sizeof('\0'))

#endif	/* EXAMPLE_MSG_H */
