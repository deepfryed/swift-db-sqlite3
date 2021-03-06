// vim:ts=4:sts=4:sw=4:expandtab

// (c) Bharanee Rathna 2012

#include "common.h"
#include "uuid/uuid.h"

VALUE rb_uuid_string() {
    size_t n;
    uuid_t uuid;
    char uuid_hex[sizeof(uuid_t) * 2 + 1];

    uuid_generate(uuid);

    memset(uuid_hex, 0, sizeof(uuid_hex));
    for (n = 0; n < sizeof(uuid_t); n++)
        sprintf(uuid_hex + n * 2, "%02x", uuid[n]);

    return rb_str_new(uuid_hex, sizeof(uuid_t) * 2);
}
