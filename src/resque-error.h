/*
 * Copyright (c) 2009 Ryan Phillips
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef RESQUE_ERROR_H_
#define RESQUE_ERROR_H_

typedef struct {
    unsigned int line;
    const char *file;
    char *msg;
} resque_error_t;

#define RESQUE_SUCCESS (NULL)

#define resque_error_create(id, err, msg) resque_error_create_impl(err,      \
                                                       msg,      \
                                                       __LINE__, \
                                                       __FILE__)

#define resque_error_createf(id, err, fmt, ...) resque_error_createf_impl(err,      \
                                                              __LINE__, \
                                                              __FILE__, \
                                                              fmt,      \
                                                              __VA_ARGS__)

resque_error_t*
resque_error_create_impl(const char *msg,
                         unsigned int line,
                         const char *file);

resque_error_t*
resque_error_createf_impl(unsigned int line,
                          const char *file, 
                          const char *fmt,
                          ...);

void
resque_error_destroy(resque_error_t *err);

#endif
