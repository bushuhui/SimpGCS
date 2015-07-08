
#include <rtk_types.h>
#include <rtk_datastream.h>

namespace rtk {


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

int datastream_get_header(ru8 *buf, ru32 &magic, ru32 &ver)
{
    ru32 mv;

    memcpy(&mv, buf, sizeof(ru32));

    ver   = mv & 0x0000FFFF;
    magic = mv >> 16;

    return 0;
}

ru32 datastream_get_length(ru8 *buf)
{
    ru32 len;

    memcpy(&len, buf+4, sizeof(ru32));

    return len;
}

int datastream_set_header(ru8 *buf, ru32 magic, ru32 ver, ru32 len)
{
    ru32 mv;

    mv = magic << 16 | (ver & 0x0000FFFF);

    memcpy(buf,   &mv,  sizeof(ru32));
    memcpy(buf+4, &len, sizeof(ru32));

    return 0;
}

} // end of namespace rtk

