#ifndef __SHELL_H__
#define __SHELL_H__

// bad form, but I don't want to have the include.
enum { TRACE_FD_REPLAY = 11, TRACE_FD_HANDOFF };

#define note(msg...) do {               \
        fprintf(stderr, "PIX:");        \
        fprintf(stderr, ##msg);         \
} while(0)

#define err(msg...) do {                                                \
        fprintf(stderr, "PIX:ERROR:%s:%s:%d:", __FILE__, __FUNCTION__, __LINE__); \
        fprintf(stderr, ##msg);                                         \
        exit(1);                                                        \
} while(0)
void put_uint(int fd, unsigned u);
unsigned get_uint(int fd);
// used to echo output from the pi until we see <msg>
int echo_until(int fd, const char *msg);

// split <buf> into null terminated tokens.
int tokenize(char **argv, int max_args, char *buf);
static void send_prog(int fd, const char *name);
unsigned crc32(const void *buf, unsigned size);

#endif
