#!/bin/sh
FIFO_ENV="/tmp/env_pipe"
FIFO_VOICE2="/tmp/voice2_pipe"

# rm $FIFO_ENV $FIFO_VOICE2

mkfifo "$FIFO_ENV"
mkfifo "$FIFO_VOICE2"

./gen_env 2.0 4.0 > "$FIFO_ENV" &
PID_ENV=$!

./gen_sine 110 > "$FIFO_VOICE2" &
PID_VOICE2=$!

cleanup() {
    echo "Stopping processes..."
    kill "$PID_ENV" "$PID_VOICE2" 2>/dev/null
    rm -f "$FIFO_ENV" "$FIFO_VOICE2"
}

trap cleanup INT TERM

./gen_lfo 3.0 tri | ./map_lfo 100.0 800.0 | ./gen_sine | ./fx_mul "$FIFO_ENV" | ./fx_mix "$FIFO_VOICE2" | aplay -t raw -f FLOAT_LE -c 1 -r 44100

cleanup
