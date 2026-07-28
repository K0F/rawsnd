# rawsnd

Minimalistický modulární syntezátor v pure C, který jako audio sběrnice a propojovací kabely využívá standardní unixové pipy a FIFOs. Žádné frameworky, systém používá originální myšlenku linuxu jako modulárního systému procesů. Soubor nezávislých programů komunikující pomocí surových 32-bitových plovoucích čísel (`float`) `sig -> stdin [program] stdout -> sig` při vzorkovací frekvenci 44100 Hz. Vše je signál.


## Obsažené moduly

### Generátory (Sources)
* **`gen_sine`** – Sinusový oscilátor (VCO). Pokud nemá žádný vstup (`stdin`), hraje statickou frekvenci zadanou argumentem. Jako VCO dynamicky mění frekvenci podle příchozích hodnot.
* **`gen_lfo`** – Nízkofrekvenční oscilátor generující průběhy `saw` (pila) nebo `tri` (trojúhelník) v rozsahu `[-1.0, 1.0]`.
* **`gen_env`** – Generátor exponenciální obálky (trigger decay) pro tvarování tónu v čase.
* **`gen_harm`** – Aditivní syntezátor skládající 20 harmonických sinusovek s automatickou normalizací a ochranou proti aliasingu.

### Modifikátory a efekty (Processors & FX)
* **`map_lfo`** – Lineární interpolace (lerp) vstupního LFO signálu do libovolného cílového rozsahu `[min, max]`.
* **`fx_add`** – Sčítač signálů pro míchání proudů.
* **`fx_mix`** – Mixér pro slučování dvou audio proudů dohromady (s podporou FIFO a doplňováním ticha při výpadku).
* **`fx_mul`** – Násobič signálů (VCA) pro aplikaci obálky nebo modulace na audio signál.
* **`fx_reverb`** – Jednoduchý prostorový efekt (comb filter s kruhovým bufferem, feedbackem a tlumicím filtrem).


## Kompilace

Pro sestavení všech binárek stačí v adresáři se zdrojovými kódy spustit:

```bash
make
```

Pro vyčištění zkompilovaných souborů:

## Příklady použití

### Sinus generator

```bash
./gen_sine 440 | aplay -t raw -f FLOAT_LE -c 1 -r 44100
```

### FM Syntéza

```bash
./gen_lfo 4.0 tri | ./map_lfo 100.0 1200.0 | ./gen_sine | aplay -t raw -f FLOAT_LE -c 1 -r 44100
```


### Multivoice s FIFO

```bash
mkfifo /tmp/env_pipe /tmp/voice2_pipe

./gen_env 2.0 4.0 > /tmp/env_pipe &
./gen_sine 110 > /tmp/voice2_pipe &

./gen_lfo 3.0 tri | ./map_lfo 100.0 800.0 | ./gen_sine | ./fx_mul /tmp/env_pipe | ./fx_mix /tmp/voice2_pipe | aplay -t raw -f FLOAT_LE -c 1 -r 44100

rm /tmp/env_pipe /tmp/voice2_pipe
```

## WIP

Toto je velmi raná verze programu, work in progress...
