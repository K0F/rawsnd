#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  FILE *pipe = NULL;

#if defined(__ANDROID__)
    // Android / Termux
    if (system("which mpv > /dev/null 2>&1") != 0) {
        fprintf(stderr, "[CHYBA] Nástroj 'mpv' nebyl nalezen.\n");
        fprintf(stderr, "Pro instalaci na Androidu (Termux) spusťte:\n");
        fprintf(stderr, "  pkg install mpv\n");
        return 1;
    }
    pipe = popen("mpv --no-video --demuxer=rawaudio --demuxer-rawaudio-rate=44100 --demuxer-rawaudio-channels=1 --demuxer-rawaudio-format=floatle - 2>/dev/null", "w");

#elif defined(__APPLE__)
  // macOS prostředí -> vyžaduje Homebrew a ffmpeg
  if (system("which ffplay > /dev/null 2>&1") != 0) {
    fprintf(stderr, "[CHYBA] Nástroj 'ffplay' nebyl nalezen.\n");
    fprintf(stderr, "Pro instalaci na macOS pomocí Homebrew spusťte:\n");
    fprintf(stderr, "  brew install ffmpeg\n");
    return 1;
  }
  // Spuštění ffplay pro raw 32-bit float audio stream na Macu
  pipe = popen("ffplay -autoexit -nodisp -f f32le -ar 44100 -ac 1 -i pipe:0 2>/dev/null", "w");

#elif defined(__linux__)
  // linux alsa-utils (aplay)
  if (system("which aplay > /dev/null 2>&1") != 0) {
    fprintf(stderr, "[CHYBA] Nástroj 'aplay' nebyl nalezen.\n");
    fprintf(stderr, "Nainstalujte jej pomocí správce balíčků:\n");
    fprintf(stderr, "  sudo apt install alsa-utils    (Debian / Ubuntu / Raspberry Pi OS)\n");
    fprintf(stderr, "  sudo pacman -S alsa-utils      (Arch Linux / Manjaro)\n");
    fprintf(stderr, "  sudo dnf install alsa-utils    (Fedora)\n");
    return 1;
  }
  pipe = popen("aplay -t raw -f FLOAT_LE -c 1 -r 44100 2>/dev/null", "w");

#else
  fprintf(stderr, "[CHYBA] Nepodporovaný operační systém.\n");
  return 1;
#endif

  if (!pipe) {
    perror("[CHYBA] Nepodařilo se spustit audio výstup");
    return 1;
  }

  float buffer[512];
  size_t n;

  while ((n = fread(buffer, sizeof(float), 512, stdin)) > 0) {
    if (fwrite(buffer, sizeof(float), n, pipe) != n) {
      break; // Přijímač zavřel rouru
    }
  }

  pclose(pipe);
  return 0;
}
