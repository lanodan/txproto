#include <libavutil/crc.h>

#include "iosys_common.h"
#include "utils.h"
#include "logging.h"
#include "../config.h"

AVBufferRef *sp_bufferlist_iosysentry_by_id(AVBufferRef *ref, void *opaque)
{
    IOSysEntry *entry = (IOSysEntry *)ref->data;
    if (entry->identifier == *((uint32_t *)opaque))
        return ref;
    return NULL;
}

uint32_t sp_iosys_gen_identifier(void *ctx, uint32_t num, uint32_t extra)
{
    const AVCRC *table = av_crc_get_table(AV_CRC_32_IEEE);

    uint32_t crc = UINT32_MAX;

    if (ctx) {
        uint32_t id = sp_class_get_type(ctx);
        crc = av_crc(table, crc, (void *)&id, sizeof(id));
    }

    crc = av_crc(table, crc, (void *)&num, sizeof(num));
    crc = av_crc(table, crc, (void *)&extra, sizeof(extra));
    return crc;
}

#ifdef HAVE_LAVD
extern const IOSysAPI src_lavd;
#endif

#ifdef HAVE_PULSEAUDIO
extern const IOSysAPI src_pulse;
#endif

#ifdef HAVE_WAYLAND
extern const IOSysAPI src_wayland;
#endif

const IOSysAPI *sp_compiled_apis[] = {
#ifdef HAVE_LAVD
    &src_lavd,
#endif
#ifdef HAVE_PULSEAUDIO
    &src_pulse,
#endif
#ifdef HAVE_WAYLAND
    &src_wayland,
#endif
};

const int sp_compiled_apis_len = SP_ARRAY_ELEMS(sp_compiled_apis);
