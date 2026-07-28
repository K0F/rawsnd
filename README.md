# Unix Pipe Modular Synth

Minimalistický, modulární syntezátor v čistém jazyce C, který jako audio sběrnice a propojovací kabely využívá standardní unixové roury (`pipes`). Žádný framework, jen nezávislé procesy komunikující pomocí surových 32-bitových (`floatů`) při vzorkovací frekvenci 44100 Hz.

## Obsažené moduly

* **`gen_sine`** – Sinusový oscilátor (VCO). Pokud nemá žádný vstup (`stdin`), hraje statickou frekvenci zadanou argumentem. Pokud je do něj zavedena roura, chová se jako napěťově řízený oscilátor a dynamicky mění frekvenci podle příchozích hodnot.
* **`fx_gain`** – Jednoduchý efekt pro úpravu hlasitosti/zesílení signálu koeficientem.
* **`gen_lfo`** – Nízkofrekvenční oscilátor generující průběhy `saw` (pila) nebo `tri` (trojúhelník) v rozsahu `[-1.0, 1.0]`.
* **`map_lfo`** – Lineární interpolace (lerp) vstupního LFO signálu do libovolného cílového rozsahu `[min, max]`.

## Kompilace

Pro sestavení všech binárek stačí v adresáři se zdrojovými kódy spustit:

```bash
make
