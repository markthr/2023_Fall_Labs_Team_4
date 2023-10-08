#include <stdlib.h>
#include <stdexcept>
#include "line_det.h"

Line_Detector::Line_Detector(int det_size, int buf_len) 
    :det_size(det_size), buffer(NULL), n_buf(buf_len), i_buf(0), buffer_init(false) {
        buffer = (int*)calloc(det_size*buf_len, sizeof(int));

        if(buffer == NULL) {
            throw new std::runtime_error("Calloc failed");
        }
        else if(det_size < 1 || buf_len < 1) {
            throw new std::invalid_argument("Detector size and buffer length must be at least 1");
        }
    }

Line_Detector::~Line_Detector() {
    if(buffer != NULL) {
        free(buffer);
        buffer = NULL;
    }
}

void Line_Detector::accept(const int det_vals[]) {
    /*
     * Circular array using the length and the index. The index, i_buf, always points to the most recently added value.
     * The entry at (i_buf+n_buf+1)%n_buf will be the second most recently added value.
     * The entry at (i_buf+n_buf-1)%n_buf will be the second most recently added value.
     * Adding n_buf does not affect positive offsets, but it has -1 point to the last element in the array.
     */
    if(!buffer_init) {
        // buffer is currently full of zeros, completely fill it with the first recorded value
        
        i_buf = n_buf - 1; // arbitrary, might as well start from the bottom and work up
    }
    else {
        i_buf = (i_buf + n_buf - 1)%n_buf;
    }
}

