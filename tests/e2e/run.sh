#!/bin/sh
# Replays the pcap fixtures through the built binary and checks the output.
set -u

cd "$(dirname "$0")/.." || exit 1

TCPKIT=${TCPKIT:-../src/tcpkit}
STATS_PORT=${STATS_PORT:-33999}
ESC=$(printf '\033')
failures=0

# Colour codes are emitted unconditionally, so strip them before matching.
run() {
    fixture=$1
    shift
    TZ=UTC "$TCPKIT" -r "fixtures/$fixture" -P "$STATS_PORT" "$@" 2>/dev/null \
        | sed "s/${ESC}\[[0-9;]*m//g"
}

ok() { printf '  ok   %s\n' "$1"; }

fail() {
    printf '  FAIL %s\n' "$1"
    failures=$((failures + 1))
}

expect_contains() {
    label=$1
    haystack=$2
    needle=$3
    case $haystack in
        *"$needle"*) ok "$label" ;;
        *) fail "$label: expected to find \"$needle\" in:
$haystack" ;;
    esac
}

expect_lines() {
    label=$1
    got=$(printf '%s' "$2" | grep -c . )
    want=$3
    if [ "$got" -eq "$want" ]; then
        ok "$label"
    else
        fail "$label: expected $want lines, got $got"
    fi
}

echo "== e2e: redis latency"
out=$(run redis-session.pcap -p redis)
expect_lines "reports one line per request" "$out" 2
expect_contains "reports the client and server endpoints" "$out" \
    "10.0.0.1:51137 => 10.0.0.2:6379"
expect_contains "formats the GET command" "$out" "GET a"
expect_contains "formats the SET command" "$out" "SET k v"
expect_contains "reports the GET latency" "$out" "0.615 ms"
expect_contains "reports the SET latency" "$out" "2.000 ms"

echo "== e2e: latency threshold"
out=$(run redis-session.pcap -p redis -t 1)
expect_lines "-t drops requests under the threshold" "$out" 1
expect_contains "-t keeps the slow request" "$out" "SET k v"

echo "== e2e: raw mode"
out=$(run redis-session.pcap -p raw)
expect_lines "prints every captured packet" "$out" 8

echo "== e2e: capture filter"
out=$(run redis-session.pcap -p raw udp)
expect_lines "a udp filter keeps only the dns packet" "$out" 1
expect_contains "the kept packet is the dns query" "$out" "10.0.0.1.40000"

out=$(run redis-session.pcap -p raw "tcp port 9999")
expect_lines "a filter matching nothing prints nothing" "$out" 0

out=$(run redis-session.pcap -p raw "tcp port 6379")
expect_lines "a matching filter keeps the tcp packets" "$out" 7

out=$(run redis-session.pcap -p redis udp)
expect_lines "a udp filter suppresses the redis latency lines" "$out" 0

out=$(run redis-session.pcap -p redis "tcp port 6379")
expect_lines "a matching filter keeps the redis latency lines" "$out" 2

if [ "$failures" -ne 0 ]; then
    echo "e2e: $failures failed"
    exit 1
fi
echo "e2e: all checks passed"
