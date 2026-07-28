# rawsnd

Minimalistický, dřevem voňavý modulární syntezátor v čistém jazyce C, který jako audio sběrnice a propojovací kabely využívá standardní unixové roury (`pipes`) a pojmenované roury (`FIFO`). Žádné složité frameworky, jen nezávislé procesy komunikující pomocí surových 32-bitových plovoucích čísel (`float`) při vzorkovací frekvenci 44100 Hz.

## Obsažené moduly

* **`gen_sine`** – Sinusový oscilátor (VCO). Pokud nemá žádný vstup (`stdin`), hraje statickou frekvenci zadanou argumentem. Pokud je do něj zavedena roura, chová se jako napěťově řízený oscilátor a dynamicky mění frekvenci podle příchozích hodnot.
* **`gen_lfo`** – Nízkofrekvenční oscilátor generující průběhy `saw` (pila) nebo `tri` (trojúhelník) v rozsahu `[-1.0, 1.0]`.
* **`gen_env`** – Generátor exponenciální obálky (trigger decay) pro tvarování tónu v čase.
* **`map_lfo`** – Lineární interpolace (lerp) vstupního LFO signálu do libovolného cílového rozsahu `[min, max]`.
* **`fx_gain`** – Jednoduchý efekt pro úpravu hlasitosti/zesílení signálu koeficientem.
* **`fx_mix`** – Sčítač (mixer) pro slučování dvou audio proudů dohromady (využívá FIFO).
* **`fx_mul`** – Násobič signálů (VCA) pro aplikaci obálky nebo modulace na audio signál.

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
