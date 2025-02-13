#!/bin/bash
# SPDX-FileCopyrightText: 2025 smdn <smdn@smdn.jp>
# SPDX-License-Identifier: MIT

# Note: This script was based on the answers provided by Microsoft Copilot.

base_dir=$(cd $(dirname $0) && pwd)
pio_project_dirs=()

# get a list of directories where platformio.ini exists
for dir in "$base_dir"/*; do
  if [ -d "$dir" ]; then
    if ls "$dir"/platformio.ini > /dev/null 2>&1; then
      pio_project_dirs+=("$dir")
    fi
  fi
done

# build each projects
for dir in "${pio_project_dirs[@]}"; do
  pio run --project-dir "$dir"
done