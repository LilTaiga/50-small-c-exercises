"""Run one test executable with a time limit and useful crash diagnostics."""

import os
from pathlib import Path
import subprocess
import sys


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
        result = subprocess.run([str(Path(sys.argv[1]).resolve())], timeout=timeout)
    except subprocess.TimeoutExpired:
        print(f"FAIL: test timed out after {timeout:g}s (possible infinite loop).", flush=True)
        return 1
    except OSError as error:
        print(f"Cannot run tests: {error}", file=sys.stderr)
        return 2
    if result.returncode < 0:
        import signal

        print(f"FAIL: test process crashed: {signal.Signals(-result.returncode).name}.")
        print("The last input printed above identifies the active check.")
        print("Try make test N=... SANITIZE=1 for memory diagnostics.")
        return 1
    return result.returncode


if __name__ == "__main__":
    sys.exit(main())
