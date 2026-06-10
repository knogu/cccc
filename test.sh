#!/bin/bash
assert() {
  expected="$1"
  input="$2"


  ~/cccc/main "$input" > tmp.s
  scp ./tmp.s "$user@$host":~/
  ssh "$user@$host" 'cc -o tmp tmp.s && ./tmp'
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 0
assert 42 42

echo OK