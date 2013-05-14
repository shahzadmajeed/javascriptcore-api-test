/*
 * Copyright (C) 2006, 2007, 2008 Apple Inc. All rights reserved.
 *           (C) 2008, 2009 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 * Copyright (C) 2013 Andrew Bortz. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "RandomNumber.h"

#include "CryptographicallyRandomNumber.h"
#include "RandomNumberSeed.h"

#include <limits>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

namespace WTF {

#if !USE(OS_RANDOMNESS)
namespace Internal {

static uint64_t state;

void initializeRandomNumber(uint64_t seed)
{
    state = seed;
}

// This random number generator comes from: Klimov, A. and Shamir, A.,
// "A New Class of Invertible Mappings", Cryptographic Hardware and Embedded
// Systems 2002, http://dl.acm.org/citation.cfm?id=752741
//
// Very fast, very simple, and passes Diehard and other good statistical
// tests as strongly as cryptographically-secure random number generators (but
// is not itself cryptographically-secure).
uint32_t randomNumber()
{
    state += (state * state) | 5;
    return static_cast<uint32_t>(state >> 32);
}

}
#endif

double randomNumber()
{
#if USE(OS_RANDOMNESS)
    uint32_t bits = cryptographicallyRandomNumber();
#else
    uint32_t bits = Internal::randomNumber();
#endif
    return static_cast<double>(bits) / (static_cast<double>(std::numeric_limits<uint32_t>::max()) + 1.0);
}

}