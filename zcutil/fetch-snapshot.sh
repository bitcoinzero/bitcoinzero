#!/bin/bash

set -eu

SNAPSHOT_DIR="$HOME/.bitcoinzero"

SNAPSHOT_NAME='bitcoinzero-snapshot.tar.gz'

SNAPSHOT_URL="http://182.61.33.29:8008/download/"

SHA256CMD="$(command -v sha256sum || echo shasum)"
SHA256ARGS="$(command -v sha256sum >/dev/null || echo '-a 256')"

WGETCMD="$(command -v wget || echo '')"

# fetch methods can be disabled with ZC_DISABLE_SOMETHING=1
ZC_DISABLE_WGET="${ZC_DISABLE_WGET:-}"
# ZC_DISABLE_IPFS="${ZC_DISABLE_IPFS:-}"
ZC_DISABLE_CURL="${ZC_DISABLE_CURL:-}"

function fetch_wget {
    if [ -z "$WGETCMD" ] || ! [ -z "$ZC_DISABLE_WGET" ]; then
        return 1
    fi

    local filename="$1"
    local dlname="$2"

    cat <<EOF

Retrieving (wget): $SNAPSHOT_URL/$filename
EOF

    wget \
        --progress=dot:giga \
        --output-document="$dlname" \
        --continue \
        --retry-connrefused --waitretry=3 --timeout=30 \
        "$SNAPSHOT_URL/$filename"
}

# function fetch_ipfs {
#     if [ -z "$IPFSCMD" ] || ! [ -z "$ZC_DISABLE_IPFS" ]; then
#         return 1
#     fi

#     local filename="$1"
#     local dlname="$2"

#     cat <<EOF

# Retrieving (ipfs): $SNAPSHOT_IPFS/$filename
# EOF

#     ipfs get --output "$dlname" "$SNAPSHOT_IPFS/$filename"
# }

function fetch_curl {
    if [ -z "$CURLCMD" ] || ! [ -z "$ZC_DISABLE_CURL" ]; then
        return 1
    fi

    local filename="$1"
    local dlname="$2"

    cat <<EOF

Retrieving (curl): $SNAPSHOT_URL/$filename
EOF

    curl \
        --output "$dlname" \
        -# -L -C - \
        "$SNAPSHOT_URL/$filename"

}

function fetch_failure {
    cat >&2 <<EOF

Failed to fetch the Bitcoin Zero chainstate snapshot!
Try installing one of the following programs and make sure you're online:

 * wget
 * curl

EOF
    exit 1
}

function fetch_SNAPSHOT {
    local filename="$1"
    local output="$2"
    local dlname="${output}.dl"
    local expectedhash="$3"

    if ! [ -f "$output" ]
    then
        for method in wget ipfs curl failure; do
            if "fetch_$method" "$filename" "$dlname"; then
                echo "Download successful!"
                break
            fi
        done

        "$SHA256CMD" $SHA256ARGS -c <<EOF
$expectedhash  $dlname
EOF

        # Check the exit code of the shasum command:
        CHECKSUM_RESULT=$?
        if [ $CHECKSUM_RESULT -eq 0 ]; then
            mv -v "$dlname" "$output"
        else
            echo "Failed to verify parameter checksums!" >&2
            exit 1
        fi
    fi
}

# Use flock to prevent parallel execution.
function lock() {
    local lockfile=/tmp/fetch_SNAPSHOT.lock
    if [[ "$OSTYPE" == "darwin"* ]]; then
        if shlock -f ${lockfile} -p $$; then
            return 0
        else
            return 1
        fi
    else
        # create lock file
        eval "exec 200>/$lockfile"
        # acquire the lock
        flock -n 200 \
            && return 0 \
            || return 1
    fi
}

function exit_locked_error {
    echo "Only one instance of fetch-snapshot.sh can be run at a time." >&2
    exit 1
}

function main() {

    lock fetch-snapshot.sh \
    || exit_locked_error

    cat <<EOF
Bitcoin Zero - fetch-snapshot.sh

This script will fetch the Bitcoin Zero chainstate snapshot and verify their
integrity with sha256sum.

If they already exist locally, it will exit now and do nothing else.
EOF

    if [ -d "$SNAPSHOT_DIR/blocks" ]
    then cat <<EOF
"$SNAPSHOT_DIR/blocks" exists. If you mean to re-sync the chain from the genesis. Please delete the folder before retry.  
EOF
    return
    fi
    # Now create SNAPSHOT_DIR and insert a README if necessary:
    if ! [ -d "$SNAPSHOT_DIR" ]
    then
        mkdir -p "$SNAPSHOT_DIR"
        README_PATH="$SNAPSHOT_DIR/README"
        cat >> "$README_PATH" <<EOF
This directory stores common Bitcoin Zero chainstate snapshot. Note that this is
the default datadir. You may change it corresponding to your -datadir parameter.
EOF

        # This may be the first time the user's run this script, so give
        # them some info, especially about bandwidth usage:
        cat <<EOF
The snapshot is currently about 400MB in size, so plan accordingly
for your bandwidth constraints. If the files are already present and
have the correct sha256sum, no networking is used.

Creating SNAPSHOT directory. For details about this directory, see:
$README_PATH

EOF
    fi

    cd "$SNAPSHOT_DIR"

    fetch_SNAPSHOT "$SNAPSHOT_NAME" "$SNAPSHOT_DIR/$SNAPSHOT_NAME" "a557e20e7efed853f1e819d1a45b92a2e54e1b18a33fa8513915fe4bc6ac77c1"
    tar -zxvf "$SNAPSHOT_DIR/$SNAPSHOT_NAME" -C "$SNAPSHOT_DIR"
}

main
rm -f /tmp/fetch_SNAPSHOT.lock
exit 0
