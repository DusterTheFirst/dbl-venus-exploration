#pragma once

#include <Arduino.h>
#include <stdint.h>

namespace chunk {
    struct Chunk {
        const bool is_progmem;
        const size_t len;
        const uint8_t *const ptr;

        template <size_t COUNT>
        explicit Chunk(bool is_progmem, const uint8_t (&bytes)[COUNT])
            : is_progmem(is_progmem), len(COUNT), ptr(bytes) {}

        explicit Chunk(bool is_progmem, const uint8_t *ptr, size_t len)
            : is_progmem(is_progmem), len(len), ptr(ptr) {}
    };

    struct Chunks {
     private:
        const size_t chunks_len;
        const Chunk *const chunks;

     public:
        template <size_t COUNT>
        explicit Chunks(const Chunk (&chunks)[COUNT])
            : chunks_len(COUNT), chunks(chunks) {}

        inline uint8_t get_at(size_t offset) const {
            // Find the chunk that the item is in
            for (size_t chunk = 0; chunk < chunks_len; chunk++) {
                size_t items_len = chunks[chunk].len;

                if (offset < items_len) {
                    auto chunk_ref = &chunks[chunk];

                    if (chunk_ref->is_progmem) {
                        // Read from progmem if chunk is in progmem
                        return pgm_read_byte(&(chunk_ref->ptr[offset]));
                    } else {
                        // Read normally if not
                        return chunk_ref->ptr[offset];
                    }
                }

                offset -= items_len;
            }

            // Return last item if reached the end
            auto last_chunk = chunks[chunks_len - 1];
            return last_chunk.ptr[last_chunk.len - 1];
        }

        inline size_t len() const {
            size_t len = 0;
            for (size_t chunk = 0; chunk < chunks_len; chunk++) {
                len += chunks[chunk].len;
            }
            return len;
        }
    };
}
