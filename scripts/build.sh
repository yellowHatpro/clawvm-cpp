#!/usr/bin/env bash
set -euo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$repo_root"

mode="build"
if [[ "${1:-}" == "--run" ]]; then
  mode="run"
elif [[ "${1:-}" != "" ]]; then
  echo "usage: ./scripts/build.sh [--run]" >&2
  exit 2
fi

compiler="$(command -v g++ || command -v clang++ || echo c++)"
mapfile -t sources < <(find src -name '*.cpp' | sort)

bazel_java_home="${CLAWVM_BAZEL_JAVA_HOME:-${JAVA_HOME:-}}"
if [[ -x /usr/lib/jvm/java-21-openjdk/bin/java ]]; then
  bazel_java_home="${CLAWVM_BAZEL_JAVA_HOME:-/usr/lib/jvm/java-21-openjdk}"
fi

bazel_startup_args=()
if [[ -n "$bazel_java_home" ]]; then
  export JAVA_HOME="$bazel_java_home"
  export PATH="$JAVA_HOME/bin:$PATH"
  bazel_startup_args+=(--server_javabase="$JAVA_HOME")
fi

{
  printf '[\n'
  for i in "${!sources[@]}"; do
    src="${sources[$i]}"
    comma=","
    if [[ "$i" == "$((${#sources[@]} - 1))" ]]; then
      comma=""
    fi
    printf '  {\n'
    printf '    "directory": "%s",\n' "$repo_root"
    printf '    "command": "%s -std=c++20 -Iinclude -c %s",\n' "$compiler" "$src"
    printf '    "file": "%s/%s"\n' "$repo_root" "$src"
    printf '  }%s\n' "$comma"
  done
  printf ']\n'
} > compile_commands.json

if command -v bazel >/dev/null 2>&1; then
  bazel_cmd="bazel"
elif command -v bazelisk >/dev/null 2>&1; then
  bazel_cmd="bazelisk"
else
  echo "error: bazel or bazelisk is required to build this project." >&2
  echo "Updated compile_commands.json for LSP support." >&2
  echo "Install Bazel, then rerun: ./scripts/build.sh" >&2
  exit 127
fi

"$bazel_cmd" "${bazel_startup_args[@]}" "$mode" //:clawvm

if [[ "$mode" == "build" ]]; then
  echo "Built //:clawvm"
  echo "Run with: bazel run //:clawvm"
  echo "Binary path: bazel-bin/clawvm"
fi
