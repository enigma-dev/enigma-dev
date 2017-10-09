#!/bin/bash +x

export GCC_COLORS='error=01;31:warning=01;35:note=01;36:caret=01;32:locus=01:quote=01'
./emake --compiler ${COMPILER} \
        --mode ${MODE} \
        --platform ${PLATFORM} \
        --graphics ${GRAPHICS} \
        --audio ${AUDIO} \
        --collision ${COLLISION} \
        --network ${NETWORK} \
        --widgets ${WIDGETS} \
        --extensions ${EXTENSIONS} \
        -o ${OUTPUT}
