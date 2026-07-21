#!/bin/bash
# Copyright (c) 2026 Qualcomm Innovation Center, Inc. All rights reserved.
# SPDX-License-Identifier: MIT
#
# Publish the objects a CI job built into the S3-backed sstate cache with a
# one-way, additive `aws s3 sync`, then tag them for lifecycle expiry. Runs on
# the runner host (not inside the kas container). Reads from the environment:
#   SSTATE_DIR        job-local sstate dir the build wrote into (source)
#   SSTATE_S3_DEST    s3://<bucket>/<prefix>/sstate-cache (destination)
#   SSTATE_RETENTION  retention tag value (e.g. 15d)
#   RUNNER_TEMP       scratch dir for the sync log
set -eo pipefail

[ -d "${SSTATE_DIR}" ] || exit 0

# Mirror hits land in the job-local SSTATE_DIR as symlinks into the read-only
# mirror mount, so --no-follow-symlinks uploads only the objects this job
# built. --size-only avoids re-uploading objects whose mtime changed from
# sstate cache-hit touches. One-way and additive: no --delete, nothing is
# removed from S3.
aws s3 sync "${SSTATE_DIR}/" "${SSTATE_S3_DEST}/" \
    --no-follow-symlinks --size-only --no-progress \
    | tee "${RUNNER_TEMP}/sstate-sync.log"

# Tag the uploaded objects with the same retention scheme used for the S3
# build artifacts so the bucket lifecycle expires them; an expired object is
# transparently rebuilt and re-uploaded (with a fresh tag) by the next job
# that needs it.
bucket="${SSTATE_S3_DEST#s3://}"; bucket="${bucket%%/*}"
awk -v p="s3://${bucket}/" '$1 == "upload:" { print substr($NF, length(p) + 1) }' \
    "${RUNNER_TEMP}/sstate-sync.log" \
    | xargs -r -P 16 -I '{}' aws s3api put-object-tagging \
        --bucket "${bucket}" --key '{}' \
        --tagging "TagSet=[{Key=retention,Value=${SSTATE_RETENTION}}]"
