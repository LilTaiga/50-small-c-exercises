"""Run one test executable with a time limit and useful crash diagnostics."""

import os
from pathlib import Path
import re
import subprocess
import sys


def report_output(stdout, stderr, interrupted=False):
    """Keep failing case blocks, plus the active case if execution stopped."""
    output = []
    current = []
    failed = False
    for line in (stdout or b"").decode("utf-8", errors="replace").splitlines(keepends=True):
        if re.match(r"^  Case \d+ \| input:", line):
            if failed:
                output.extend(current)
            current = [line]
            failed = False
        elif line.startswith(("PASS:", "FAIL:")):
            if failed:
                output.extend(current)
            current = []
            failed = False
            output.append(line)
        elif current:
            current.append(line)
            if line.startswith("    FAIL line "):
                failed = True
        else:
            output.append(line)
    if failed or interrupted:
        output.extend(current)
    sys.stdout.write("".join(output))
    sys.stdout.flush()
    sys.stderr.write((stderr or b"").decode("utf-8", errors="replace"))
    sys.stderr.flush()


def main():
    if len(sys.argv) != 2:
        print("Usage: run_tests.py EXECUTABLE", file=sys.stderr)
        return 2
    try:
        timeout = float(os.environ.get("TEST_TIMEOUT", "5"))
        if not 0 < timeout < float("inf"):
            raise ValueError
    except ValueError:
        print("TEST_TIMEOUT must be a positive finite number of seconds.", file=sys.stderr)
        return 2
    try:
        result = subprocess.run([str(Path(sys.argv[1]).resolve())], timeout=timeout, capture_output=True)
    except subprocess.TimeoutExpired as error:
        report_output(error.stdout, error.stderr, interrupted=True)
        print(f"FAIL: test timed out after {timeout:g}s (possible infinite loop).", flush=True)
        return 1
    except OSError as error:
        print(f"Cannot run tests: {error}", file=sys.stderr)
        return 2
    report_output(result.stdout, result.stderr, interrupted=result.returncode != 0)
    if result.returncode < 0:
        import signal

        print(f"FAIL: test process crashed: {signal.Signals(-result.returncode).name}.")
        print("The last input printed above identifies the active check.")
        print("Try make test N=... SANITIZE=1 for memory diagnostics.")
        return 1
    return result.returncode


if __name__ == "__main__":
    sys.exit(main())
