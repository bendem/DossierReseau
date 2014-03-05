unsigned short cksum(void *ipt, int len) {
    /* assume 32 bit long, 16 bit short */
    long sum = 0;
    unsigned short *ip;
    ip = ipt;

    while (len > 1) {
        sum +=*(ip)++;
        /* if hogh-order bit set, fold */
        if (sum & 0x80000000) {
            sum = (sum & 0xFFFF) + (sum >> 16);
        }
        len -= 2;
    }
    /* take care of left over byte */
    if (len) {
        sum += (unsigned short) *(unsigned char *) ip;
    }

    while(sum >> 16) {
        sum = (sum & 0xFFFF) + (sum >> 16);
    }

    return ~sum;
}
