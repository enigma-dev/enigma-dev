#!/bin/bash +x

ENIGMA_DEBUG=TRUE ./emake --compiler "${COMPILER}" \
        --mode "${MODE}" \
        --platform "${PLATFORM}" \
        --graphics "${GRAPHICS}" \
        --audio "${AUDIO}" \
        --collision "${COLLISION}" \
        --network "${NETWORK}" \
        --widgets "${WIDGETS}" \
        --extensions "${EXTENSIONS}" \
        -o "${OUTPUT}" \
        "CommandLine/testing/SimpleTests/clean_exit.sog/"
