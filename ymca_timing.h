struct timedtext
{
    double start;
    const char *text;
};

/* These are the letters in "YMCA", each followed by a newline and
   accompanied by the time (in seconds) it occurs after the "Y". */
#define YMCA(start) \
{(start) + 0.000, "Y\n"}, \
{(start) + 0.930, "M\n"}, \
{(start) + 1.395, "C\n"}, \
{(start) + 1.628, "A\n"}

/* These are all times the word "YMCA" occurs in the song's chorus. */
static const struct timedtext ymca_text[] = {
    YMCA(45.766), YMCA(49.611),
    YMCA(60.889), YMCA(64.661),
    YMCA(109.816), YMCA(113.591),
    YMCA(124.810), YMCA(128.687),
    YMCA(173.830), YMCA(177.620),
    YMCA(188.950), YMCA(192.724),
    YMCA(204.013), YMCA(207.739),
    YMCA(219.057), {0.000, NULL}
};
