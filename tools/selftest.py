"""Regression tests for the harness, independent of student implementations."""

import os
from pathlib import Path
import shlex
import subprocess
import tempfile
import unittest

ROOT = Path(__file__).resolve().parent.parent


class HarnessTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        (ROOT / "build").mkdir(exist_ok=True)
        cls.directory = tempfile.TemporaryDirectory(prefix="selftest-", dir=ROOT / "build")
        cls.binary = Path(cls.directory.name) / "fixture"
        compiler = shlex.split(os.environ.get("CC", "cc"))
        flags = shlex.split(os.environ.get("CFLAGS", "-std=c11 -Wall -Wextra -Wpedantic -Werror"))
        sources = ["tests/support.c", "tests/framework_fixture.c"]
        includes = ["-Iinclude", "-Itests"]
        # Check types/syntax first, then build the executable.
        subprocess.run(compiler + flags + includes + ["-fsyntax-only"] + sources, cwd=ROOT, check=True)
        subprocess.run(compiler + flags + includes + sources + ["-o", str(cls.binary)], cwd=ROOT, check=True)

    @classmethod
    def tearDownClass(cls):
        cls.directory.cleanup()

    def run_fixture(self, mode, timeout="2"):
        env = {**os.environ, "FRAMEWORK_MODE": mode, "TEST_TIMEOUT": timeout}
        return subprocess.run(
            [os.sys.executable, "tools/run_tests.py", str(self.binary)],
            cwd=ROOT, env=env, text=True, capture_output=True, timeout=10,
        )

    def test_correct_assertions_pass(self):
        result = self.run_fixture("pass")
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("PASS: 11/11 checks", result.stdout)
        self.assertNotIn("Case ", result.stdout)
        self.assertNotIn("input:", result.stdout)

    def test_only_failing_cases_are_shown(self):
        result = self.run_fixture("mixed")
        self.assertEqual(result.returncode, 1)
        self.assertIn("input: failing middle", result.stdout)
        self.assertNotIn("input: passing before", result.stdout)
        self.assertNotIn("input: passing after", result.stdout)
        self.assertNotIn("input: framework fixture", result.stdout)
        self.assertEqual(result.stdout.count("Case "), 1)
        self.assertEqual(result.stdout.count("    FAIL line "), 2)
        self.assertIn("FAIL: 2/4 checks passed across 4 cases.", result.stdout)

    def test_boolean_failures_use_only_true_and_false(self):
        result = self.run_fixture("bool-fail")
        self.assertEqual(result.returncode, 1)
        self.assertIn("expected: true\n      actual:   false", result.stdout)
        self.assertIn("expected: false\n      actual:   true", result.stdout)
        self.assertNotIn("byte", result.stdout)
        self.assertNotIn("0x", result.stdout)
        self.assertNotIn("0b", result.stdout)
        self.assertIn("FAIL: 0/3 checks", result.stdout)

    def test_boolean_checks_accept_any_nonzero_true_value(self):
        result = self.run_fixture("bool-pass")
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)
        self.assertIn("PASS: 4/4 checks", result.stdout)
        self.assertNotIn("Case ", result.stdout)

    def test_failed_inputs_remain_escaped(self):
        result = self.run_fixture("escaped-input")
        self.assertEqual(result.returncode, 1)
        self.assertIn('input: "a\\nb\\t\\\"\\\\"', result.stdout)
        self.assertIn('a="\\xFF", b="\\x7F"', result.stdout)
        self.assertIn('a="a\\nb", b="a\\tb", count=2', result.stdout)

    def test_interrupted_cases_are_shown_without_passing_cases(self):
        for mode in ("mixed-crash", "mixed-hang"):
            with self.subTest(mode=mode):
                result = self.run_fixture(mode, "0.1" if mode == "mixed-hang" else "2")
                self.assertEqual(result.returncode, 1)
                self.assertIn("input: failing middle", result.stdout)
                self.assertIn("input: active input before interruption", result.stdout)
                self.assertNotIn("input: passing before", result.stdout)
                self.assertNotIn("input: framework fixture", result.stdout)

    def test_stderr_diagnostics_remain_visible(self):
        result = self.run_fixture("stderr")
        self.assertEqual(result.returncode, 0)
        self.assertIn("diagnostic on stderr", result.stderr)
        self.assertNotIn("Case ", result.stdout)

    def test_failures_have_multiple_readable_representations(self):
        result = self.run_fixture("fail")
        self.assertEqual(result.returncode, 1)
        for marker in ("expected:", "actual:", "'A'", "0x41", "0b10100101",
                       '"a\\n\\t\\\\\\\""', "42 0A 7F", "|B..|", "[1, 9, 3]",
                       "first differing index: 1", "buffer + 1", "buffer + 2", "NULL",
                       "PARSE_OK (0)", "PARSE_OVERFLOW (2)", "FAIL: 0/12 checks",
                       "first differing byte: 79 (expected 0x00, actual 0xFF)",
                       "first differing index: 39 (expected 0, actual 42)"):
            self.assertIn(marker, result.stdout)

    def test_crashes_are_failures(self):
        result = self.run_fixture("crash")
        self.assertEqual(result.returncode, 1)
        self.assertIn("SIGSEGV", result.stdout)
        self.assertIn("input: framework fixture", result.stdout)

    def test_infinite_loops_time_out(self):
        result = self.run_fixture("hang", "0.1")
        self.assertEqual(result.returncode, 1)
        self.assertIn("timed out", result.stdout)
        self.assertIn("input: framework fixture", result.stdout)

    def test_exit_status_is_preserved(self):
        self.assertEqual(self.run_fixture("exit").returncode, 7)

    def test_invalid_timeouts_are_rejected(self):
        for value in ("0", "-1", "nan", "inf", "nope"):
            with self.subTest(value=value):
                result = self.run_fixture("pass", value)
                self.assertEqual(result.returncode, 2)
                self.assertIn("positive finite", result.stderr)

    def test_missing_executable_is_reported(self):
        result = subprocess.run(
            [os.sys.executable, "tools/run_tests.py", str(self.binary) + "-missing"],
            cwd=ROOT, text=True, capture_output=True,
        )
        self.assertEqual(result.returncode, 2)
        self.assertIn("Cannot run tests:", result.stderr)

    def test_runner_requires_one_executable_argument(self):
        result = subprocess.run(
            [os.sys.executable, "tools/run_tests.py"], cwd=ROOT, text=True, capture_output=True,
        )
        self.assertEqual(result.returncode, 2)
        self.assertIn("Usage:", result.stderr)

    def test_invalid_exercise_numbers_are_rejected(self):
        for value in (None, "0", "51", "01", "abc", "1 2", "%", "1%"):
            command = ["make", "--no-print-directory", "-n", "test"]
            if value is not None:
                command.append(f"N={value}")
            result = subprocess.run(command, cwd=ROOT, text=True, capture_output=True)
            with self.subTest(value=value):
                self.assertNotEqual(result.returncode, 0)
                self.assertIn("N must be", result.stderr)

    def make_with_mock_compiler(self, goal, verbose=False, fail=False):
        # Exercise Make's command echoing without depending on student code.
        code = "import sys; print('compiler diagnostic', file=sys.stderr)"
        if fail:
            code += "; sys.exit(1)"
        compiler = shlex.join([os.sys.executable, "-c", code])
        return subprocess.run(
            ["make", "--no-print-directory", goal,
             f"BUILD={self.directory.name}", "N=1", f"CC={compiler}",
             f"V={int(verbose)}"],
            cwd=ROOT, text=True, capture_output=True, timeout=10,
        )

    def test_compilation_is_quiet_by_default(self):
        for goal in (str(Path(self.directory.name) / "01-run"),
                     str(Path(self.directory.name) / "01-test"), "check"):
            with self.subTest(goal=goal):
                result = self.make_with_mock_compiler(goal)
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertNotIn("-Iinclude", result.stdout)
                self.assertIn("compiler diagnostic", result.stderr)

    def test_verbose_flag_shows_compilation_commands(self):
        for goal in (str(Path(self.directory.name) / "01-run"),
                     str(Path(self.directory.name) / "01-test"), "check"):
            with self.subTest(goal=goal):
                result = self.make_with_mock_compiler(goal, verbose=True)
                self.assertEqual(result.returncode, 0, result.stderr)
                self.assertIn("-Iinclude", result.stdout)
                self.assertIn("compiler diagnostic", result.stderr)

    def test_compiler_failures_remain_visible_in_quiet_mode(self):
        result = self.make_with_mock_compiler(str(Path(self.directory.name) / "01-run"), fail=True)
        self.assertNotEqual(result.returncode, 0)
        self.assertNotIn("-Iinclude", result.stdout)
        self.assertIn("compiler diagnostic", result.stderr)

    def test_all_exercises_have_playgrounds_and_instructions(self):
        for number in range(1, 51):
            source = (ROOT / "exercises" / f"{number:02}.c").read_text()
            instructions = (ROOT / "exercises" / f"{number:02}.md").read_text()
            with self.subTest(number=number):
                self.assertIn("#ifndef EXERCISE_TEST", source)
                self.assertIn("int main(void)", source)
                self.assertIn(f"make run N={number}", instructions)
                self.assertIn(f"make test N={number}", instructions)

    def test_editor_flags_resolve_project_headers_and_fixed_width_types(self):
        source = Path(self.directory.name) / "editor-headers.c"
        source.write_text(
            '#include "exercises.h"\n'
            '#include "lib.h"\n'
            '#include "support.h"\n'
            'uint32_t editor_value = UINT32_C(42);\n'
        )
        compiler = shlex.split(os.environ.get("CC", "cc"))
        flags = (ROOT / "compile_flags.txt").read_text().splitlines()
        result = subprocess.run(
            compiler + flags + ["-Werror", "-fsyntax-only", str(source)],
            cwd=ROOT, text=True, capture_output=True,
        )
        self.assertEqual(result.returncode, 0, result.stdout + result.stderr)


if __name__ == "__main__":
    unittest.main(verbosity=2)
