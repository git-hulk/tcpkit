#!/usr/bin/env python3
"""Replays a busy capture while hammering the stats endpoint.

Meant to be run against a binary built with -fsanitize=thread: the stats
thread walks the endpoint table that the capture thread is still filling in,
so any missing synchronisation shows up as a ThreadSanitizer report.
"""

import os
import re
import socket
import subprocess
import sys
import tempfile
import time

HERE = os.path.dirname(os.path.abspath(__file__))
TESTS = os.path.dirname(HERE)
TCPKIT = os.environ.get("TCPKIT", os.path.join(TESTS, os.pardir, "src", "tcpkit"))
PORT = int(os.environ.get("STATS_PORT", "34100"))
TIMEOUT = float(os.environ.get("RACE_TIMEOUT", "60"))


def main():
    if not os.path.exists(TCPKIT):
        print("race: %s not built" % TCPKIT)
        return 1

    with tempfile.TemporaryDirectory() as tmp:
        capture = os.path.join(tmp, "stress.pcap")
        subprocess.run([sys.executable, "gen_fixtures.py", "--stress", capture],
                       cwd=os.path.join(TESTS, "fixtures"), check=True,
                       stdout=subprocess.DEVNULL)

        env = dict(os.environ)
        env["TSAN_OPTIONS"] = "halt_on_error=0 " + env.get("TSAN_OPTIONS", "")
        proc = subprocess.Popen(
            [TCPKIT, "-r", capture, "-p", "redis", "-P", str(PORT)],
            stdout=subprocess.DEVNULL, stderr=subprocess.PIPE, text=True, env=env)

        polls = 0
        deadline = time.time() + TIMEOUT
        while proc.poll() is None and time.time() < deadline:
            try:
                conn = socket.create_connection(("127.0.0.1", PORT), timeout=0.5)
                conn.recv(1 << 20)
                conn.close()
                polls += 1
            except OSError:
                time.sleep(0.01)

        try:
            err = proc.communicate(timeout=30)[1]
        except subprocess.TimeoutExpired:
            proc.kill()
            print("race: the capture did not finish in time")
            return 1

    races = re.findall(r"SUMMARY: ThreadSanitizer: data race (.+)", err)
    print("race: %d stats requests served during the replay" % polls)
    if polls == 0:
        print("race: the stats endpoint was never reached, nothing was exercised")
        return 1
    if races:
        print("race: %d data races reported" % len(races))
        for where in sorted(set(races)):
            print("  " + where)
        return 1
    if proc.returncode != 0:
        print("race: the capture exited with %d" % proc.returncode)
        sys.stderr.write(err)
        return 1
    print("race: no data races reported")
    return 0


if __name__ == "__main__":
    sys.exit(main())
