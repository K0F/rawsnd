# rawsnd

A minimalist modular synthesizer in pure C that uses standard Unix pipes and FIFOs as audio buses and patch cables. No frameworks—it uses the original Linux concept of a modular process system. A collection of independent programs communicating via raw 32-bit floating-point numbers (`float`) `sig -> stdin [program] stdout -> sig` at a sample rate of 44100 Hz. Everything is a signal.

## Included Modules

### Generators (Sources)
* **`gen_sine`** – Sine wave oscillator (VCO). If it has no input (`stdin`), it plays a static frequency specified by an argument. As a VCO, it dynamically changes frequency based on incoming values.
* **`gen_lfo`** – Low-frequency oscillator generating `saw` or `tri` (triangle) waveforms in the range `[-1.0, 1.0]`.
* **`gen_env`** – Exponential envelope generator (trigger decay) for shaping tones over time.
* **`gen_harm`** – Additive synthesizer combining 20 harmonic sine waves with automatic normalization and anti-aliasing protection.
* **`gen_noise`** – White noise generator.

### Modifiers and Effects (Processors & FX)
* **`map_lfo`** – Linear interpolation (lerp) of the input LFO signal into an arbitrary target range `[min, max]`.
* **`fx_add`** – Signal adder for additive synthesis.
* **`fx_corrode`** – Bitcrusher and subsampler.
* **`fx_mix`** – Mixer for merging two audio streams together.
* **`fx_mix_pool`** – Mixer for merging more named piped as an arguments together (max 16).
* **`fx_mul`** – Signal multiplier (VCA) for applying envelopes or modulation to the audio signal.
* **`fx_reverb`** – Simple spatial effect (reverb).
* **`fx_lowpass`** – Lowpass filter accepts freq.
* **`fx_highpass`** – Highpass filter accepts freq.

### Sink

* **`dac`** – Default audio sink.
* **`disk_writer`** – Writes wav file with [filename.wav].


## Compilation

To build all binaries, simply run the following in the source directory:

```bash
make
```

## Examples

### Sinus generator

```bash
./gen_sine 440 | ./dac
```

### FM Synth

```bash
./gen_lfo 4.0 tri | ./map_lfo 100.0 1200.0 | ./gen_sine | ./dac
```


### Multivoice /w FIFO

```bash
mkfifo /tmp/env_pipe /tmp/voice2_pipe

./gen_env 2.0 4.0 > /tmp/env_pipe &
./gen_sine 110 > /tmp/voice2_pipe &

./gen_lfo 3.0 tri | ./map_lfo 100.0 800.0 | ./gen_sine | ./fx_mul /tmp/env_pipe | ./fx_mix /tmp/voice2_pipe | ./dac

rm /tmp/env_pipe /tmp/voice2_pipe
```

## Compatibility

It runs on linux like machines, Android termux included. Code should run on MacOs as well (untested).


## WIP

This is very early stage of program... happy hacking!



